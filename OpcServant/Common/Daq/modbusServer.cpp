#include "modbusServer.h"
#include <string.h>

#ifdef WIN32
typedef int socklen_t;
#endif

// map of modbus servers
std::map<uint16_t,MRL::ModbusServer *> MRL::ModbusServer::_map;
MRL::ReadWriteMutex MRL::ModbusServer::_mutex;



/*!
 * \brief MRL::ModbusServer::initModbus
 * \param Host_Ip
 * \param port
 * \param debugging
 * \return
 */
bool MRL::ModbusServer::initModbus(bool debugging )
{
    WriteLock l(slavemutex);
    m_initialized = false;
    _debugging = debugging;
    ctx = modbus_new_tcp(m_host.c_str(), m_port);
    if (!ctx)
    {
        throw std::runtime_error("There was an error allocating the modbus" );
    }
    modbus_set_debug(ctx, debugging);
    m_modbusSocket = modbus_tcp_listen(ctx, 1);
    if(m_modbusSocket < 1)
    {
        throw std::runtime_error("Failed to create listening socket" );
    }
    _fdSet.clear();

    mapping = modbus_mapping_new(m_numBits, m_numInputBits,m_numRegisters, m_numInputRegisters );
    if (mapping == NULL)
    {
        modbus_free(ctx);
        //
        throw std::runtime_error("There was an error allocating the mapping" );
    }
    //
    //
    m_initialized = true;
    return true;
}
/*!
 * \brief MRL::ModbusServer::ModbusServer
 * \param host
 * \param port
 */
MRL::ModbusServer::ModbusServer(std::string host, uint16_t port, int numBits, int numInputBits, int numRegisters, int numInputRegisters) :
    m_host(host),
    m_port( port),
    m_numBits(numBits),
    m_numInputBits(numInputBits),
    m_numRegisters(numRegisters),
    m_numInputRegisters(numInputRegisters)
{
    {
        WriteLock l(_mutex);
        _map[m_port] = this;
    }
}
/*!
 * \brief MRL::ModbusServer::~ModbusServer
 */
MRL::ModbusServer::~ModbusServer()
{
    WriteLock l(slavemutex);
    if(mapping)
    {
        modbus_mapping_free(mapping);
        mapping = nullptr;
    }
    if(ctx)
    {
        modbus_close(ctx);
        modbus_free(ctx);
        ctx = nullptr;
    }
    {
        WriteLock l(_mutex);
        _map.erase(m_port);
    }
}

/*!
 * \brief MRL::ModbusServer::run
 *  The loop - wrap with a thread for a
 */
void MRL::ModbusServer::run()
{
    if (m_initialized)
    {
        receiveMessages();
    }
}
/*!
 * \brief MRL::ModbusServer::modbus_set_slave_id
 * \param id
 * \return
 */
bool MRL::ModbusServer::modbus_set_slave_id(int id)
{
    WriteLock l(slavemutex);
    int rc = modbus_set_slave(ctx, id);
    if (rc == -1)
    {
        throw std::runtime_error("Invalid slave id");
    }
    return true;
}

/*!
 * \brief MRL::ModbusServer::setInputRegisterValue
 * \param registerStartaddress
 * \param Value
 * \return
 */
bool MRL::ModbusServer::setInputRegisterValue(int registerStartaddress, uint16_t Value)
{
    WriteLock l(slavemutex);
    if (!mapping || (registerStartaddress > (m_numInputRegisters - 1)))
    {
        return false;
    }
    mapping->tab_input_registers[registerStartaddress] = Value;
    return true;
}

/*!
 * \brief MRL::ModbusServer::setInputRegisterValue
 * \param registerStartaddress
 * \param Value
 * \return
 */
bool MRL::ModbusServer::setInputRegisterLongValue(int registerStartaddress, uint32_t Value)
{
    return setInputRegisterValue(registerStartaddress++, uint16_t(Value >> 16)) &&
           setInputRegisterValue(registerStartaddress, uint16_t(Value & 0xFFFF));
}
/*!
 * \brief MRL::ModbusServer::setHoldingRegisterValue
 * \param registerStartaddress
 * \param Value
 * \return
 */
bool MRL::ModbusServer::setHoldingRegisterValue(int registerStartaddress, uint16_t Value)
{
    if (!mapping ||  (registerStartaddress > (m_numRegisters - 1)))
    {
        return false;
    }
    WriteLock l(slavemutex);
    mapping->tab_registers[registerStartaddress] = Value;
    return true;
}

bool MRL::ModbusServer::setHoldingRegisterLongValue(int registerStartaddress, uint32_t Value)
{
    return setHoldingRegisterValue(registerStartaddress++, uint16_t(Value >> 16)) &&  setHoldingRegisterValue(registerStartaddress, uint16_t(Value & 0xFFFF));
}


