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
#ifndef MRLMODBUS_MODBUS_H
#define MRLMODBUS_MODBUS_H
#include <string>
#include <vector>
#include <MrlLib/trace.hpp>
extern "C"
{
#include "modbus/modbus.h"
}

//
// Modbusinterface using libmodbus
// each connection is a thread - so transactions can be event driven or treated as blocking
//
namespace MrlModbus {

// modbus connection
typedef union
{
    int16_t i;
    uint16_t u;
} MODBUSVALUE;

typedef std::vector<uint16_t> ModbusRegisterArray;
typedef std::vector<bool> ModbusCoilArray;



/**
    @brief MODBUS interface using libmodbus

*/


class  Modbus  {
    modbus_t *_ctx; // the connection handle
    // where the reads are cached
    int _lastError; // last error code
    int _numberRetries;  // max number of retires before giving up
    //
public:
    /**
        @brief

        @param parent
    */
    explicit Modbus();
    /**
        @brief

    */
    ~Modbus() {
        if (_ctx) {
            close();
            modbus_free(_ctx);
        }
    }

    /*!
     * \brief flush
     */
    void flush()
    {
        if(_ctx) modbus_flush(_ctx);
    }
    //
    /**
        @brief

        @return modbus_t
    */
    modbus_t *handle() {
        // calling handle traps invalid use
        _lastError = -1;
        if (!_ctx) THROW_DEF("Null MODBUS Context");
        return _ctx;
    }

    const char * strerror()
    {
        return modbus_strerror(int errno);
    }
    /**
        @brief

        @param h
        @param port
        @param parent
    */
    bool setTcpConnection(const std::string &h, const std::string &port = "502"); // MODBUS/TCP connection

    /**
        @brief

        @param port
        @param baud_rate
        @param parity
        @param data_bit
        @param stop_bit
        @param parent
    */
    bool setSerialConnection(const std::string &port, int baud_rate = 9600, char parity = 'N', int data_bit = 8, int stop_bit = 1); // Serial connection
    // TO DO add MODBUS RTU over TCP (eg connecting to a ethernet to serial interface)
    /**
        @brief

        @return bool
    */
    bool connectTo(); // make the connection
    /**
        @brief

        @return bool
    */
    bool isConnected() {
        return _ctx && (modbus_get_socket(_ctx) > 0);
    }
    /**
        @brief

        @param s
    */
    void setSocket(int s) {
        modbus_set_socket(handle(), s);
    }
    /**
        @brief

    */
    void close() {
        if (isConnected()) {
            modbus_close(handle());
        }
    }


    /**
        @brief

        @return int
    */
    int lastError() {
        return _lastError;
    }

    /**
        @brief

        @return int
    */
    int numberRetries() const {
        return _numberRetries;
    }
    /**
        @brief

        @param n
    */
    void setNumberRetries(int n) {
        _numberRetries = n;
    }




    /**
        @brief Sets the current slave address

        @param slave
        @return bool
    */
    bool setSlave(int slave) {
        _lastError = -1;
        return _ctx && ((_lastError = modbus_set_slave(handle(), slave)) == 0);
    }
    /**
        @brief

        @param slave
        @param addr
        @param nb
        @return QByteArray
    */
    bool  readBits(int slave, int addr, int nb, ModbusCoilArray &coils) {
        coils.clear();
        if (setSlave(slave)) {
            coils.resize(nb);
            for (int i = 1; i < _numberRetries; i++) {
                unsigned char bits[nb];
                if ((_lastError = modbus_read_bits(handle(), addr,  nb, (uint8_t *)bits)) >= 0) {
                    for (int i = 0; i < nb; i++) {
                        coils.push_back(bits[i] != 0);
                    }
                    return true;
                }
            }
        }
        return false;
    }

    /**
        @brief

        @param slave
        @param addr
        @param nb
        @return ModbusRegisterArray
    */
    bool readRegisters(int slave, int addr,  ModbusRegisterArray &res) {
        uint16_t *p = (uint16_t *)(res.data());
        return readRegisters(slave,addr,res.size(),p);
    }


    /*!
     * \brief readRegisters
     * \param slave
     * \param addr
     * \param nb
     * \param res
     * \return  true on success
     */
    bool readRegisters(int slave, int addr, int nb, uint16_t *res) {
        if (setSlave(slave)) {
            for (int i = 1; i < _numberRetries; i++) {
                _lastError = modbus_read_registers(handle(), addr, nb, res);
                if (_lastError  >= 0) {
                    return true;
                }
            }
        }
        return false;
    }


    /**
        @brief

        @param slave
        @param addr
        @param nb
        @return ModbusRegisterArray
    */
    bool readInputRegisters(int slave, int addr,  ModbusRegisterArray &ret) {
        uint16_t *p = (uint16_t *)(ret.data());
        return readInputRegisters(slave,addr, ret.size(),p);
    }

    /*!
     * \brief readInputRegisters
     * \param slave
     * \param addr
     * \param nb
     * \param ret
     * \return true on success
     */
    bool readInputRegisters(int slave, int addr, int nb, uint16_t *res) {
        if (setSlave(slave)) {
            for (int i = 1; i < _numberRetries; i++) {
                _lastError = modbus_read_input_registers(handle(), addr, nb, res);
                if (_lastError >= 0) {
                    return true;
                }
            }
        }
        return false;
    }


    /**
        @brief

        @param coil_addr
        @param status
        @return true on success
    */
    bool writeBit(int slave, int coil_addr, int status) {
        if(setSlave(slave))
        {
            for (int i = 1; i < _numberRetries; i++) {
                if ((_lastError = modbus_write_bit(handle(), coil_addr, status)) >= 0) break;
            }
        }
        return _lastError >= 0;
    }

