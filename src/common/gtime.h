/*------------------------------------------------------------------------------
* gtime.h : time conversions
*
* Copyright (C) 2020-2099 by Ubiquitous Navigation & Integrated positioning lab in Quest (UNIQ), all rights reserved.
*    This file is part of GAMP II - GOOD (Gnss Observations and prOducts Downloader)
*
* history : 2020/08/16 1.0  new (by Feng Zhou)
*           2020/10/12 3.0  mojor modifications (by Feng Zhou)
*-----------------------------------------------------------------------------*/
#ifndef GTIME_H
#define GTIME_H

#include "types.h"

class GTime
{
public:
    /**
    * @brief   : yy2yyyy - the conversion from 2-digit year to 4-digit year
    * @param[I]: yy (2-digit year)
    * @param[O]: none
    * @return  : 4-digit year
    * @note    :
    **/
    static int yy2yyyy(int yy)
    {
        int yyyy = yy;
        if (yyyy <= 50) yyyy += 2000;
        else if (yy > 50 && yy < 1900) yyyy += 1900;

        return yyyy;
    } /* end of yy2yyyy */

    /**
    * @brief   : yyyy2yy - the conversion from 4-digit year to 2-digit year
    * @param[I]: yyyy (4-digit year)
    * @param[O]: none
    * @return  : 2-digit year
    * @note    :
    **/
    static int yyyy2yy(int yyyy)
    {
        int yy = yyyy;
        if (yy >= 2000) yy -= 2000;
        else if (yy >= 1900 && yy < 2000) yy -= 1900;

        return yy;
    } /* end of yyyy2yy */

    /**
    * @brief   : TimeAdd - to increase time t0 by dt (in seconds), return t1 with t1(1) < 604800.0
    * @param[I]: t0 (time struct (.mjd and .sod))
    * @param[I]: dt (increased seconds)
    * @param[O]: none
    * @return  : time struct (.mjd and .sod)
    * @note    :
    **/
    static gtime_t TimeAdd(gtime_t t0, double dt)
    {
        gtime_t t1 = t0;
        t1.sod = t0.sod + dt;

        while (true)
        {
            if (t1.sod >= 86400.0)
            {
                t1.sod -= 86400.0;
                t1.mjd += 1;
            }
            else if (t1.sod < 0.0)
            {
                t1.sod += 86400.0;
                t1.mjd -= 1;
            }
            else break;
        }

        return t1;
    } /* end of TimeAdd */

    /**
    * @brief   : TimeDiff - time difference (t1-t0), return in seconds
    * @param[I]: t1, t0 (time struct (.mjd and .sod))
    * @param[O]: none
    * @return  : time difference in seconds
    * @note    : the first time subtracts the second time
    **/
    static double TimeDiff(gtime_t t1, gtime_t t0)
    {
        return (t1.mjd - t0.mjd) * 86400.0 + (t1.sod - t0.sod);
    } /* end of TimeDiff */

    /**
    * @brief   : hms2sod - the conversion from hour, minute, and second to seconds of day
    * @param[I]: hh (hour)
    * @param[I]: minu (minute)
    * @param[I]: sec (second)
    * @param[O]: none
    * @return  : seconds of day
    * @note    :
    **/
    static double hms2sod(int hh, int minu, double sec)
    {
        return hh * 3600.0 + minu * 60.0 + sec;
    } /* end of hms2sod */

    /**
    * @brief   : ymdhms2time - the conversion from year, month, day, hour, minute, second to time
    * @param[I]: ep (6 x 1 vector indicating year, month, day, hour, minute, and second)
    * @param[O]: none
    * @return  : time struct (.mjd and .sod)
    * @note    :
    **/
    static gtime_t ymdhms2time(const std::vector<double> ep)
    {
        gtime_t tt = {0};
        tt.sod = hms2sod((int)floor(ep[3]), (int)floor(ep[4]), ep[5]);

        int yyyy = yy2yyyy((int)floor(ep[0]));
        int month = (int)floor(ep[1]);
        int day = (int)floor(ep[2]);
        if (month <= 2)
        {
            yyyy -= 1;
            month += 12;
        }

        double jd = (int)floor(365.25 * yyyy + 1.0e-9) + (int)floor(30.6001 * (month + 1) + \
            1.0e-9) + day + 1720981.5;
        tt.mjd = (int)floor(jd - 2400000.5);

        return tt;
    } /* end of ymdhms2time */

    /**
    * @brief   : time2ymdhms - the conversion from time to year, month, day, hour, minute, second
    * @param[I]: tt (time struct (.mjd and .sod))
    * @param[O]: none
    * @return  : 6 x 1 vector indicating year, month, day, hour, minute, and second
    * @note    :
    **/
    static std::vector<double> time2ymdhms(gtime_t tt)
    {
        double mjd = tt.mjd + tt.sod / 86400.0;

        int a = (int)floor(mjd + 1.0 + 1.0e-9) + 2400000;
        double frac = mjd + 0.5 + 2400000.5 - a;
        int b = a + 1537;
        int c = (int)floor((b - 122.1) / 365.25 + 1.0e-9);
        int d = (int)floor(365.25 * c + 1.0e-9);
        int e = (int)floor((b - d) / 30.6001 + 1.0e-9);
        int day = b - d - (int)floor(30.6001 * e);
        int month = e - 1 - 12 * (int)floor(e / 14.0 + 1.0e-9);
        int year = c - 4715 - (int)floor((7 + month) / 10.0 + 1.0e-9);

        double thh = frac * 24.0;
        int hour = (int)floor(thh + 1.0e-9);
        double tmin = (thh - hour) * 60.0;
        int min = (int)floor(tmin + 1.0e-9);
        double sec = (tmin - min) * 60.0;
        if (sec > 59.999)
        {
            sec = 0.0;
            min += 1;
        }
        if (min >= 60)
        {
            min -= 60;
            hour += 1;
        }

        std::vector<double> ep;
        ep.push_back(year); ep.push_back(month); ep.push_back(day);
        ep.push_back(hour); ep.push_back(min); ep.push_back(sec);

        return ep;
    } /* end of time2ymdhms */

