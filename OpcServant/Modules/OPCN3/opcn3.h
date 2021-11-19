#ifndef OPCN3_H
#define OPCN3_H
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


#include <fcntl.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <sys/ioctl.h>			//Needed for SPI port
#include <linux/spi/spidev.h>	//Needed for SPI port
#include <string>
#include <iostream>
#include <cstring>
#include <errno.h>


/*!
 * \brief The OPCN3 class
 */
class OPCN3
{
    int _channel = -1;
    int _fd = -1;
    unsigned char spi_mode;
    unsigned char spi_bitsPerWord;
    unsigned int spi_speed;
    //
    static constexpr unsigned SPI_OPC_busy = 0x31;
    static constexpr unsigned SPI_OPC_ready = 0xF3;
    //

public:

#pragma pack(push,1)
    struct firmwareVersion
    {
        uint8_t major;
        uint8_t minor;
    };

    struct serialNumber
    {
        uint8_t  serial[6];

    };

    struct informationString
    {
        uint8_t information[60];
    };

    struct histogramData
    {
        uint16_t binCount0  ;
        uint16_t binCount1  ;
        uint16_t binCount2  ;
        uint16_t binCount3  ;
        uint16_t binCount4  ;
        uint16_t binCount5  ;
        uint16_t binCount6  ;
        uint16_t binCount7  ;
        uint16_t binCount8  ;
        uint16_t binCount9  ;
        uint16_t binCount10 ;
        uint16_t binCount11 ;
        uint16_t binCount12 ;
        uint16_t binCount13 ;
        uint16_t binCount14 ;
        uint16_t binCount15 ;
        uint16_t binCount16 ;
        uint16_t binCount17 ;
        uint16_t binCount18 ;
        uint16_t binCount19 ;
        uint16_t binCount20 ;
        uint16_t binCount21 ;
        uint16_t binCount22 ;
        uint16_t binCount23 ;

        uint8_t bin1TimeToCross  ;
        uint8_t bin3TimeToCross  ;
        uint8_t bin5TimeToCross  ;
        uint8_t bin7TimeToCross  ;

        uint16_t samplingPeriod ;
        uint16_t sampleFlowRate ;
        uint16_t temperature    ;
        uint16_t humidity       ;

        float pm1     ;
        float pm2_5   ;
        float pm10   ;

        uint16_t rejectCountGlitch       ;
        uint16_t rejectCountLongTOF      ;
        uint16_t rejectCountRatio        ;
        uint16_t rejectCountOutOfRange   ;
        uint16_t fanRevCount             ;
        uint16_t laserStatus             ;
        uint16_t checkSum                ;
    };

    struct DACandPowerStatus
    {
        uint8_t fanOn;
        uint8_t laserDACOn;
        uint8_t fanDACVal;
        uint8_t laserDACVal;
        uint8_t laserSwitch;
        uint8_t gainAndAutoGainToggleSetting;
    };

    struct pmData
    {
        float pm1    ;
        float pm2_5  ;
        float pm10   ;
        uint16_t checkSum ;
    };

    struct configurationVariables
    {

        uint16_t binBoundriesADC0;
        uint16_t binBoundriesADC1;
        uint16_t binBoundriesADC2;
        uint16_t binBoundriesADC3;
        uint16_t binBoundriesADC4;
        uint16_t binBoundriesADC5;
        uint16_t binBoundriesADC6;
        uint16_t binBoundriesADC7;
        uint16_t binBoundriesADC8;
        uint16_t binBoundriesADC9;
        uint16_t binBoundriesADC10;
        uint16_t binBoundriesADC11;
        uint16_t binBoundriesADC12;
        uint16_t binBoundriesADC13;
        uint16_t binBoundriesADC14;
        uint16_t binBoundriesADC15;
        uint16_t binBoundriesADC16;
        uint16_t binBoundriesADC17;
        uint16_t binBoundriesADC18;
        uint16_t binBoundriesADC19;
        uint16_t binBoundriesADC20;
        uint16_t binBoundriesADC21;
        uint16_t binBoundriesADC22;
        uint16_t binBoundriesADC23;
        uint16_t binBoundriesADC24;


        uint16_t binBoundriesDiametor0;
        uint16_t binBoundriesDiametor1;
        uint16_t binBoundriesDiametor2;
        uint16_t binBoundriesDiametor3;
        uint16_t binBoundriesDiametor4;
        uint16_t binBoundriesDiametor5;
        uint16_t binBoundriesDiametor6;
        uint16_t binBoundriesDiametor7;
        uint16_t binBoundriesDiametor8;
        uint16_t binBoundriesDiametor9;
        uint16_t binBoundriesDiametor10;
        uint16_t binBoundriesDiametor11;
        uint16_t binBoundriesDiametor12;
        uint16_t binBoundriesDiametor13;
        uint16_t binBoundriesDiametor14;
        uint16_t binBoundriesDiametor15;
        uint16_t binBoundriesDiametor16;
        uint16_t binBoundriesDiametor17;
        uint16_t binBoundriesDiametor18;
        uint16_t binBoundriesDiametor19;
        uint16_t binBoundriesDiametor20;
        uint16_t binBoundriesDiametor21;
        uint16_t binBoundriesDiametor22;
        uint16_t binBoundriesDiametor23;
        uint16_t binBoundriesDiametor24;

