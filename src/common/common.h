/*------------------------------------------------------------------------------
* common.h : constants definition
*
* Copyright (C) 2020-2099 by Ubiquitous Navigation & Integrated positioning lab in Quest (UNIQ), all rights reserved.
*    This file is part of GAMP II - GOOD (Gnss Observations and prOducts Downloader)
*
* history : 2020/08/16 1.0  new (by Feng Zhou)
*           2020/10/12 3.0  mojor modifications (by Feng Zhou)
*-----------------------------------------------------------------------------*/
#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#ifdef _WIN32  /* for Windows */
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <direct.h>
#include "..\win\unistd.h"
#pragma warning(disable:4996)
#pragma warning(disable:4101)
#else          /* for Linux or Mac */
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#endif

/* Trace debug export level --------------------------------------------------*/
enum LogLevel
{
    TERROR = 1,
    TINFO,
    TWARNING,
    TDEBUG,
    TEXPORT,
};

/* constant definitions ----------------------------------------------------------*/
#define TEXT_RED        "\033[31m"     /* RED                            */
#define TEXT_GREEN      "\033[32m"     /* GREEN                          */
#define TEXT_YELLOW     "\033[33m"     /* YELLOW                         */
#define TEXT_BLUE       "\033[34m"     /* BLUE                           */
#define TEXT_PURPLE     "\033[35m"     /* PURPLE                         */
#define TEXT_CYAN       "\033[36m"     /* CYAN                           */
#define TEXT_RESET      "\033[0m"

#define MAXCHARS        1024           /* maximum characters in one line */
#define MAXSTRPATH      1024           /* max length of stream path      */

#ifdef _WIN32  /* for Windows */
#define FILEPATHSEP '\\'
#define strcasecmp  _stricmp
#define access      _access
#else          /* for Linux or Mac */
#define FILEPATHSEP '/'
#endif

#endif  // COMMON_H