/*!
 * \brief MRL::ModbusServer::getHoldingRegisterValue
 * \param registerStartaddress
 * \return
 */
uint16_t MRL::ModbusServer::getHoldingRegisterValue(int registerStartaddress)
{
    if (!mapping || (!m_initialized))
    {
        return -1;
    }
    ReadLock l(slavemutex);
    return mapping->tab_registers[registerStartaddress];
}
/*!
 * \brief MRL::ModbusServer::setTab_Input_Bits
 * \param NumBit
 * \param Value
 * \return
 */
bool MRL::ModbusServer::setTab_Input_Bits(int NumBit, uint8_t Value)
{
    if (!mapping ||  (NumBit > (m_numInputBits - 1)))
    {
        return false;
    }
    WriteLock l(slavemutex);
    mapping->tab_input_bits[NumBit] = Value;
    return true;
}
/*!
 * \brief MRL::ModbusServer::getTab_Input_Bits
 * \param NumBit
 * \return
 */
uint8_t MRL::ModbusServer::getTab_Input_Bits(int NumBit)
{
    if (!mapping || (!m_initialized))
    {
        return -1;
    }
    ReadLock l(slavemutex);
    return mapping->tab_input_bits[NumBit];
}
/*!
 * \brief MRL::ModbusServer::setHoldingRegisterValue
 * \param registerStartaddress
 * \param Value
 * \return
 */
bool MRL::ModbusServer::setHoldingRegisterValue(int registerStartaddress, float Value)
{
    if (!mapping || (registerStartaddress > (m_numRegisters - 2)))
    {
        return false;
    }
    WriteLock l(slavemutex);
    modbus_set_float(Value, &mapping->tab_registers[registerStartaddress]);
    return true;
}

/*!
 * \brief MRL::ModbusServer::setInputRegisterValue
 * \param registerStartaddress
 * \param Value
 * \return
 */
bool MRL::ModbusServer::setInputRegisterValue(int registerStartaddress, float Value)
{
    if (!mapping || (registerStartaddress > (m_numInputRegisters - 2)))
    {
        return false;
    }
    WriteLock l(slavemutex);
    modbus_set_float(Value, &mapping->tab_input_registers[registerStartaddress]);
    return true;
}


/*!
 * \brief MRL::ModbusServer::getHoldingRegisterFloatValue
 * \param registerStartaddress
 * \return
 */
float MRL::ModbusServer::getHoldingRegisterFloatValue(int registerStartaddress)
{
    if (!mapping || !m_initialized)
    {
        return -1.0f;
    }
    ReadLock l(slavemutex);
    return modbus_get_float_badc(&mapping->tab_registers[registerStartaddress]);
}

uint32_t MRL::ModbusServer::getHoldingRegisterLongValue(int registerStartaddress)
{
    if (!mapping || !m_initialized)
    {
        return 0;
    }
    ReadLock l(slavemutex);
    uint32_t ret = (uint32_t(mapping->tab_registers[registerStartaddress]) << 16) | (uint32_t(mapping->tab_registers[registerStartaddress + 1]));
    return ret;
}


/*!
 * \brief MRL::ModbusServer::recieveMessages
 */
void MRL::ModbusServer::receiveMessages()
{
    WriteLock l(slavemutex);
    if(m_initialized)
    {
        // handle pending accepts
        struct pollfd pfd = {m_modbusSocket, POLLIN,0};
        if(poll(&pfd,1,0) > 0)
        {
            // pending accept
            socklen_t addrlen;
            struct sockaddr_in clientaddr;
            int newfd = 0;

            /* Handle new connections */
            addrlen = sizeof(clientaddr);
            memset(&clientaddr, 0, sizeof(clientaddr));
            newfd = accept(m_modbusSocket, (struct sockaddr *)&clientaddr, &addrlen);
            if (newfd > 0)
            {
                if(_debugging) std::cerr << __FUNCTION__ << " New Connection " << newfd << std::endl;
                _fdSet.insert(newfd);
            }
        }

        // handle requests
        if(_fdSet.size() > 0)
        {
            for(const int &i : _fdSet)
            {
                struct pollfd dfd = {i,POLLIN,0};
                if(poll(&dfd,1,0) > 0)
                {
                    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
                    modbus_set_socket(ctx, i);
                    int rc = modbus_receive(ctx, query);
                    if (rc > 0)
                    {

                        modbus_reply(ctx, query, rc, mapping);
                    }
                    else if (rc == -1)
                    {
                        if(_debugging) std::cerr << __FUNCTION__ << " Connection closed on socket " <<  i << std::endl;
                        close(i);
                        _fdSet.erase(i);
                        break;
                    }
                }
            }
        }
    }
}
