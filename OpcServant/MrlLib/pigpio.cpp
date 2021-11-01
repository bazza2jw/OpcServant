#include "pigpio.h"
/*!
 * \brief MRL::PiGpio::PiGpio
 * \param host
 * \param port
 */

MRL::PiGpio::PiGpio(const std::string &host, int port) : _host(host)
{
    std::string p = std::to_string(port);
    _handle = ::pigpio_start(host.c_str(),p.c_str());
}

/*!
 * \brief MRL::PiGpio::~PiGpio
 */
MRL::PiGpio::~PiGpio()
{
    if(_handle > 0) ::pigpio_stop(_handle);
}



