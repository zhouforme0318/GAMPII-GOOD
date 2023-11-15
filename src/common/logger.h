/*------------------------------------------------------------------------------
* logger.h : logger printing
*
* Copyright (C) 2020-2099 by Ubiquitous Navigation & Integrated positioning lab in Quest (UNIQ), all rights reserved.
*    This file is part of GAMP II - GOOD (Gnss Observations and prOducts Downloader)
*
* history : 2020/08/16 1.0  new (by Feng Zhou)
*           2020/10/12 3.0  mojor modifications (by Feng Zhou)
*-----------------------------------------------------------------------------*/
#ifndef LOGGER_H
#define LOGGER_H

#include "common.h"
#include "types.h"
#include "gtime.h"

static int ilevel_ = 5;        /* level of debug trace */
static std::ofstream sfile_;   /* iostream of output file */

class Logger
{
public:
    /**
    * @brief   : TraceLevel -
    * @param[I]: level ()
    * @param[O]: none
    * @return  : none
    * @note    :
    **/
    static void TraceLevel(int level)
    {
        ilevel_ = level;
    } /* end of TraceLevel */

    /**
    * @brief   : TraceOpen -
    * @param[I]: filename ()
    * @param[O]: none
    * @return  : none
    * @note    :
    **/
    static bool TraceOpen(std::string filename)
    {
        sfile_.open(filename, std::ios::out);
        if (!sfile_.is_open()) return false;
        return true;
    } /* end of TraceOpen */

    /**
    * @brief   : TraceClose -
    * @param[I]: none
    * @param[O]: none
    * @return  : none
    * @note    :
    **/
    static void TraceClose()
    {
        if (!sfile_.is_open()) sfile_.close();
    } /* end of TraceClose */

    /**
    * @brief   : Trace -
    * @param[I]: level ()
    * @param[I]: info ()
    * @param[O]: none
    * @return  : none
    * @note    :
    **/
    static void Trace(int level, std::string info)
    {
        if (level > ilevel_) return;
        switch (level)
        {
            case 1 : std::cout << TEXT_RED << info << std::endl << TEXT_RESET; break;
            case 2 : std::cout << TEXT_GREEN << info << std::endl << TEXT_RESET; break;
            case 3 : std::cout << TEXT_YELLOW << info << std::endl << TEXT_RESET; break;
            case 4 : std::cout << TEXT_CYAN << info << std::endl << TEXT_RESET; break;
            case 5 : std::cout << TEXT_PURPLE << info << std::endl << TEXT_RESET; break;
            default: break;
        }
        if (sfile_.is_open()) sfile_ << info << std::endl;
    } /* end of Trace */

    /**
    * @brief   : Trace -
    * @param[I]: level ()
    * @param[I]: info ()
    * @param[I]: isnow ()
    * @param[O]: none
    * @return  : none
    * @note    :
    **/
    static void Trace(int level, std::string info, bool isnow)
    {
        if (level > ilevel_) return;
        if (!isnow) Trace(level, info);
        std::time_t tnow = std::time(0);  /* time of program now moment */
        char stime[256];  /* string of time for output  */
        std::strftime(stime, sizeof(stime), "%Y-%m-%d %H:%M:%S: ", std::localtime(&tnow));
        switch (level)
        {
            case 1 : std::cout << TEXT_RED << stime << info << std::endl << TEXT_RESET; break;
            case 2 : std::cout << TEXT_GREEN << stime << info << std::endl << TEXT_RESET; break;
            case 3 : std::cout << TEXT_YELLOW << stime << info << std::endl << TEXT_RESET; break;
            case 4 : std::cout << TEXT_CYAN << stime << info << std::endl << TEXT_RESET; break;
            case 5 : std::cout << TEXT_PURPLE << stime << info << std::endl << TEXT_RESET; break;
            default: break;
        }
        if (sfile_.is_open()) sfile_ << info << std::endl;
    } /* end of Trace */
};

#endif  // LOGGER_H