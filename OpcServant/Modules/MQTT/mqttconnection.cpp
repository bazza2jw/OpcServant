#include "mqttconnection.h"

/*!
 * \brief MRL::MqttConnection::MqttConnection
 */
MRL::MqttConnection::MqttConnection( const char *id) : mosqpp::mosquittopp(id)

{

}
/*!
 * \brief MRL::MqttConnection::~MqttConnection
 */
MRL::MqttConnection::~MqttConnection()
{

}
/*!
 * \brief MRL::MqttConnection::on_connect
 * \param rc
 */
void MRL::MqttConnection::on_connect(int rc)
{
    printf("Connected with code %d.\n", rc);
    _ok = rc== 0;
//        if(rc == 0){
            /* Only attempt to subscribe on a successful connect. */
            //subscribe(nullptr,_pubKey.c_str()); // subscribe to data input
//        }
}
/*!
 * \brief MRL::MqttConnection::on_message
 * \param message
 */
void MRL::MqttConnection::on_message(const struct mosquitto_message *message)
{
    // Message received
//        double temp_celsius, temp_farenheit;
//        char buf[51];

//        if(!strcmp(message->topic, "temperature/celsius")){
//            memset(buf, 0, 51*sizeof(char));
//            /* Copy N-1 bytes to ensure always 0 terminated. */
//            memcpy(buf, message->payload, 50*sizeof(char));
//            temp_celsius = atof(buf);
//            temp_farenheit = temp_celsius*9.0/5.0 + 32.0;
//            snprintf(buf, 50, "%f", temp_farenheit);
//            publish(NULL, "temperature/farenheit", strlen(buf), buf);
//        }
}
/*!
 * \brief MRL::MqttConnection::on_subscribe
 * \param mid
 * \param qos_count
 * \param granted_qos
 */
void MRL::MqttConnection::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
    printf("Subscription succeeded.\n");
}

