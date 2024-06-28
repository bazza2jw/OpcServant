#ifndef MODBUSSERVER_H
#define MODBUSSERVER_H


#include <iostream>
#include <thread>
#include <stdlib.h>
#include <iostream>
#include <mutex>
#include <string>
#include <memory>
#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <modbus.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "modbus.lib")
/*linux*/
#else
#include <modbus/modbus.h>
#include <unistd.h>
#include <error.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#endif


//#define  MAX_POINT  50000


namespace MRL
{
/*!
 * \brief The ModbusServer class
 */
class ModbusServer
{
public:
    ModbusServer(std::string host="0.0.0.0", uint16_t port=502,int numBits = 100, int numInputBits = 100, int numRegisters = 100, int numInputRegisters = 100);
    ~ModbusServer();

public:
    void recieveMessages();
    bool modbus_set_slave_id(int id);
    bool initModbus(bool debugging = false);

    uint8_t getTab_Input_Bits(int NumBit);
    bool setTab_Input_Bits(int NumBit, uint8_t Value);

    uint16_t getHoldingRegisterValue(int registerNumber);
    float getHoldingRegisterFloatValue(int registerStartaddress);

    bool setHoldingRegisterValue(int registerNumber, uint16_t Value);
    bool setHoldingRegisterValue(int registerNumber, float Value);

    bool setInputRegisterValue(int registerNumber, uint16_t Value);
    bool setInputRegisterValue(int registerNumber, float Value);

private:
    std::string m_host;
    uint16_t m_port = 5000; // not a privileged socket
    std::mutex slavemutex;
    int m_errCount{ 0 };
    int m_modbusSocket{ -1 };
    bool m_initialized{ false };
    modbus_t * ctx = nullptr;
    modbus_mapping_t *mapping = nullptr;
    /*Mapping*/
    int m_numBits {0 };
    int m_numInputBits {0};
    int m_numRegisters {0};
    int m_numInputRegisters {0};

public:
    void run();
    bool ok() { return ctx && mapping; }
};




}
#endif // MODBUSSERVER_H
