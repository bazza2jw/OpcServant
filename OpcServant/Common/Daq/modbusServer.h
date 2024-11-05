#ifndef MODBUSSERVER_H
#define MODBUSSERVER_H

#include <MrlLib/mrlmutex.h>
#include <iostream>
#include <thread>
#include <stdlib.h>
#include <iostream>
#include <mutex>
#include <string>
#include <memory>
#include <map>
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
#include <poll.h>
#endif


//#define  MAX_POINT  50000


namespace MRL
{
/*!
 * \brief The ModbusServer class
 */
class ModbusServer
{
    static std::map<uint16_t,ModbusServer *> _map;
    static MRL::ReadWriteMutex _mutex;


public:
    ModbusServer(std::string host="0.0.0.0", uint16_t port=502,int numBits = 100, int numInputBits = 100, int numRegisters = 100, int numInputRegisters = 100);
    ~ModbusServer();

public:
    void receiveMessages();
    bool modbus_set_slave_id(int id);
    bool initModbus(bool debugging = false);

    uint8_t getTab_Input_Bits(int NumBit);
    bool setTab_Input_Bits(int NumBit, uint8_t Value);

    uint16_t getHoldingRegisterValue(int registerNumber);
    float getHoldingRegisterFloatValue(int registerStartaddress);
    uint32_t getHoldingRegisterLongValue(int registerStartaddress);

    bool setHoldingRegisterValue(int registerNumber, uint16_t Value);
    bool setHoldingRegisterValue(int registerNumber, float Value);
    bool setHoldingRegisterValue(int registerNumber, uint32_t Value);

    bool setInputRegisterValue(int registerNumber, uint16_t Value);
    bool setInputRegisterValue(int registerNumber, float Value);
    bool setInputRegisterValue(int registerNumber, uint32_t Value);
    //
    static ModbusServer * find(uint16_t i) {
        return _map[i];
    }
    static void pollAll()
    {
        ReadLock l(_mutex);
        {
            for(auto const &i : _map)
            {
                ModbusServer *p;
                p = const_cast<ModbusServer *>(i.second);
                if(p ) p->run();
            }
        }
    }

    // close everything
    static void clear()
    {
        WriteLock l(_mutex);
        _map.clear();
    }
private:
    std::string m_host;
    uint16_t m_port = 5000; // not a privileged socket
    MRL::ReadWriteMutex slavemutex;
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
    bool _running = false;
    //
    // Used in the receive loop
    bool _debugging = false;
    std::set<int> _fdSet; // set of fds

public:
    void run();
    bool running() const {
        return _running;
    }
    bool ok() {
        return ctx && mapping;
    }
    void stop()
    {
        _running = false;
    }
    void start()
    {
        _running = true;
    }
};




}
#endif // MODBUSSERVER_H
