/*
 * stringTools.cpp
 *
 *  Created on: 2015-7-11
 *      Author: waroy
 */

#include "../../header/Tools/stringTools.h"
#include <stdarg.h>
#include <stdio.h>
//stringTools::stringTools() {
//
//}
//
//stringTools::~stringTools() {
//}


std::string format(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    const size_t SIZE = 512;
    char buffer[SIZE] = { 0 };
    vsnprintf(buffer, SIZE, fmt, ap);

    va_end(ap);

    return std::string(buffer);
}
