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
#ifndef VALIDATORS_H
#define VALIDATORS_H
#include "wxmrlcommon.h"
//
// Some validator helpers
//

namespace MRL {
/*!
     * \brief The Validators struct
     */
    struct Validators {

        // Strings for validators
        static wxString *validAlphaNumericCharacters();  // for names
        static wxString *validIPCharacters();  // IP address charcaters
        static wxString *validNumberCharacters();  // characters for numbers
        static wxString *validHexCharacters();  // characters for numbers
        //
        // Text Validators
        static wxTextValidator &AlphaNumeric();
        static wxTextValidator &IP();
        static wxTextValidator &Numbers();
        static wxTextValidator &Hex();

        //
    };
}

#endif // VALIDATORS_H
