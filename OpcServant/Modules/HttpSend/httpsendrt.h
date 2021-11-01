#ifndef HTTPSENDRT_H
#define HTTPSENDRT_H
#include <mosquittopp.h>
#include <Common/Daq/rtobject.h>
namespace MRL
{
    /*!
     * \brief The HttpSendRT class
     */
    class HttpSendRt : public RTObject {
        std::string _filter = "*"; // filter on source path
        std::string _url = "localhost"; // HttpSend target hostaddress

    public:
        /*!
         * \brief HttpSendRT
         * \param id
         */
        HttpSendRt(int id);
        /*!
            \brief start
        */
        virtual void start();
        /*!
            \brief stop
        */
        virtual void stop();
        /*!
            \brief publishQueueItem
            \param msg
            \return
        */
        void  publishQueueItem(const Message &msg);

        virtual bool useProcessTimer() {
            return true;   // used to drive state machines
        }
        /*!
            \brief process
        */
        virtual void process();
        /*!
            \brief onOneSecond
            \param t
        */
        virtual void onOneSecond(time_t t);
    };

}
#endif // HTTPSENDRT_H
