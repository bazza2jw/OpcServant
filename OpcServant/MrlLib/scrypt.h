
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
/*
 * C based encryption routines - these are intended to work on low end micro controllers (dsPic)
 * so have to be
 * */
#ifndef SCRYPT_H
#define SCRYPT_H
#include <string>

/*!
 * \brief hashText
 * Calculates the SHA256 for the string in text form
 * \param b string to hash
 * \return hash
 */
std::string  hashText(const std::string &b);
/*!
 * \brief fast_crc32
 * \param sum seed value
 * \param p   buffer to checksum
 * \param len length of buffer
 * returns CRC
 */
unsigned fast_crc32(unsigned sum, unsigned char *p, int len);
#endif
