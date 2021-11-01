#include "opcn3.h"

/*!
 * \brief OPCN3::OPCN3
 * \param ch
 */
OPCN3::OPCN3(int ch) : _channel(ch)
{

}

/*!
 * \brief OPCN3::calcCRC
 * \param data
 * \param nbrOfBytes
 * \return
 */
unsigned int OPCN3::calcCRC(uint8_t data[], unsigned char nbrOfBytes)
{
    static const unsigned POLYNOMIAL_MODBUS = 0xA001; //Generator polynomial for MODBUS crc
    static const unsigned InitCRCval_MODBUS = 0xFFFF; //Initial CRC value

    unsigned char _bit; // bit mask
    unsigned int crc = InitCRCval_MODBUS; // initialise calculated checksum
    unsigned char byteCtr; // byte counter

    // calculates 16-Bit checksum with given polynomial
    for(byteCtr = 0; byteCtr < nbrOfBytes; byteCtr++)
    {
        crc ^= (unsigned int)data[byteCtr];
        for(_bit = 0; _bit < 8; _bit++)
        {
            if (crc & 1) //if bit0 of crc is 1
            {
                crc >>= 1;
                crc ^= POLYNOMIAL_MODBUS;
            }
            else
                crc >>= 1;
        }
    }
    return crc;
}


/*!
 * \brief OPCN3::getReadyResponse
 * \param SPIcommand
 * \return
 */
bool OPCN3::getReadyResponse (unsigned char SPIcommand)
{
    unsigned char response;
    //Try reading a byte here to clear out anything remnant of SD card SPI activity (WORKS!)
    unsigned char tries = 0;
    do
    {
        spiWriteAndRead ( &SPIcommand, &response, 1, true); // keep CS low
        if (response != SPI_OPC_ready) delayMs(1); //wait 1ms
    } while ((tries++ < 20) && (response != SPI_OPC_ready));

    if (response != SPI_OPC_ready)
    {
        //std::cout << "getReadyResponse fails " << int(response) << std::endl;
        return false;
    }
    //std::cout << "getReadyResponse success " << int(response) << std::endl;
    return true;
}
/*!
 * \brief start
 * \param spiDevice
 * \return
 */
bool OPCN3::start()
{
    bool ret = false;
    uint8_t laserOn = 7;
    uint8_t fanOn = 3;
    uint8_t response;
    //Turn ON fan and laser
    if(spiOpenPort())
    {
        //Laser power ON
        if(getReadyResponse(OP_WRITE_STATUS))
        {
            if(spiWriteAndRead ( &laserOn, &response, 1, true))
            {
                spiClosePort();
                delayMs(1000);
                spiOpenPort();
                //Fan DAC ON
                if(getReadyResponse(OP_WRITE_STATUS))
                {
                    ret = spiWriteAndRead ( &fanOn, &response, 1, true);
                    delayMs(2000);
                }
            }
        }

        spiClosePort();
    }
    return ret;
}
/*!
 * \brief start
 * \param spiDevice
 * \return
 */
bool OPCN3::stop()
{
    bool ret = false;
    if(spiOpenPort())
    {
        uint8_t laserOff = 6;
        uint8_t fanOff = 2;
        uint8_t response;
        //Laser power OFF
        getReadyResponse(OP_WRITE_STATUS);
        delayMs(20);
        if(spiWriteAndRead ( &laserOff, &response, 1, true))
        {
            spiClosePort();
            delayMs(1000);
            spiOpenPort();
            //Fan DAC ON
            getReadyResponse(OP_WRITE_STATUS);
            delayMs(20);
            ret = spiWriteAndRead ( &fanOff, &response, 1, true);
        }
        spiClosePort();
    }
    return ret;
}


