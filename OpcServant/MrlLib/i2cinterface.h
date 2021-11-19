/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#ifndef I2CINTERFACESCRIPT_H
#define I2CINTERFACESCRIPT_H
/*!
 *
 *  Author:  B. J. Hill
 */

// get the definitions
#include <MrlLib/mrllib.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <wx/utils.h>
#include <modbus/modbus-tcp.h>

//#ifdef RASPBERRY_PI_BUILD
#define I2C_DEF_DEV "/dev/i2c-1"
//#else
// USB - I2C adapter
// use udev rules to map across
//#define I2C_DEF_DEV "/dev/i2c-ch341"
//#endif
namespace MRL {


/*!
 * \brief The I2cInterface class
 * Communications with I2c devices - base class
 */
class  I2cInterfaceBase
{

public:
    I2cInterfaceBase()
    {

    }

    virtual ~I2cInterfaceBase();
    /*!
     * \brief open
     * \param s i2c device to access
     * \return true on success
     */
    virtual bool start(int slave)
    {
        return false;
    }
    /*!
     * \brief close
     */
    virtual void end()
    {
    }
    /*!
     * \brief write
     * \param device address
     * \param reg register
     * \param value value to set
     */
    template <typename T>
    bool  write(  int reg, const T& value)
    {
        return writeBlock(reg,(const uint8_t *)&value, sizeof(T));
    }
    /*!
     * \brief write
     * \param device
     * \param reg
     * \param buf
     * \param len
     */
    virtual bool writeBlock( int reg, const uint8_t * buf, int len)
    {
        return false;

    }


    /*!
     * \brief read
     * \param device device id to read from
     * \param address register to read from
     * \return value read
     */
    template <typename T>
    bool read( int address, T &v)
    {
        return readBlock(address, (uint8_t *)&v, sizeof(T));
    }
    /*!
     * \brief read
     * \param device
     * \param address
     * \param buf
     * \param len
     * \return true on success
     */
    virtual bool  readBlock( int address, uint8_t *buf, int len)
    {
        return false;
    }

    /*!
     * \brief blockRead
     * \param buf
     * \param len
     * \return true on success
     */
    virtual bool blockRead(uint8_t */*buf*/, int /*len*/)
    {
        return false;
    }

    /*!
     * \brief blockWrite
     * \param buf
     * \param len
     * \return true on success
     */
    virtual bool blockWrite(const uint8_t */*buf*/, int /*len*/)
    {
        return false;
    }

    typedef enum
    {
        LOCAL = 0, REMOTE
    } Type;

    typedef std::unique_ptr<I2cInterfaceBase> Ptr;
    static void makeI2c(Type type, Ptr &i2c,const std::string &address = I2C_DEF_DEV, int port = 5001 ) ;

};
/*!
 * \brief The I2cInterfaceLocal class
 * Local I2C bus
 */
class  I2cInterfaceLocal : public I2cInterfaceBase
{
    int _fd = -1;
    std::string _dev;

public:
    I2cInterfaceLocal(const std::string &dev = I2C_DEF_DEV) : _dev(dev)
    {

    }

    virtual ~I2cInterfaceLocal()
    {
        end();
    }
    /*!
     * \brief open
     * \param s i2c device to access
     * \return true on success
     */
    bool start(int slave)
    {
        if ((_fd = ::open(_dev.c_str(), O_RDWR)) > 0)
        {
            return ::ioctl(_fd, I2C_SLAVE, slave) == 0;
        }
        return false;
    }
    /*!
     * \brief close
     */
    void end()
    {
        if(_fd > 0) ::close(_fd);
        _fd = -1;
    }
    /*!
     * \brief write
     * \param device
     * \param reg
     * \param buf
     * \param len
     */
    bool writeBlock( int reg, const uint8_t * buf, int len)
    {
        if(buf && (_fd > 0) && (len > 0))
        {
            uint8_t b[len +1];
            memcpy(b+1,buf,len);
            b[0] = uint8_t(reg);
            return ::write(_fd,b,len+1) == (len + 1);
        }
        return false;
    }


    /*!
     * \brief read
     * \param device
     * \param address
     * \param buf
     * \param len
     * \return true on success
     */
    bool  readBlock( int address, uint8_t *buf, int len)
    {
        if(buf && (_fd > 0) && (len > 0))
        {
            uint8_t r = uint8_t(address);
            if (::write(_fd, &r, sizeof(r)) == sizeof(r))
            {
                return ::read(_fd, buf, len) == len;
            }
        }
        return false;
    }

    /*!
     * \brief blockRead
     * \param buf
     * \param len
     * \return true on success
     */
    virtual bool blockRead(uint8_t *buf, int len)
    {
        if(buf && (_fd > 0) && (len > 0))
        {
            return ::read(_fd, buf, len) == len;
        }
        return false;
    }

    /*!
     * \brief blockWrite
     * \param buf
     * \param len
     * \return true on success
     */
    virtual bool blockWrite(const uint8_t *buf, int len)
    {
        if(buf && (_fd > 0) && (len > 0))
        {
            return ::write(_fd,buf,len) == len;
        }
        return false;
    }

};

/*!
 * \brief The I2cInterfaceRemote class
 * Access via the MODBUC I2C interface
 */
class  I2cInterfaceRemote : public I2cInterfaceBase
{
    std::string _dev;
    int _port = 0;
    modbus_t *_ctx = nullptr;
public:
    /*!
     * \brief I2cInterfaceRemote
     * \param dev
     * \param port
     */
    I2cInterfaceRemote(const std::string &dev, int port = 5001) :
        _dev(dev), _port(port)
    {

    }

    virtual ~I2cInterfaceRemote();
    /*!
     * \brief open
     * \param s i2c device to access
     * \return true on success
     */
    bool start(int slave);
    /*!
     * \brief close
     */
    void end();
    /*!
     * \brief write
     * \param device
     * \param reg
     * \param buf
     * \param len
     */
    bool writeBlock( int reg, const uint8_t * buf, int len);
    /*!
     * \brief read
     * \param device
     * \param address
     * \param buf
     * \param len
     * \return true on success
     */
    bool  readBlock( int address, uint8_t *buf, int len);

    /*!
     * \brief blockRead
     * \param buf
     * \param len
     * \return true on success
     */
    virtual bool blockRead(uint8_t *buf, int len);

    /*!
     * \brief blockWrite
     * \param buf
     * \param len
     * \return true on success
     */
    virtual bool blockWrite(const uint8_t *buf, int len);

};








}

#endif // I2CINTERFACESCRIPT_H
