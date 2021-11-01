#ifndef ICPCON7065RT_H
#define ICPCON7065RT_H
#include "../icpcon_global.h"
#include "../icpcon.h"
#include <Common/Daq/rtobject.h>
#include <Common/Daq/serialpacket.h>
#include "icpcon7065rt.h"

namespace MRL {
    // runtime ICPCON7065 object
    constexpr int NUMBER_7065_CHAN= 5;
    constexpr int TIMEOUT_7065 = 10;

    //
    class ICPCON7065RT : public RTObject
    {

        PropertyPath _path; // where in the models the object data goes
        int _state = 0;
        SerialPacket _packet;
        //
        std::string _port = "/dev/ttyUSB0";
        int _baudRate = 56700;
        int _node = 1;
        int _errorWait = 10;
        wxStopWatch _timer;
        //
        // The pre-built packets
        std::string moduleList;
        //
        //
        bool _outputs[NUMBER_7065_CHAN];
        Open62541::NodeId _valueNodes[NUMBER_7065_CHAN];
        Open62541::NodeId _nodeStatus;
        bool _nodeOk = true;
        //
        bool _secondFlag = false;
        unsigned _lastOut = 0xFFFF;
        enum
        {
          STATE_IDLE = 0,
          STATE_ID, // identify the module
          STATE_INIT, // send the initialisation string
          STATE_SEND, // send the channel read request
          STATE_RECEIVE, // wait for response
          STATE_ERROR,
        };


    public:

        enum
        {
           SET_RELAY =  MESSAGEID::Last + 1
        };

        /*!
         * \brief ICPCON7065RT
         * \param id
         */
        ICPCON7065RT(int id) : RTObject(id)
        {

        }

        static StringVector _channelNames;
        /*!
         * \brief output
         * \param i
         * \return
         */
        bool output(size_t i) const
        {
            if(i < NUMBER_7065_CHAN)
            {
                return _outputs[i];
            }
            return false;
        }

        void setOutput(size_t i, bool v)
        {
            if(i < NUMBER_7065_CHAN)
            {
                _outputs[i] = v;
            }
        }
        /*!
         * \brief useProcessTimer
         * \return
         */
        virtual bool useProcessTimer() {
            return true;   // used to drive state machines
        }

        /*!
         * \brief makePackets
         */
        void makePackets()
        {
            moduleList = ICPCON::makePacket(_node,"$%02XM"); // get the module ID
        }
        /*!;
            \brief start
        */
        virtual void start();
        /*!
            \brief stop
        */
        virtual void stop();
        /*!
            \brief publish
        */
        virtual void publish();
        /*!
            \brief measure
        */
        virtual void measure();

        /*!
         * \brief process
         */
        virtual void process();
        /*!
         * \brief onOneSecond
         * \param t
         */
        virtual void onOneSecond(time_t t);

        /*!
         * \brief updateOpc
         * \param server
         * \param objectNode
         */
        virtual void updateOpc(OpcServer &server, Open62541::NodeId &objectNode);
        /*!
         * \brief initialiseOpc
         * \param server
         * \param objectNode
         */
        virtual void initialiseOpc(OpcServer &server, Open62541::NodeId &objectNode);
        /*!
         * \brief publishHtmlPage
         */
        virtual void publishHtmlPage(CTML::Node &s); // publish the current runtime status as HTML text

        /*!
         * \brief measureHtmlPage
         */
        virtual void measureHtmlPage(CTML::Node &s);
        /*!
            \brief processQueueItem
        */
        virtual bool processQueueItem(const Message &);

        //
        /*!
         * \brief hasOutputs
         * \return true if has outputs
         */
        virtual bool hasOutputs() const { return true;}
        /*!
         * \brief outputs
         * \return
         */
        virtual StringVector &outputs() { return _channelNames;}

       /*!
        * \brief setFault
        * \param s
        */
       void setFault(const std::string &s)
       {
           runtime().setValue("FaultReason",s);
           runtime().setValue("Fault",true);
           _errorWait = TIMEOUT_7065 * 1000;
           _state = STATE_ERROR;
           _timer.Start();
           _nodeOk = false;
           _lastOut = 0xFFFF;
       }

       /*!
        * \brief clearFault
        * \param s
        */
       void clearFault()
       {
           runtime().setValue("FaultReason",std::string("OK"));
           runtime().setValue("Fault",false);
           _state = STATE_ID;
       }


    };
}
#endif // ICPCON7065RT_H