void OPCN3::printHistogram(OPCN3::histogramData &h)
{
    std::cout << "Bin Counts" << std::endl;
    std::cout << std::hex << h.binCount0<< std::endl;
    std::cout << std::hex <<  h.binCount1<< std::endl;
    std::cout << std::hex <<  h.binCount2<< std::endl;
    std::cout << std::hex <<  h.binCount3<< std::endl;
    std::cout << std::hex <<  h.binCount4<< std::endl;
    std::cout << std::hex <<  h.binCount5<< std::endl;
    std::cout << std::hex <<  h.binCount6<< std::endl;
    std::cout << std::hex <<  h.binCount7<< std::endl;
    std::cout << std::hex <<  h.binCount8<< std::endl;
    std::cout << std::hex <<  h.binCount9 << std::endl;
    std::cout << std::hex <<  h.binCount10<< std::endl;
    std::cout << std::hex <<  h.binCount11<< std::endl;
    std::cout << std::hex <<  h.binCount12<< std::endl;
    std::cout << std::hex <<  h.binCount13<< std::endl;
    std::cout << std::hex <<  h.binCount14<< std::endl;
    std::cout << std::hex <<  h.binCount15<< std::endl;
    std::cout << std::hex <<  h.binCount16<< std::endl;
    std::cout << std::hex <<  h.binCount17<< std::endl;
    std::cout << std::hex <<  h.binCount18<< std::endl;
    std::cout << std::hex <<  h.binCount19 << std::endl;
    std::cout << std::hex <<  h.binCount20<< std::endl;
    std::cout << std::hex <<  h.binCount21<< std::endl;
    std::cout << std::hex <<  h.binCount22<< std::endl;
    std::cout << std::hex <<  h.binCount23<< std::endl;

    std::cout << std::endl << "Time To Cross" << std::endl;
    std::cout << h.bin1TimeToCross<< std::endl;
    std::cout << h.bin3TimeToCross<< std::endl;
    std::cout << h.bin5TimeToCross<< std::endl;
    std::cout << h.bin7TimeToCross << std::endl;

    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "Sampling Period" << std::endl;
    std::cout << h.samplingPeriod << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "Sample Flow Rate" << std::endl;
    std::cout << h.sampleFlowRate << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "Temperature" << std::endl;
    std::cout << h.temperature << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "Humidity" << std::endl;
    std::cout << h.humidity << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "pm1" << std::endl;
    std::cout << h.pm1 << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "pm2.5" << std::endl;
    std::cout << h.pm2_5 << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "pm10" << std::endl;
    std::cout << h.pm10 << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;

    std::cout << "-------------------------------------------------" << std::endl;

    std::cout << h.rejectCountGlitch<< std::endl;
    std::cout << h.rejectCountLongTOF<< std::endl;
    std::cout << h.rejectCountRatio<< std::endl;
    std::cout << h.rejectCountOutOfRange<< std::endl;
    std::cout << h.fanRevCount<< std::endl;
    std::cout << h.laserStatus<< std::endl;
    std::cout << h.checkSum<< std::endl;

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
 * \brief OPCN3::spiWriteAndRead
 * \param TxData
 * \param RxData
 * \param Length
 * \param LeaveCsLow
 * \return
 */
bool OPCN3::spiWriteAndRead ( unsigned char *TxData, unsigned char *RxData, int Length, bool LeaveCsLow)
{
    struct spi_ioc_transfer spi;
    int retVal = -1;
    if(_fd > 0)
    {
        spi.tx_buf = (unsigned long)TxData;		//transmit from "data"
        spi.rx_buf = (unsigned long)RxData;		//receive into "data"
        spi.len = (unsigned)(Length);
        spi.delay_usecs = 5;
        spi.speed_hz = spi_speed;
        spi.bits_per_word = spi_bitsPerWord;
        spi.cs_change = LeaveCsLow?1:0;						//0=Set CS high after a transfer, 1=leave CS set low
        spi.rx_nbits = 8;
        spi.tx_nbits = 8;
        spi.pad = 0;
        //
        retVal = ioctl(_fd, SPI_IOC_MESSAGE(1), &spi);
    }
//    if(retVal < 0 )
//    {
//        std::cout << "Send Error " << strerror(errno) << std::endl;
//    }
    return retVal == Length ;
}

bool OPCN3::spiOpenPort ()
{
    int status_value = -1;


    //----- SET SPI MODE -----
    //SPI_MODE_0 (0,0) 	CPOL = 0, CPHA = 0, Clock idle low, data is clocked in on rising edge, output data (change) on falling edge
    //SPI_MODE_1 (0,1) 	CPOL = 0, CPHA = 1, Clock idle low, data is clocked in on falling edge, output data (change) on rising edge
    //SPI_MODE_2 (1,0) 	CPOL = 1, CPHA = 0, Clock idle high, data is clocked in on falling edge, output data (change) on rising edge
    //SPI_MODE_3 (1,1) 	CPOL = 1, CPHA = 1, Clock idle high, data is clocked in on rising, edge output data (change) on falling edge
    spi_mode = SPI_MODE_1;

    //----- SET BITS PER WORD -----
    spi_bitsPerWord = 8;

    //----- SET SPI BUS SPEED -----
    spi_speed = 300000;

//
// these are present on the Raspberry Pi
// in principle we could use USB - SPI adapters and have loads of OPCs
//
    if (_channel)
        _fd = open("/dev/spidev0.1", O_RDWR);
    else
        _fd = open("/dev/spidev0.0", O_RDWR);

    if (_fd < 0)
    {
        perror("Error - Could not open SPI device");
        return false;
    }

    status_value = ioctl(_fd, SPI_IOC_WR_MODE, &spi_mode);
    if(status_value < 0)
    {
        perror("Could not set SPIMode (WR)...ioctl fail");
        return false;
    }

    status_value = ioctl(_fd, SPI_IOC_RD_MODE, &spi_mode);
    if(status_value < 0)
    {
        perror("Could not set SPIMode (RD)...ioctl fail");
        return false;
    }

    status_value = ioctl(_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bitsPerWord);
    if(status_value < 0)
    {
        perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
        return false;
    }

    status_value = ioctl(_fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bitsPerWord);
    if(status_value < 0)
    {
        perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
        return false;
    }

    status_value = ioctl(_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
    if(status_value < 0)
    {
        perror("Could not set SPI speed (WR)...ioctl fail");
        return false;
    }

    status_value = ioctl(_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
    if(status_value < 0)
    {
        perror("Could not set SPI speed (RD)...ioctl fail");
        return false;
    }
    return true;
}