    /**
    * @brief   : yrdoy2time - the conversion from year, day of year to time
    * @param[I]: year (4- or 2-digit year)
    * @param[I]: doy (day of year)
    * @param[O]: none
    * @return  : time struct (.mjd and .sod)
    * @note    :
    **/
    static gtime_t yrdoy2time(int year, int doy)
    {
        /* time at yyyy-01-01 00:00:00 */
        std::vector<double> ep;
        ep.push_back(year); ep.push_back(1); ep.push_back(1);
        ep.push_back(0); ep.push_back(0); ep.push_back(0);
        gtime_t t_jan1 = ymdhms2time(ep);
        double dt = (doy - 1) * 86400.0;
        gtime_t tt = TimeAdd(t_jan1, dt);

        return tt;
    } /* end of yrdoy2time */

    /**
    * @brief   : time2yrdoy - the conversion from time to year, day of year
    * @param[I]: tt (time struct (.mjd and .sod))
    * @param[O]: year (4-digit year, nullptr:NO output)
    * @param[O]: doy (day of year, nullptr:NO output)
    * @return  : none
    * @note    :
    **/
    static void time2yrdoy(gtime_t tt, int& year, int& doy)
    {
        std::vector<double> ep;
        ep = time2ymdhms(tt);
        year = (int)ep[0];

        /* time at yyyy-01-01 00:00:00 */
        ep[1] = 1; ep[2] = 1; ep[3] = 0; ep[4] = 0; ep[5] = 0;
        gtime_t t_jan1 = ymdhms2time(ep);

        double dt = TimeDiff(tt, t_jan1);
        doy = (int)(dt / 86400.0 + 1.0e-9) + 1;
    } /* end of time2yrdoy */

    /**
    * @brief   : gpst2time - the conversion from GPS week, seconds of week to time
    * @param[I]: week (GPS week)
    * @param[I]: sow (seconds of week)
    * @param[O]: none
    * @return  : time struct (.mjd and .sod)
    * @note    :
    **/
    static gtime_t gpst2time(int week, double sow)
    {
        /* compute the time of the GPS starting time (1980-01-06 00:00:00) */
        const std::vector<double> gpst0 = { 1980, 1, 6, 0, 0, 0 };  /* GPS time reference */
        gtime_t t_gpst0 = ymdhms2time(gpst0);

        double dt = week * 7 * 86400.0 + sow;
        gtime_t tt = TimeAdd(t_gpst0, dt);

        return tt;
    } /* end of gpst2time */

    /**
    * @brief   : time2gpst - the conversion from time to GPS week, seconds of week
    * @param[I]: tt (time struct (.mjd and .sod))
    * @param[O]: week (GPS week, nullptr:NO output)
    * @param[O]: sow (seconds of week, nullptr:NO output)
    * @return  : day of week
    * @note    :
    **/
    static int time2gpst(gtime_t tt, int& week, double& sow)
    {
        /* compute the time of the GPS starting time (1980-01-06 00:00:00) */
        const std::vector<double> gpst0 = { 1980, 1, 6, 0, 0, 0 };  /* GPS time reference */
        gtime_t t_gpst0 = ymdhms2time(gpst0);

        /* compute the time differences */
        double delta_day = TimeDiff(tt, t_gpst0) / 86400.0;

        /* compute the GPS week */
        week = (int)floor(delta_day / 7.0 + 1.0e-9);

        /* compute the days from the last GPS week to current day */
        int dow = (int)floor(delta_day - week * 7 + 1.0e-9);

        /* compute the seconds within GPS week */
        sow = dow * 24.0 * 3600.0 + tt.sod;

        return dow;
    } /* end of time2gpst */

    /**
    * @brief   : time2str - the conversion from gtime_t struct to string
    * @param[I]: tt (time struct (.mjd and .sod))
    * @param[O]: none
    * @return  : string ("yyyy/mm/dd hh:mm:ss.ssss")
    * @note    :
    **/
    static std::string time2str(gtime_t tt)
    {
        std::vector<double> ep;
        std::stringstream stime;

        ep = time2ymdhms(tt);
        stime << std::fixed   << std::setprecision(0) << ep[0] << "/"
              << std::setw(2) << std::setfill('0')    << ep[1] << "/"
              << std::setw(2) << std::setfill('0')    << ep[2] << " "
              << std::setw(2) << std::setfill('0')    << ep[3] << ":"
              << std::setw(2) << std::setfill('0')    << ep[4] << ":"
              << std::setw(6) << std::setfill('0')    << std::setprecision(3) << ep[5];
        return stime.str();
    } /* end of time2str */
};

#endif  // GTIME_H