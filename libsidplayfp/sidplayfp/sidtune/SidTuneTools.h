/*
 * This file is part of libsidplayfp, a SID player engine.
 *
 * Copyright (C) Michael Schwendt <mschwendt@yahoo.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef TOOLS_H
#define TOOLS_H

#include "SidTuneCfg.h"

#include <string.h>

#if defined(HAVE_SSTREAM)
#   include <sstream>
#else
#   include <strstream>
#   undef  istringstream
#   define istringstream istrstream
#endif

#if defined(HAVE_STRINGS_H)
#   include <strings.h>
#endif

#if defined(HAVE_STRCASECMP)
#   define MYSTRICMP strcasecmp
#else
#   define MYSTRICMP stricmp
#endif

#if defined(HAVE_STRNCASECMP)
#   define MYSTRNICMP strncasecmp
#else
#   define MYSTRNICMP strnicmp
#endif

class SidTuneTools
{
 public:
    /** Return pointer to file name position in complete path. */
    static size_t fileNameWithoutPath(const char* s);

    /**
    * Return pointer to file name position in complete path.
    * Special version: file separator = forward slash.
    */
    static size_t slashedFileNameWithoutPath(const char* s);

    /**
    * Return pointer to file name extension in path.
    * Searching backwards until first dot is found.
    */
    static char* fileExtOfPath(char* s);
};

#endif  /* TOOLS_H */
