#include "modbusServer.h"
#include <string.h>

#ifdef WIN32
typedef int socklen_t;
#endif

// map of modbus servers
std::map<uint16_t,MRL::ModbusServer *> MRL::ModbusServer::_map;



/*!
 * \brief MRL::ModbusServer::initModbus
 * \param Host_Ip
 * \param port
 * \param debugging
 * \return
 */
bool MRL::ModbusServer::initModbus(bool debugging )
{
    ctx = modbus_new_tcp(m_host.c_str(), m_port);
    if (ctx)
    {
        throw std::runtime_error("There was an error allocating the modbus" );
    }
    modbus_set_debug(ctx, debugging);
    m_modbusSocket = modbus_tcp_listen(ctx, 1);

    mapping = modbus_mapping_new(m_numBits, m_numInputBits, m_numInputRegisters, m_numRegisters);
    if (mapping == NULL)
    {
        std::cerr << "Unable to assign mapping：" <<  modbus_strerror(errno) << std::endl;
        modbus_free(ctx);
        //
        m_initialized = false;
        throw std::runtime_error("There was an error allocating the mapping" );
        return false;
    }
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
    _map[m_port] = this;
}
/*!
 * \brief MRL::ModbusServer::~ModbusServer
 */
MRL::ModbusServer::~ModbusServer()
{
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
    _map.erase(m_port);
}

/*!
 * \brief MRL::ModbusServer::run
 */
void MRL::ModbusServer::run()
{
    if(!_running)
    {
        _running = true;
        std::thread loop([this]()
        {
            while (_running)
            {
                if (m_initialized)
                {
                    recieveMessages();
                }
                else
                {
                    m_initialized = true;
                }
            }
        });
        loop.detach();
    }
    return;
}
/*!
 * \brief MRL::ModbusServer::modbus_set_slave_id
 * \param id
 * \return
 */
bool MRL::ModbusServer::modbus_set_slave_id(int id)
{
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
    if (!mapping || (registerStartaddress > (m_numRegisters - 1)))
    {
        return false;
    }
    slavemutex.lock();
    mapping->tab_input_registers[registerStartaddress] = Value;
    slavemutex.unlock();
    return true;
}

/*!
 * \brief MRL::ModbusServer::setInputRegisterValue
 * \param registerStartaddress
 * \param Value
 * \return
 */
bool MRL::ModbusServer::setInputRegisterValue(int registerStartaddress, uint32_t Value)
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
    slavemutex.lock();
    mapping->tab_registers[registerStartaddress] = Value;
    slavemutex.unlock();
    return true;
}

bool MRL::ModbusServer::setHoldingRegisterValue(int registerStartaddress, uint32_t Value)
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
    slavemutex.lock();
    mapping->tab_input_bits[NumBit] = Value;
    slavemutex.unlock();
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
    slavemutex.lock();
    modbus_set_float(Value, &mapping->tab_registers[registerStartaddress]);
    slavemutex.unlock();
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
    if (!mapping || (registerStartaddress > (m_numRegisters - 2)))
    {
        return false;
    }
    slavemutex.lock();
    modbus_set_float(Value, &mapping->tab_input_registers[registerStartaddress]);
    slavemutex.unlock();
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
    return modbus_get_float_badc(&mapping->tab_registers[registerStartaddress]);
}

uint32_t MRL::ModbusServer::getHoldingRegisterLongValue(int registerStartaddress)
{
    if (!mapping || !m_initialized)
    {
        return 0;
    }
    uint32_t ret = (uint32_t(mapping->tab_registers[registerStartaddress]) << 16) | (uint32_t(mapping->tab_registers[registerStartaddress + 1]));
    return ret;
}


/*!
 * \brief MRL::ModbusServer::recieveMessages
 */
void MRL::ModbusServer::recieveMessages()
{
    uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
    int master_socket;
    int rc;
    fd_set refset;
    fd_set rdset;
    /* Maximum file descriptor number */
    int fdmax;
    /* Clear the reference set of socket */
    FD_ZERO(&refset);
    /* Add the server socket */
    FD_SET(m_modbusSocket, &refset);

    /* Keep track of the max file descriptor */
    fdmax = m_modbusSocket;

    while( true )
    {
        rdset = refset;
        if (select(fdmax+1, &rdset, NULL, NULL, NULL) == -1)
        {
            std::cerr << "Server select() failure." << std::endl;
            break;
        }

        /* Run through the existing connections looking for data to be
         * read */
        for (master_socket = 0; master_socket <= fdmax; master_socket++)
        {
            if (!FD_ISSET(master_socket, &rdset))
            {
                continue;
            }

            if (master_socket == m_modbusSocket)
            {
                /* A client is asking a new connection */
                socklen_t addrlen;
                struct sockaddr_in clientaddr;
                int newfd;

                /* Handle new connections */
                addrlen = sizeof(clientaddr);
                memset(&clientaddr, 0, sizeof(clientaddr));
                newfd = accept(m_modbusSocket, (struct sockaddr *)&clientaddr, &addrlen);
                if (newfd == -1)
                {
                    std::cerr << "Server accept() error" << std::endl;
                }
                else
                {
                    FD_SET(newfd, &refset);

                    if (newfd > fdmax)
                    {
                        /* Keep track of the maximum */
                        fdmax = newfd;
                    }
                    std::cerr << "New connection from " <<  inet_ntoa(clientaddr.sin_addr) << " on "  << clientaddr.sin_port << " fd " << newfd << std::endl;
                }
            }
            else if (ok())
            {
                modbus_set_socket(ctx, master_socket);
                rc = modbus_receive(ctx, query);
                if (rc > 0)
                {
                    modbus_reply(ctx, query, rc, mapping);
                }
                else if (rc == -1)
                {
                    /* This example server in ended on connection closing or
                     * any errors. */
                    std::cerr << "Connection closed on socket " <<  master_socket << std::endl;
#ifdef _WIN32
                    closesocket(master_socket);
#else
                    close(master_socket);
#endif
                    /* Remove from reference set */
                    FD_CLR(master_socket, &refset);

                    if (master_socket == fdmax)
                    {
                        fdmax--;
                    }
                }
            }
        }
    }
    m_initialized = false;
}
