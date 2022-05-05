/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#ifndef STATES_H
#define STATES_H
#include "mrllib_global.h"
#include <map>
#include <string>
//
namespace MRL
{
class StatisticsThresholdSet;
}
//
namespace STATES {
    // these are the default states
    constexpr const char *STATE_NONE = "None";
    constexpr const char *STATE_WAITING = "Waiting";
    constexpr const char *STATE_OK = "Ok";
    constexpr const char *STATE_DISABLED = "Disabled";
    constexpr const char *STATE_ENABLED = "Enabled";
    constexpr const char *STATE_FAULT = "Fault";
    constexpr const char *STATE_STARTED = "Started";
    constexpr const char *STATE_STOPPED = "Stopped";
    constexpr const char *STATE_TRIGGERED = "Triggered";
    //
    constexpr const char *STATE_ON = "On";
    constexpr const char *STATE_OFF = "Off";
    //
    constexpr const char *STATE_ACTIVE = "Active";
    constexpr const char *STATE_INACTIVE = "Inactive";
    //
    // message levels - these are also alarm levels plus the above
    constexpr const char *STATE_TRACE = "TraceLevel";
    constexpr const char *STATE_DEBUG = "DebugLevel";
    constexpr const char *STATE_INFO = "InfoLevel";
    constexpr const char *STATE_WARNING = "WarningLevel";
    constexpr const char *STATE_ERROR = "ErrorLevel";
    constexpr const char *STATE_CRITICAL = "CriticalLevel";
    constexpr const char *STATE_FATAL = "FatalLevel";
    constexpr const char *STATE_WRITE = "WriteLevel";
    //
    // the standard alarm threshold types
    //
    constexpr const char *STATE_HIHI = "HiHi";
    constexpr const char *STATE_HILO = "HiLo";
    constexpr const char *STATE_LOHI = "LoHi";
    constexpr const char *STATE_LOLO = "LoLo";
    //
    constexpr const char *STATE_ACTION = "Action";
    constexpr const char *STATE_ALERT = "Alert";
    //
    constexpr const char *STATE_QUALITY_GOOD = "Good";
    constexpr const char *STATE_QUALITY_BAD = "Bad";


/*!
 * \brief The States class
 */
class  MRLLIBSHARED_EXPORT States {

            std::string name;//!< state name

            std::string iconName;//< the name of the icon used
            // the level
            unsigned level = 0; //!< priority level
            std::string displayName;//!< name to show as - this can be translated
            // colours
            wxColour  foreground; //!< foreground colour
            wxColour background; //!< background colour

        public:

            // Alarm level - item state

            /*!
                \brief enum Level
            */
            typedef enum {
                StateWaiting = 0,
                StateDisabled,
                StateEnabled,
                StateStarted,
                StateStopped,
                StateOn,
                StateOff
            } ActiveLevel;

            // Action / Event states
            typedef enum {
                StateNone = 0,
                StateTriggered,
                StateChanged,
                StateFault,
                // Alarm threshold states
                StateOk,
                StateLoLo,
                StateLoHi,
                StateHiLo,
                StateHiHi,
                //
                // Alarm type
                StateAction = 0x20,
                StateAlert = 0x60
            } EventAlarmLevel;

            // MessageStates
            typedef enum
            {
                Trace = 0,
                Debug,
                Info,
                Warning,
                Error,
                Critical,
                Fatal
            } MessageLevel;

            // Data quality states
            typedef enum {
                QualityGood = 0,
                QualityBad
            } QualityLevel;

            /*!
                \brief States
                Construct
                \param n
                \param in
                \param l
                \param d
                \param f
                \param b
            */
            States(std::string n = "", std::string in = std::string(), unsigned  l = 0, std::string d = std::string(),
                   wxColour f = *wxBLACK, wxColour b = *wxWHITE)
                : name(n), iconName(in), level(l), displayName(d), foreground(f), background(b) {

            }
            /**
                @brief

                @param a
            */
            States(const States &a)
                : name(a.name), iconName(a.iconName), level(a.level), displayName(a.displayName),
                  foreground(a.foreground), background(a.background)
            {}

            // accessors
            /**
                @brief

                @return QIcon
            */
            const wxBitmap &getIcon();

            /**
                @brief

                @return QString
            */
            const std::string &getIconName() const {
                return iconName;
            }
            /**
                @brief

                @return QString
            */
            const std::string &getName() const {
                return name;
            }
            /**
                @brief

                @return unsigned
            */
            unsigned getLevel() const {
                return level;
            }
            /**
                @brief

                @return QString
            */
            std::string getDisplayName() {
                return displayName;
            }
            /**
                @brief

                @return QColor
            */
            const wxColour &getForeground() const {
                return foreground;
            }
            /**
                @brief

                @return QColor
            */
            const wxColour &getBackground() const {
                return background;
            }
            //

            /**
                @brief Set the icon file name and load it from the cache

                @param s
            */
            void setIconName(const std::string &s) {
                iconName = s;
                // icon = getPixmapFromCache(s);
            }

            /**
                @brief

                @param i
            */
            void setLevel(unsigned i) {
                level = i;
            }
            /**
                @brief

                @param i
            */
            void setDisplayName(const std::string &i) {
                displayName = i;
            }
            /**
                @brief

                @param i
            */
            void setForeground(wxColour i) {
                foreground = i;
            }
            /**
                @brief

                @param i
            */
            void setBackground(wxColour i) {
                background = i;
            }
            //
            /**
                @brief

            */
            typedef std::map<std::string, States> MAP;
            //
            // the state map
            //
            static MAP _states; /**< TODO */
            /**
                @brief

                @return MAP
            */
            static MAP &getStates() {
                return _states;
            }
            /**
                @brief

            */
            static void initialiseStates();
            /**
                @brief

                @param s
                @return bool
            */
            static bool loadStates(const std::string &s);
            /**
                @brief

                @param s
                @return bool
            */
            static bool saveStates(const std::string &s);
            /**
                @brief

                @param st
                @return String
            */
            static std::string  StatisticStateToState(MRL::StatisticsThresholdSet &st);
            /*!
             * \brief toName
             * \param state index
             * \return name of state
             */
            static std::string toName(unsigned state);
    };

}

// H
#endif