    /**
        @brief

        @param reg_addr
        @param value
        @return true on success
    */
    bool writeRegister(int slave, int reg_addr, int value) {
        _lastError = -1;
        if(setSlave(slave))
        {
            for (int i = 1; i < _numberRetries; i++) {
                if ((_lastError = modbus_write_register(handle(), reg_addr, value)) >= 0) break;

            }
        }
        return _lastError >= 0;
    }

    /**
        @brief

        @param addr
        @param data
        @return true on success
    */
    bool writeBits(int slave, int addr, ModbusCoilArray &data) {
        if(setSlave(slave))
        {
            unsigned char bits[data.size()];
            for (int i = 0; i < (int)data.size(); i++) {
                bits[i] = data[i] ? 0xFF : 0;
            }
            for (int i = 1; i < _numberRetries; i++) {
                if ((_lastError = modbus_write_bits(handle(), addr, data.size(), (const uint8_t *)bits)) >= 0) break;
            }
        }
        return _lastError >= 0;
    }

    /**
        @brief

        @param addr
        @param data
        @return true on success
    */
    bool writeRegisters(int slave, int addr, ModbusRegisterArray &data) {
        uint16_t *p = (uint16_t *)(data.data());
        return writeRegisters(slave,addr,data.size(), p);
    }

    /*!
     * \brief writeRegisters
     * \param slave
     * \param addr
     * \param data
     * \return true on success
     */
    bool writeRegisters(int slave, int addr, int size, const uint16_t *data) {
        if(setSlave(slave))
        {
            for (int i = 1; i < _numberRetries; i++) {
                if ((_lastError = modbus_write_registers(handle(), addr, size, data)) >= 0) break;
            }
        }
        return _lastError >= 0;
    }



    /**
        @brief

        @param write_addr
        @param write_nb
        @param writeReg
        @param read_addr
        @param readReg
        @return true on success
    */
    bool writeAndReadRegisters(int slave, int write_addr, ModbusRegisterArray &writeReg, int read_addr, ModbusRegisterArray &readReg) {
        if(setSlave(slave))
        {
            uint16_t *p = (uint16_t *)(writeReg.data());
            uint16_t *q = (uint16_t *)(readReg.data());

            for (int i = 1; i < _numberRetries; i++) {
                if ((_lastError = modbus_write_and_read_registers(handle(),
                                  write_addr,
                                  writeReg.size(),
                                  (const uint16_t *)p,
                                  read_addr, readReg.size(),q)) >= 0) break;
            }
        }
        return _lastError >= 0;
    }

    /*!
     * \brief setTimeout
     * \param sec
     * \param us
     * \return true on success
     */
    bool setTimeout(uint32_t sec, uint32_t us = 0)
    {
        if(_ctx)
        {
            // set the transaction timeout
            return modbus_set_response_timeout(_ctx, sec, us) == 0;
        }
        return false;
    }

};

/*!
 * \brief The ModbusSlave class
 */
class ModbusSlave {
    uint8_t _request[MODBUS_RTU_MAX_ADU_LENGTH];//Will contain internal libmodubs data from a request that must be given back to answer
    modbus_t *_ctx = nullptr;
    modbus_mapping_t *_mb_mapping = nullptr;
    int _lastError = 0;
public:
    /*!
     * \brief ModbusSlave
     */
    ModbusSlave() {}


    ~ModbusSlave()
    {
        close();
        if(_mb_mapping) modbus_mapping_free(_mb_mapping);
    }

    /*!
     * \brief setMapping
     * \param nb_bits
     * \param nb_input_bits
     * \param nb_registers
     * \param nb_input_registers
     * \return true on sucess
     */
    bool setMapping(int nb_bits = 256, int nb_input_bits = 256,
                    int nb_registers = 256, int nb_input_registers = 256)
    {
        // useful defaults
        if(_mb_mapping) modbus_mapping_free(_mb_mapping);
        _mb_mapping = modbus_mapping_new(nb_bits,nb_input_bits,nb_registers,nb_input_registers);
        return _mb_mapping != nullptr;
    }

    /*!
     * \brief open
     * \param port
     * \param baudRate
     * \return
     */
    bool open(const std::string &port, int baudRate = 115200 )
    {
        close();
        _ctx = modbus_new_rtu(port.c_str(), baudRate, 'N', 8, 1);
        if (_ctx == NULL) {
            return false;
        }
        _lastError = modbus_connect(_ctx);
        return (_lastError >= 0);
    }
    /*!
     * \brief close
     */
    void close()
    {
        if(_ctx)
        {
            modbus_close(_ctx);
            modbus_free(_ctx);
        }
        _ctx = nullptr;
    }
    /*!
     * \brief setSlave
     * \param slave
     * \return  true on success
     */
    bool setSlave(int slave) {
        _lastError = -1;
        return _ctx && ((_lastError = modbus_set_slave(_ctx, slave)) == 0);
    }
    /*!
     * \brief process
     */
    virtual void process()
    {
        if(_ctx)
        {
            int rc = modbus_receive(_ctx, _request); // rx a record
            if(rc > 0)
            {
                _lastError = modbus_reply(_ctx,_request,rc,_mb_mapping); // post the reply
            }
        }
    }
    // accessors
    modbus_t * handle() const { return _ctx;}
    modbus_mapping_t * mapping() const { return _mb_mapping;}
};

}
#endif // MODBUS_H
