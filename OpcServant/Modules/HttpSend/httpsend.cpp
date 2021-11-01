#include "httpsend.h"
#include <mosquittopp.h>
#include "httpsendobjectmanager.h"
#include "httpsendrt.h"
/*!
 * \brief _instance
 */
static MRL::HttpSend _instance; // self register
/*!
 * \brief MRL::HttpSend::HttpSend
 */
MRL::HttpSend::HttpSend() : Plugin("HttpSend")
{
}
/*!
 * \brief MRL::HttpSend::initialise
 */
void MRL::HttpSend::initialise()
{
    mosqpp::lib_init();
    (void) new HttpSendObjectManager(); // HttpSend publisher
    //
    // Add the HttpSend command processors
    //
}

/*!
 * \brief MRL::HttpSend::terminate
 */
void MRL::HttpSend::terminate()
{
    mosqpp::lib_cleanup();
}