        uint16_t binWeightings0;
        uint16_t binWeightings1;
        uint16_t binWeightings2;
        uint16_t binWeightings3;
        uint16_t binWeightings4;
        uint16_t binWeightings5;
        uint16_t binWeightings6;
        uint16_t binWeightings7;
        uint16_t binWeightings8;
        uint16_t binWeightings9;
        uint16_t binWeightings10;
        uint16_t binWeightings11;
        uint16_t binWeightings12;
        uint16_t binWeightings13;
        uint16_t binWeightings14;
        uint16_t binWeightings15;
        uint16_t binWeightings16;
        uint16_t binWeightings17;
        uint16_t binWeightings18;
        uint16_t binWeightings19;
        uint16_t binWeightings20;
        uint16_t binWeightings21;
        uint16_t binWeightings22;
        uint16_t binWeightings23;

        uint16_t pmDiametorA;
        uint16_t pmDiametorB;
        uint16_t pmDiametorC;

        uint16_t maximumTimeOfFlight;
        uint16_t AMSamplingIntervalCount;
        uint16_t AMIdleIntervalCount;
        uint16_t AMMaxDataArraysInFile;

        uint8_t AMOnlySavePMData;
        uint8_t AMFanOnInIdle;
    };

#pragma pack(pop)


    enum
    {
        OP_WRITE_STATUS = 3,
        OP_READ_DAC_AND_POWER = 0x13,
        OP_SET_FAN_LASER_POT=0x42,
        OP_SET_BIN_WEIGHTING_INDEX=0x5,
        OP_READ_INFORMATION_STRING = 0x3F,
        OP_READ_SERIAL_NUMBER=0x10,
        OP_WRITE_SERIAL_NUMBER=0x11,
        OP_READ_FIRMWARE_VERSION=0x12,
        OP_READ_HISTOGRAM_DATA=0x30,
        OP_READ_PM_DATA=0x32,
        OP_SAVE_CONFIG=0x43,
        OP_CHECK_STATUS = 0xCF,
        OP_RESET = 0x06,
        OP_ENTER_BOOTLOADER=0x41,
        OP_READ_CONFIG=0x3C
    };


    typedef std::vector<uint8_t> BUFFER;
    /*!
     * \brief OPCN3
     * \param ch
     */
    OPCN3(int ch = 0);
    /*!
     * \brief fd
     * \return
     */
    int fd() const {
        return _fd;
    }

    template<typename  T>
    /*!
     * \brief read
     * \param op
     * \param c
     * \return
     */
    bool read(uint8_t op, T &c)
    {
        bool ret = false;
        BUFFER b(sizeof(T),0);
        if(spiOpenPort())
        {
            if(getReadyResponse(op))
            {
                delayMs(20);
                ret = transfer(op,b);
                if(ret)
                {
                    memcpy(&c,b.data(),sizeof(T));
                }
                else {
                    memset(&c,0,sizeof(T));
                }
            }
            spiClosePort();
        }
        return ret;
    }




    /*!
     * \brief calcCRC
     * \param data
     * \param nbrOfBytes
     * \return
     */
    unsigned int calcCRC(uint8_t data[], unsigned char nbrOfBytes);


    //Convert SHT31 ST output to Temperature (C)
    /*!
     * \brief convSTtoTemperature
     * \param ST
     * \return
     */
    double convSTtoTemperature (unsigned int ST)
    {
        return -45 + 175*(float)ST/65535;
    }


    //Convert SHT31 SRH output to Relative Humidity (%)
    /*!
     * \brief convSRHtoRelativeHumidity
     * \param SRH
     * \return
     */
    double convSRHtoRelativeHumidity (unsigned int SRH)
    {
        return 100*(float)SRH/65535;
    }

    /*!
     * \brief delayMs
     * \param i
     */
    void delayMs(unsigned i)
    {
        usleep(i * 1000);
    }

    /*!
     * \brief forceCsHigh
     */
    void forceCsHigh()
    {
        unsigned char response = 0;
        spiWriteAndRead ( &response, &response, 1, false); // force CS high
        delayMs(1);  //wait 1ms
    }

    bool getReadyResponse (unsigned char SPIcommand);
    /*!
     * \brief start
     * \param spiDevice
     * \return
     */
    bool start();
    /*!
     * \brief start
     * \param spiDevice
     * \return
     */
    bool stop();


    /*!
     * \brief transfer
     * \param op
     * \param b
     * \return
     */
    bool transfer( uint8_t op, BUFFER &b)
    {
        //
        uint8_t *p = b.data();
        for(size_t i = 0; i < b.size(); i++,p++)
        {
            spiWriteAndRead ( &op, p, 1, true); // keep CS Low
            usleep(20);
        }
        return true;
    }

    /*!
    * \brief spiOpenPort
     * \return
    */
    bool spiOpenPort ();
    /*!
     * \brief spiClosePort
     * \return
     */
    void spiClosePort ()
    {
        if(_fd > 0) close(_fd);
        _fd = -1;
    }



    //*******************************************
    //*******************************************
    //********** SPI WRITE & READ DATA **********
    //*******************************************
    //*******************************************
    //SpiDevice		0 or 1
    //TxData and RxData can be the same buffer (read of each byte occurs before write)
    //Length		Max 511 (a C SPI limitation it seems)
    //LeaveCsLow	1=Do not return CS high at end of transfer (you will be making a further call to transfer more data), 0=Set CS high when done
    /*!
     * \brief spiWriteAndRead
     * \param TxData
     * \param RxData
     * \param Length
     * \param LeaveCsLow
     * \return
     */
    bool spiWriteAndRead ( unsigned char *TxData, unsigned char *RxData, int Length, bool LeaveCsLow);
    /*!
     * \brief printHistogram
     * \param h
     */
    void printHistogram(OPCN3::histogramData &h);
};

#endif // OPCN3_H
