/*------------------------------------------------------------------------------
 * ftps.cpp : GNSS data and products downloading module
 *
 * Copyright (C) 2020-2099 by Ubiquitous Navigation & Integrated positioning lab in Quest (UNIQ), all rights reserved.
 *    This file is part of GAMP II - GOOD (Gnss Observations and prOducts Downloader)
 *
 *     Author : Feng Zhou
 *    Contact : zhouforme@163.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *
 * References:
 *    Dr Cheng Wang @ BUAA provided the C++ code for observation downloading as reference
 *
 * History : 2021/04/16 1.0  new (by Feng Zhou @ SDUST)
 *           2021/04/20      MGEX multi-GNSS precise products option added (by Feng Zhou @ SDUST)
 *           2021/04/23 1.1  IGN and WHU FPT archives added (by Feng Zhou @ SDUST)
 *           2021/04/30      the day before and after the current day for precise satellite orbit and clock products
 *                             downloading is added (by Feng Zhou @ SDUST and Zan Liu @ CUMT)
 *           2021/04/30 1.2  CNES real-time orbit, clock, bias files (in offline mode) option added (by Feng Zhou @ SDUST)
 *           2021/05/01      from DOY 345/2020, 'brdcDDD0.YYn' was converted from '*.Z' to '*.gz' (by Feng Zhou @ SDUST and
 *                             Zan Liu @ CUMT)
 *           2021/05/01      added the full path of third-party softwares (by Feng Zhou @ SDUST and Yudan Yi)
 *           2021/05/05 1.3  add 'GetRoti' for Rate of TEC index (ROTI) file downloading (by Feng Zhou @ SDUST and Lei Liu @ WHU)
 *           2021/05/06 1.4  add the compatibility for changing directory (chdir) in Windows and Linux OS (by Feng Zhou @ SDUST)
 *           2021/05/07      add the option 'printInfoWget' in configure file for (not) printing the information of running
 *                             'wget' (by Feng Zhou @ SDUST and Hong Hu @ AHU)
 *           2021/05/08      add IGS and MGEX hourly observation (30s) downloading (by Feng Zhou @ SDUST and Menghao Li @ HEU)
 *           2021/05/10 1.5  modify some codes to make GOOD adaptable to different compression form (i.e., '*.Z' or '*.gz')
 *                             (by Feng Zhou @ SDUST)
 *           2021/05/12      add the option for IGR, IGU, GFU (from GFZ), and WUU (from WHU) products as well as the site-specific
 *                             broadcast ephemeris files downloading (by Feng Zhou @ SDUST and Menghao Li @ HEU)
 *           2021/05/15      sub-directory (i.e., daily, hourly, and highrate) creation for observation downloading (by Feng Zhou @ SDUST and
 *                             Menghao Li @ HEU)
 *           2021/05/18 1.6  modify some codes to make configuration file and program look more concise (by Feng Zhou @ SDUST and
 *                             Menghao Li @ HEU)
 *           2021/05/21      add the extraction and conversion for the 'all' option in IGS and MGEX observation downloading (by Feng Zhou @ SDUST)
 *           2021/06/01 1.7  add 'getObc', 'getObg', and 'getObh' options for Curtin University of Technology (CUT), Geoscience Australia (GA),
 *                             and Hong Kong CORS observation downloading (by Feng Zhou @ SDUST)
 *           2021/07/16      modify "*_R_" to "*" for CRX file naming convention (by Feng Zhou @ SDUST)
 *           2021/09/01 1.8  add various types of ultra-rapid (i.e., ESA, GFZ, IGS, WHU) and rapid (i.e., CODE, NRCan, ESA, GFZ, IGS)
 *                             orbit and clock products for downloading (by Feng Zhou @ SDUST and Sermet Ogutcu @ NEU)
 *           2021/09/04      add the option for NGS/NOAA CORS (from Amazon Web Services (AWS)) and EUREF Permanent Network (EPN)
 *                             observation downloading (by Feng Zhou @ SDUST)
 *           2021/09/05      fix the bug of broadcast ephemeris downloading via WHU FTP before and after 2020 (by Feng Zhou @ SDUST and
 *                             Zan Liu @ CUMT)
 *           2021/09/06      modify the setting of directories of GNSS observations and products in configure file to make them
 *                             more elastic (by Feng Zhou @ SDUST and Caoming Fan @ SDU)
 *           2021/12/01 1.9  delete the function of "GetDcbCode", "GetDcbMgex", "GetRtOrbClkCNT", and "GetRtBiasCNT" (by Feng Zhou @ SDUST)
 *           2021/12/03      add the option for CNES real-time and MGEX final ORBEX (ORBit EXchange format) for satllite attitude information
 *                             from MGEX (by Feng Zhou @ SDUST and Zan Liu @ CUMT)
 *           2021/12/05      add the option for CNES real-time and MGEX final observable-specific code/phase signal bias (OSB)
 *                             (by Feng Zhou @ SDUST)
 *           2021/12/10      add the option for various mixed broadcast ephemeris files downloading (i.e., "igs", "dlr", "ign", "gop", or
 *                             "wrd") (by Feng Zhou @ SDUST and Jiahuan Hu @ YorkU)
 *           2021/12/19      make many optimizations in function of "GetOrbClk", "GetEop", "GetObx", "GetDsb", and "GetOsb" (by Feng Zhou @ SDUST)
 *           2021/12/20      support high-frequency GNSS observation data (i.e., 1s) downloading from WHU FTP (by Feng Zhou @ SDUST)
 *           2021/12/20      fix the bug for hourly broadcast ephemeris file downloading from WHU FTP (by Feng Zhou @ SDUST and Menghao Li @ HEU)
 *           2021/12/21      add the corresponding codes for the comment/header component in 'site.list' (by Feng Zhou @ SDUST and
 *                             Biquan Cheng @ TJU)
 *           2021/12/23      detele the option of "rtnav" for "getNav" since the FTP address "ftp://ftp.lrz.de/transfer/steigenb/brdm" is
 *                             now invalid (by Feng Zhou @ SDUST)
 *           2021/12/24      add the downloading option for various rapid and predicted global ionosphere map (by Feng Zhou @ SDUST)
 *           2021/12/24      integrate all of the observation downloading options (i.e., "getObm", "getObc", ...) into "getObs" (by Feng Zhou @ SDUST)
 *           2021/12/25      add the option "igm" of "getObs" for taking the union of IGS and MGEX (IGS + MGEX, while the priority of
 *                             MGEX sites is higher) observation downloading with respect to the site name (by Feng Zhou @ SDUST and Menghao Li @ HEU)
 *           2021/12/25      add the function "GetDailyObsIgm", "GetHourlyObsIgm", and "GetHrObsIgm" for daily, hourly, and high-rate IGS and MGEX
 *                             observation downloading (taking the union of them) (by Feng Zhou @ SDUST)
 *           2021/12/29      add the function "GetDailyObsPbo2", "GetDailyObsPbo3", and "GetDailyObsPbo5" for daily Plate Boundary Observatory (PBO)
 *                             observation downloading ("pbo2" is for RINEX version 2.xx, short name "d"; "pbo3" is for RINEX version 3.xx,
 *                             long name "crx"; "pbo5" is for pbo2 + pbo3, while the priority of pbo3 sites is higher) (by Feng Zhou @ SDUST,
 *                             Yuze Yang  @ SDUST, and Zan Liu @ CUMT)
 *           2022/01/01 2.0  add the option "mixed3" and "mixed4" for RINEX 3.xx and 4.xx broadcast ephemeris file downloading (by Feng Zhou @ SDUST and Zan Liu @ CUMT)
 *           2022/01/05      add the option for generating log file to give information about whether the downloading was successful (by Feng Zhou @ SDUST and Zan Liu @ CUMT)
 *           2022/01/07      add the option for downloading prodcuts (i.e., "GetOrbClk" and "GetIono") from multiple analysis centers, i.e.,
 *                             "cod+igs", "cod+igs+jpl" etc (by Feng Zhou @ SDUST)
 *           2022/01/10      make a deeper optimization for "GetOrbClk" to make its codes more concise (by Feng Zhou @ SDUST)
 *           2022/01/15      adopt 2D vector to adapt to a variety of ultra-rapid ephemeris downloading, i.e., "cod+igs+igs_r+igs_u+whu_u+cnt" (by Feng Zhou @ SDUST)
 *           2022/01/16      add some codes to adapt to the orbit and clock products with "all" and/or "all_m" option(s), i.e.,
 *                             "cod+igs+igs_r+igs_u+whu_u+cnt+all+all_m" (by Feng Zhou @ SDUST)
 *           2022/01/17      perfect the output information of the log file to more intuitively understand the name of the downloading files (by Feng Zhou @ SDUST)
 *           2022/02/19      change "BRDX*" to "BRD4" for "mixed4" (RINEX 4.xx) broadcast ephemeris file downloading (by Feng Zhou @ SDUST and Zan Liu @ CUMT)
 *           2022/03/19      move the codes for log file writing from "FtpUtil.cpp" to "PreProcess.cpp" (by Feng Zhou @ SDUST)
 *           2022/07/06 2.1  add the configuration file in YAML format (by Feng Zhou and Yuze Yang @ SDUST)
 *           2022/08/01      modify the main function "run_GOOD.cpp" for better adaptation to Windows and Linux, as well as debug and release modes (by Feng Zhou @ SDUST)
 *           2022/09/03      delete the options for selecting sub-directory in relative or absolute path (by Feng Zhou @ SDUST)
 *           2022/10/09      the addition of downloading MGEX prodcuts from Russia Information and Analysis Center (IAC), Japan Aerospace Exploration Agency (JAXA), and Shanghai Observatory (SHAO) (by Feng Zhou @ SDUST)
 *           2022/11/20      the option of 'igsdepot.ign.fr' for multi-GNSS orbit and clock products downloading was added (by Feng Zhou @ SDUST)
 *           2023/02/13 2.2  the modifications in IGS and the other AC final, rapid and ultra-rapid prodcuts downloading for the adaptation of long file name (by Feng Zhou @ SDUST)
 *           2023/10/06 3.0  mojor modifications and improvements (by Feng Zhou @ SDUST)
 *           2023/11/05      all product downloads with long file names have been adapted (by Feng Zhou @ SDUST)
 *           2023/12/14      fix a bug when the station list file was named "all" (by Feng Zhou @ SDUST)
 *           2023/12/25      add the Python script 'run_GOOD.py' to make GOOD easier to use (by Feng Zhou @ SDUST)
 *           2024/01/30 3.1  add the option of merging SP3 files into one file (by Feng Zhou @ SDUST)
 *           2024/02/15      fix some bugs during the various tests (by Feng Zhou @ SDUST)
 *           2024/02/20      add two Bash scripts named 'crx_downloaded_fromGA.sh' and 'rxo_downloaded_fromGA.sh' for GA observations downloading (by Feng Zhou @ SDUST)
 *----------------------------------------------------------------------------*/
#include "../common/common.h"
#include "../common/gtime.h"
#include "../common/cstring.h"
#include "../common/logger.h"
#include "ftps.h"

/* constants/macros ----------------------------------------------------------*/
#define IDX_OBSD   0    /* index for IGS daily observation (30s) file downloaded (short name "d" files) */
#define IDX_OBSH   1    /* index for IGS hourly observation (30s) file downloaded (short name "d" files) */
#define IDX_OBSHR  2    /* index for IGS high-rate observation (1s) file downloaded (short name "d" files) */
#define IDX_OBMD   3    /* index for MGEX daily observation (30s) file downloaded (long name "crx" files) */
#define IDX_OBMH   4    /* index for MGEX daily observation (30s) file downloaded (long name "crx" files) */
#define IDX_OBMHR  5    /* index for MGEX high-rate observation (1s) file downloaded (long name "crx" files) */
#define IDX_NAV    6    /* index for broadcast ephemeris file downloaded */
#define IDX_SP3    7    /* index for IGS SP3 file downloaded */
#define IDX_CLK    8    /* index for IGS CLK file downloaded */
#define IDX_EOP    9    /* index for IGS EOP file downloaded */
#define IDX_SNX    10   /* index for IGS weekly SINEX file downloaded */
#define IDX_SP3M   11   /* index for MGEX SP3 file downloaded */
#define IDX_CLKM   12   /* index for MGEX CLK file downloaded */
#define IDX_OBXM   13   /* index for MGEX ORBEX file downloaded */
#define IDX_DSBM   14   /* index for MGEX DSB file downloaded */
#define IDX_OSBM   15   /* index for MGEX OSB file downloaded */
#define IDX_ION    16   /* index for global ionosphere map (GIM) downloaded */
#define IDX_ROTI   17   /* index for Rate of TEC index (ROTI) downloaded */
#define IDX_ZTD    18   /* index for IGS final tropospheric product downloaded */

#define PROD_REALTIME    1   /* index for real-time orbit and clock products downloading */
#define PROD_ULTRA_RAPID 2   /* index for ultra-rapid orbit and clock products downloading */
#define PROD_RAPID       3   /* index for rapid orbit and clock products downloading */
#define PROD_FINAL_IGS   4   /* index for IGS final orbit and clock products downloading */
#define PROD_FINAL_MGEX  5   /* index for MGEX final orbit and clock products downloading */

/* function definition -------------------------------------------------------*/

/**
* @brief   : init - Get FTP archive for CDDIS, IGN, or WHU
* @param[I]: none
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::init()
{
    /* FTP archive for CDDIS */
    ftparchive_.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/data/daily");                /* IGS daily observation (30s) files */
    ftparchive_.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/data/hourly");               /* IGS hourly observation (30s) files */
    ftparchive_.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/data/highrate");             /* IGS high-rate observation (1s) files */
    ftparchive_.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/data/daily");                /* MGEX daily observation (30s) files */
    ftparchive_.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/data/hourly");               /* MGEX hourly observation (30s) files */
    ftparchive_.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/data/highrate");             /* MGEX high-rate observation (1s) files */
    ftparchive_.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/data/daily");                /* broadcast ephemeris files */
    ftparchive_.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products");                  /* IGS SP3 files */
    ftparchive_.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products");                  /* IGS CLK files */
    ftparchive_.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products");                  /* IGS EOP files */
    ftparchive_.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products");                  /* IGS weekly SINEX files */
    ftparchive_.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products/mgex");             /* MGEX SP3 files */
    ftparchive_.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products/mgex");             /* MGEX CLK files */
    ftparchive_.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products/mgex");             /* MGEX ORBEX files */
    ftparchive_.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products/bias");             /* MGEX DSB files */
    ftparchive_.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products/mgex");             /* MGEX OSB files */
    ftparchive_.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products/ionex");            /* global ionosphere map (GIM) files */
    ftparchive_.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products/ionex");            /* Rate of TEC index (ROTI) files */
    ftparchive_.CDDIS.push_back("ftps://gdc.cddis.eosdis.nasa.gov/pub/gnss/products/troposphere/zpd");  /* IGS final tropospheric product files */

    /* FTP archive for IGN */
    ftparchive_.IGN.push_back("ftp://igs.ign.fr/pub/igs/data");                                         /* IGS daily observation (30s) files */
    ftparchive_.IGN.push_back("ftp://igs.ign.fr/pub/igs/data/hourly");                                  /* IGS hourly observation (30s) files */
    ftparchive_.IGN.push_back("ftp://igs.ign.fr/pub/igs/data/highrate");                                /* IGS high-rate observation (1s) files */
    ftparchive_.IGN.push_back("ftp://igs.ign.fr/pub/igs/data");                                         /* MGEX daily observation (30s) files */
    ftparchive_.IGN.push_back("ftp://igs.ign.fr/pub/igs/data/hourly");                                  /* MGEX hourly observation (30s) files */
    ftparchive_.IGN.push_back("ftp://igs.ign.fr/pub/igs/data/highrate");                                /* MGEX high-rate observation (1s) files */
    ftparchive_.IGN.push_back("ftp://igs.ign.fr/pub/igs/data");                                         /* broadcast ephemeris files */
    ftparchive_.IGN.push_back("ftp://igs.ign.fr/pub/igs/products");                                     /* IGS SP3 files */
    ftparchive_.IGN.push_back("ftp://igs.ign.fr/pub/igs/products");                                     /* IGS CLK files */
    ftparchive_.IGN.push_back("ftp://igs.ign.fr/pub/igs/products");                                     /* IGS EOP files */
    ftparchive_.IGN.push_back("ftp://igs.ign.fr/pub/igs/products");                                     /* IGS weekly SINEX files */
    ftparchive_.IGN.push_back("ftp://igs.ign.fr/pub/igs/products/mgex");                                /* MGEX SP3 files */
    ftparchive_.IGN.push_back("ftp://igs.ign.fr/pub/igs/products/mgex");                                /* MGEX CLK files */
    ftparchive_.IGN.push_back("ftp://igs.ign.fr/pub/igs/products/mgex");                                /* MGEX ORBEX files */
    ftparchive_.IGN.push_back("ftp://igs.ign.fr/pub/igs/products/mgex/dcb");                            /* MGEX DSB files */
    ftparchive_.IGN.push_back("ftp://igs.ign.fr/pub/igs/products/mgex");                                /* MGEX OSB files */
    ftparchive_.IGN.push_back("ftp://igs.ign.fr/pub/igs/products/ionosphere");                          /* global ionosphere map (GIM) files */
    ftparchive_.IGN.push_back("ftp://igs.ign.fr/pub/igs/products/ionosphere");                          /* Rate of TEC index (ROTI) files */
    ftparchive_.IGN.push_back("ftp://igs.ign.fr/pub/igs/products/troposphere");                         /* IGS final tropospheric product files */

    /* FTP archive for WHU */
    ftparchive_.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/data/daily");                               /* IGS daily observation (30s) files */
    ftparchive_.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/data/hourly");                              /* IGS hourly observation (30s) files */
    ftparchive_.WHU.push_back("ftp://igs.gnsswhu.cn/pub/highrate");                                     /* IGS high-rate observation (1s) files */
    ftparchive_.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/data/daily");                               /* MGEX daily observation (30s) files */
    ftparchive_.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/data/hourly");                              /* MGEX hourly observation (30s) files */
    ftparchive_.WHU.push_back("ftp://igs.gnsswhu.cn/pub/highrate");                                     /* MGEX high-rate observation (1s) files */
    ftparchive_.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/data/daily");                               /* broadcast ephemeris files */
    ftparchive_.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products");                                 /* IGS SP3 files */
    ftparchive_.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products");                                 /* IGS CLK files */
    ftparchive_.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products");                                 /* IGS EOP files */
    ftparchive_.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products");                                 /* IGS weekly SINEX files */
    ftparchive_.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products/mgex");                            /* MGEX SP3 files */
    ftparchive_.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products/mgex");                            /* MGEX CLK files */
    ftparchive_.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products/mgex");                            /* MGEX ORBEX files */
    ftparchive_.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products/mgex/dcb");                        /* MGEX DSB files */
    ftparchive_.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products/mgex");                            /* MGEX OSB files */
    ftparchive_.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products/ionex");                           /* global ionosphere map (GIM) files */
    ftparchive_.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products/ionex");                           /* Rate of TEC index (ROTI) files */
    ftparchive_.WHU.push_back("ftp://igs.gnsswhu.cn/pub/gps/products/troposphere/new");                 /* IGS final tropospheric product files */
} /* end of init */

/**
* @brief   : GetDailyObsIgs - download IGS RINEX daily observation (30s) files (short name "d") according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetDailyObsIgs(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* creation of sub-directory ('daily') */
    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string subdir = dir + sep + "daily";
    if (access(subdir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        std::string cmd = "mkdir " + subdir;
#else           /* for Linux or Mac */
        std::string cmd = "mkdir -p " + subdir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subdir.c_str());
#else           /* for Linux or Mac */
    chdir(subdir.c_str());
#endif

    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);

    /* FTP archive: CDDIS, IGN, or WHU */
    std::string ftpname = fopt->ftpfrom;
    CString::trim(ftpname);
    CString::ToUpper(ftpname);
    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, crx2rnxfull = fopt->crx2rnxfull, qr = fopt->qr;
    std::string::size_type ipos = fopt->obslist.find_last_of(sep) + 1;
    std::string obslist = fopt->obslist.substr(ipos, fopt->obslist.length() - ipos);
    if ((int)obslist.length() < 9) /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* download all the IGS observation files */
        std::string url, cutdirs = " --cut-dirs=7 ";
        if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBSD] + "/" + syyyy + "/" + sdoy + "/" + syy + "d";
        else if (ftpname == "IGN")
        {
            url = ftparchive_.IGN[IDX_OBSD] + "/" + syyyy + "/" + sdoy;
            cutdirs = " --cut-dirs=5 ";
        }
        else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_OBSD] + "/" + syyyy + "/" + sdoy + "/" + syy + "d";
        else url = ftparchive_.CDDIS[IDX_OBSD] + "/" + syyyy + "/" + sdoy + "/" + syy + "d";

        /* it is OK for '*.Z' or '*.gz' format */
        std::string dfile = "*" + sdoy + "0." + syy + "d";
        std::string dxfile = dfile + ".*";
        std::string cmd = wgetfull + " " + qr + " -nH -A " + dxfile + cutdirs + url;
        std::system(cmd.c_str());

        /* get the file list */
        std::string suffix = "." + syy + "d";
        std::vector<std::string> dfiles;
        CString::GetFilesAll(subdir, suffix, dfiles);
        for (size_t i = 0; i < dfiles.size(); i++)
        {
            if (access(dfiles[i].c_str(), 0) == 0)
            {
                /* extract it */
                cmd = gzipfull + " -d -f " + dfiles[i];
                std::system(cmd.c_str());

                std::string site = dfiles[i].substr(0, 4);
                CString::ToLower(site);
                dfile = site + sdoy + "0." + syy + "d";
                if (access(dfile.c_str(), 0) == -1) continue;
                std::string ofile = site + sdoy + "0." + syy + "o";
                /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxfull + " " + dfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                cmd = "cat " + dfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                std::system(cmd.c_str());

                /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                cmd = "del " + dfile;
#else           /* for Linux or Mac */
                cmd = "rm -rf " + dfile;
#endif
                std::system(cmd.c_str());
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the IGS observation file site-by-site */
        if (access(fopt->obslist.c_str(), 0) == 0)
        {
            std::ifstream sitlst(fopt->obslist);
            if (!sitlst.is_open())
            {
                Logger::Trace(TERROR, "*** ERROR(FtpUtil::GetDailyObsIgs): open site.list = " + fopt->obslist +
                    " file failed, please check it");
                return;
            }

            std::string site, sitname;
            while (getline(sitlst, site))
            {
                if (site[0] == '#') continue;
                CString::trim(site);
                if (site.size() < 4) continue;
                sitname = site.substr(0, 4);
                CString::ToLower(sitname);
                std::string ofile = sitname + sdoy + "0." + syy + "o";
                std::string dfile = sitname + sdoy + "0." + syy + "d";
                if (access(ofile.c_str(), 0) == -1 && access(dfile.c_str(), 0) == -1)
                {
                    std::string url, cutdirs = " --cut-dirs=7 ";
                    if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBSD] + "/" + syyyy + "/" + sdoy + "/" + syy + "d";
                    else if (ftpname == "IGN")
                    {
                        url = ftparchive_.IGN[IDX_OBSD] + "/" + syyyy + "/" + sdoy;
                        cutdirs = " --cut-dirs=5 ";
                    }
                    else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_OBSD] + "/" + syyyy + "/" + sdoy + "/" + syy + "d";
                    else url = ftparchive_.CDDIS[IDX_OBSD] + "/" + syyyy + "/" + sdoy + "/" + syy + "d";

                    /* it is OK for '*.Z' or '*.gz' format */
                    std::string dxfile = dfile + ".*";
                    std::string cmd = wgetfull + " " + qr + " -nH -A " + dxfile + cutdirs + url;
                    std::system(cmd.c_str());

                    std::string dgzfile = dfile + ".gz", dzfile = dfile + ".Z";
                    bool isgz = false;
                    if (access(dgzfile.c_str(), 0) == 0)
                    {
                        /* extract '*.gz' */
                        cmd = gzipfull + " -d -f " + dgzfile;
                        std::system(cmd.c_str());
                        isgz = true;

                        if (access(dfile.c_str(), 0) == 0 && access(dzfile.c_str(), 0) == 0)
                        {
                            /* delete '*.Z' file */
#ifdef _WIN32   /* for Windows */
                            cmd = "del " + dzfile;
#else           /* for Linux or Mac */
                            cmd = "rm -rf " + dzfile;
#endif
                            std::system(cmd.c_str());
                        }
                    }
                    if (access(dfile.c_str(), 0) == -1 && access(dzfile.c_str(), 0) == 0)
                    {
                        /* extract '*.Z' */
                        cmd = gzipfull + " -d -f " + dzfile;
                        std::system(cmd.c_str());
                        isgz = false;
                    }

                    char tmpfile[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpfile, "%s%c%s", subdir.c_str(), sep, ofile.c_str());
                    std::string localfile = tmpfile;
                    if (access(dfile.c_str(), 0) == -1)
                    {
                        Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetDailyObsIgs): failed to download IGS daily observation file " + ofile);

                        std::string url0 = url + '/' + dxfile;
                        if (fplog_.is_open()) fplog_ << "* WARNING(GetDailyObsIgs): " << url0 << "  ->  " << localfile <<
                            "  failed" << std::endl;

                        continue;
                    }

#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxfull + " " + dfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                    cmd = "cat " + dfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                    std::system(cmd.c_str());

                    if (access(ofile.c_str(), 0) == 0)
                    {
                        Logger::Trace(TINFO, "*** INFO(FtpUtil::GetDailyObsIgs): successfully download IGS daily observation file " + ofile);

                        if (isgz) dxfile = dgzfile;
                        else dxfile = dzfile;
                        std::string url0 = url + '/' + dxfile;
                        if (fplog_.is_open()) fplog_ << "* INFO(GetDailyObsIgs): " << url0 << "  ->  " << localfile <<
                            "  OK" << std::endl;

                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + dfile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + dfile;
#endif
                        std::system(cmd.c_str());
                    }
                }
                else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetDailyObsIgs): IGS daily observation file " + ofile +
                    " or " + dfile + " has existed!");
            }

            /* close 'site.list' */
            sitlst.close();
        }
    }
} /* end of GetDailyObsIgs */

/**
* @brief   : GetHourlyObsIgs - download IGS RINEX hourly observation (30s) files (short name "d") according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetHourlyObsIgs(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* creation of sub-directory ('hourly') */
    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string subdir = dir + sep + "hourly";
    if (access(subdir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        std::string cmd = "mkdir " + subdir;
#else           /* for Linux or Mac */
        std::string cmd = "mkdir -p " + subdir;
#endif
        std::system(cmd.c_str());
    }

    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);

    /* FTP archive: CDDIS, IGN, or WHU */
    std::string ftpname = fopt->ftpfrom;
    CString::trim(ftpname);
    CString::ToUpper(ftpname);
    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, crx2rnxfull = fopt->crx2rnxfull, qr = fopt->qr;
    std::string::size_type ipos = fopt->obslist.find_last_of(sep) + 1;
    std::string obslist = fopt->obslist.substr(ipos, fopt->obslist.length() - ipos);
    if ((int)obslist.length() < 9) /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* download all the IGS observation files */
        for (size_t i = 0; i < fopt->hhobs.size(); i++)
        {
            std::string shh = CString::int2str(fopt->hhobs[i], 2);
            char tmpdir[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            sprintf(tmpdir, "%s%c%s", subdir.c_str(), sep, shh.c_str());
            std::string shhdir = tmpdir;
            if (access(shhdir.c_str(), 0) == -1)
            {
                /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                std::string cmd = "mkdir " + shhdir;
#else           /* for Linux or Mac */
                std::string cmd = "mkdir -p " + shhdir;
#endif
                std::system(cmd.c_str());
            }

            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(shhdir.c_str());
#else           /* for Linux or Mac */
            chdir(shhdir.c_str());
#endif

            std::string url, cutdirs = " --cut-dirs=7 ";
            if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBSH] + "/" + syyyy + "/" + sdoy + "/" + shh;
            else if (ftpname == "IGN")
            {
                url = ftparchive_.IGN[IDX_OBSH] + "/" + syyyy + "/" + sdoy;
                cutdirs = " --cut-dirs=6 ";
            }
            else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_OBSH] + "/" + syyyy + "/" + sdoy + "/" + shh;
            else url = ftparchive_.CDDIS[IDX_OBSH] + "/" + syyyy + "/" + sdoy + "/" + shh;

            /* 'a' = 97, 'b' = 98, ... */
            int ii = fopt->hhobs[i] + 97;
            char ch = ii;
            std::string sch;
            sch.push_back(ch);
            /* it is OK for '*.Z' or '*.gz' format */
            std::string dfile = "*" + sdoy + sch + "." + syy + "d";
            std::string dxfile = dfile + ".*";
            std::string cmd = wgetfull + " " + qr + " -nH -A " + dxfile + cutdirs + url;
            std::system(cmd.c_str());

            /* get the file list */
            std::string suffix = "." + syy + "d";
            std::vector<std::string> dfiles;
            CString::GetFilesAll(shhdir, suffix, dfiles);
            for (size_t i = 0; i < dfiles.size(); i++)
            {
                if (access(dfiles[i].c_str(), 0) == 0)
                {
                    /* extract it */
                    cmd = gzipfull + " -d -f " + dfiles[i];
                    std::system(cmd.c_str());

                    std::string site = dfiles[i].substr(0, 4);
                    CString::ToLower(site);
                    dfile = site + sdoy + sch + "." + syy + "d";
                    if (access(dfile.c_str(), 0) == -1) continue;
                    std::string ofile = site + sdoy + sch + "." + syy + "o";
                    /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxfull + " " + dfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                    cmd = "cat " + dfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                    std::system(cmd.c_str());

                    /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                    cmd = "del " + dfile;
#else           /* for Linux or Mac */
                    cmd = "rm -rf " + dfile;
#endif
                    std::system(cmd.c_str());
                }
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the IGS observation file site-by-site */
        if (access(fopt->obslist.c_str(), 0) == 0)
        {
            std::ifstream sitlst(fopt->obslist);
            if (!sitlst.is_open())
            {
                Logger::Trace(TERROR, "*** ERROR(FtpUtil::GetHourlyObsIgs): open site.list = " + fopt->obslist +
                    " file failed, please check it");
                return;
            }

            std::string site, sitname;
            while (getline(sitlst, site))
            {
                if (site[0] == '#') continue;
                CString::trim(site);
                if (site.size() < 4) continue;
                sitname = site.substr(0, 4);
                for (size_t i = 0; i < fopt->hhobs.size(); i++)
                {
                    std::string shh = CString::int2str(fopt->hhobs[i], 2);
                    char tmpdir[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpdir, "%s%c%s", subdir.c_str(), sep, shh.c_str());
                    std::string shhdir = tmpdir;
                    if (access(shhdir.c_str(), 0) == -1)
                    {
                        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                        std::string cmd = "mkdir " + shhdir;
#else           /* for Linux or Mac */
                        std::string cmd = "mkdir -p " + shhdir;
#endif
                        std::system(cmd.c_str());
                    }

                    /* change directory */
#ifdef _WIN32   /* for Windows */
                    _chdir(shhdir.c_str());
#else           /* for Linux or Mac */
                    chdir(shhdir.c_str());
#endif

                    /* 'a' = 97, 'b' = 98, ... */
                    int ii = fopt->hhobs[i] + 97;
                    char ch = ii;
                    std::string sch;
                    sch.push_back(ch);
                    CString::ToLower(sitname);
                    std::string ofile = sitname + sdoy + sch + "." + syy + "o";
                    std::string dfile = sitname + sdoy + sch + "." + syy + "d";
                    if (access(ofile.c_str(), 0) == -1 && access(dfile.c_str(), 0) == -1)
                    {
                        std::string url, cutdirs = " --cut-dirs=7 ";
                        if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBSH] + "/" + syyyy + "/" + sdoy + "/" + shh;
                        else if (ftpname == "IGN")
                        {
                            url = ftparchive_.IGN[IDX_OBSH] + "/" + syyyy + "/" + sdoy;
                            cutdirs = " --cut-dirs=6 ";
                        }
                        else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_OBSH] + "/" + syyyy + "/" + sdoy + "/" + shh;
                        else url = ftparchive_.CDDIS[IDX_OBSH] + "/" + syyyy + "/" + sdoy + "/" + shh;

                        /* it is OK for '*.Z' or '*.gz' format */
                        std::string dxfile = dfile + ".*";
                        std::string cmd = wgetfull + " " + qr + " -nH -A " + dxfile + cutdirs + url;
                        std::system(cmd.c_str());

                        std::string dgzfile = dfile + ".gz", dzfile = dfile + ".Z";
                        bool isgz = false;
                        if (access(dgzfile.c_str(), 0) == 0)
                        {
                            /* extract '*.gz' */
                            cmd = gzipfull + " -d -f " + dgzfile;
                            std::system(cmd.c_str());
                            isgz = true;

                            if (access(dfile.c_str(), 0) == 0 && access(dzfile.c_str(), 0) == 0)
                            {
                                /* delete '*.Z' file */
#ifdef _WIN32   /* for Windows */
                                cmd = "del " + dzfile;
#else           /* for Linux or Mac */
                                cmd = "rm -rf " + dzfile;
#endif
                                std::system(cmd.c_str());
                            }
                        }
                        if (access(dfile.c_str(), 0) == -1 && access(dzfile.c_str(), 0) == 0)
                        {
                            /* extract '*.Z' */
                            cmd = gzipfull + " -d -f " + dzfile;
                            std::system(cmd.c_str());
                            isgz = false;
                        }

                        char tmpfile[MAXSTRPATH] = { '\0' };
                        char sep = (char)FILEPATHSEP;
                        sprintf(tmpfile, "%s%c%s", shhdir.c_str(), sep, ofile.c_str());
                        std::string localfile = tmpfile;
                        if (access(dfile.c_str(), 0) == -1)
                        {
                            Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetHourlyObsIgs): failed to download IGS Hourly observation file " + ofile);

                            std::string url0 = url + '/' + dxfile;
                            if (fplog_.is_open()) fplog_ << "* WARNING(GetHourlyObsIgs): " << url0 << "  ->  " <<
                                localfile << "  failed" << std::endl;

                            continue;
                        }

#ifdef _WIN32  /* for Windows */
                        cmd = crx2rnxfull + " " + dfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                        cmd = "cat " + dfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                        std::system(cmd.c_str());

                        if (access(ofile.c_str(), 0) == 0)
                        {
                            Logger::Trace(TINFO, "*** INFO(FtpUtil::GetHourlyObsIgs): successfully download IGS hourly observation file " + ofile);

                            if (isgz) dxfile = dgzfile;
                            else dxfile = dzfile;
                            std::string url0 = url + '/' + dxfile;
                            if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetHourlyObsIgs): " << url0 << "  ->  " <<
                                localfile << "  OK" << std::endl;

                            /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                            cmd = "del " + dfile;
#else           /* for Linux or Mac */
                            cmd = "rm -rf " + dfile;
#endif
                            std::system(cmd.c_str());
                        }
                    }
                    else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetHourlyObsIgs): IGS hourly observation file " +
                        ofile + " or " + dfile + " has existed!");
                }
            }

            /* close 'site.list' */
            sitlst.close();
        }
    }
} /* end of GetHourlyObsIgs */

/**
* @brief   : GetHrObsIgs - download IGS RINEX high-rate observation (1s) files (short name "d") according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetHrObsIgs(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* creation of sub-directory ('highrate') */
    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string subdir = dir + sep + "highrate";
    if (access(subdir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        std::string cmd = "mkdir " + subdir;
#else           /* for Linux or Mac */
        std::string cmd = "mkdir -p " + subdir;
#endif
        std::system(cmd.c_str());
    }

    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);

    /* FTP archive: CDDIS, IGN, or WHU */
    std::string ftpname = fopt->ftpfrom;
    CString::trim(ftpname);
    CString::ToUpper(ftpname);
    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, crx2rnxfull = fopt->crx2rnxfull, qr = fopt->qr;
    std::string::size_type ipos = fopt->obslist.find_last_of(sep) + 1;
    std::string obslist = fopt->obslist.substr(ipos, fopt->obslist.length() - ipos);
    if ((int)obslist.length() < 9) /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* download all the IGS observation files */
        for (size_t i = 0; i < fopt->hhobs.size(); i++)
        {
            std::string shh = CString::int2str(fopt->hhobs[i], 2);
            char tmpdir[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            sprintf(tmpdir, "%s%c%s", subdir.c_str(), sep, shh.c_str());
            std::string shhdir = tmpdir;
            if (access(shhdir.c_str(), 0) == -1)
            {
                /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                std::string cmd = "mkdir " + shhdir;
#else           /* for Linux or Mac */
                std::string cmd = "mkdir -p " + shhdir;
#endif
                std::system(cmd.c_str());
            }

            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(shhdir.c_str());
#else           /* for Linux or Mac */
            chdir(shhdir.c_str());
#endif
            std::string url, cutdirs = " --cut-dirs=8 ";
            if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBSHR] + "/" + syyyy + "/" + sdoy + "/" + syy + "d" + "/" + shh;
            else if (ftpname == "IGN")
            {
                url = ftparchive_.IGN[IDX_OBSHR] + "/" + syyyy + "/" + sdoy;
                cutdirs = " --cut-dirs=6 ";
            }
            else if (ftpname == "WHU")
            {
                url = ftparchive_.WHU[IDX_OBSHR] + "/" + syyyy + "/" + sdoy + "/" + syy + "d" + "/" + shh;
                cutdirs = " --cut-dirs=6 ";
            }
            else url = ftparchive_.CDDIS[IDX_OBSHR] + "/" + syyyy + "/" + sdoy + "/" + syy + "d" + "/" + shh;

            /* 'a' = 97, 'b' = 98, ... */
            int ii = fopt->hhobs[i] + 97;
            char ch = ii;
            std::string sch;
            sch.push_back(ch);
            /* it is OK for '*.Z' or '*.gz' format */
            std::string dfile = "*" + sdoy + sch + "*." + syy + "d";
            std::string dxfile = dfile + ".*";
            std::string cmd = wgetfull + " " + qr + " -nH -A " + dxfile + cutdirs + url;
            std::system(cmd.c_str());

            std::vector<std::string> smin = { "00", "15", "30", "45" };
            for (size_t i = 0; i < smin.size(); i++)
            {
                /* get the file list */
                std::string suffix = smin[i] + "." + syy + "d";
                std::vector<std::string> dfiles;
                CString::GetFilesAll(shhdir, suffix, dfiles);
                for (size_t j = 0; j < dfiles.size(); j++)
                {
                    if (access(dfiles[j].c_str(), 0) == 0)
                    {
                        /* extract it */
                        cmd = gzipfull + " -d -f " + dfiles[j];
                        std::system(cmd.c_str());

                        std::string site = dfiles[j].substr(0, 4);
                        CString::ToLower(site);
                        dfile = site + sdoy + sch + smin[i] + "." + syy + "d";
                        if (access(dfile.c_str(), 0) == -1) continue;
                        std::string ofile = site + sdoy + sch + smin[i] + "." + syy + "o";
                        /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                        cmd = crx2rnxfull + " " + dfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                        cmd = "cat " + dfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                        std::system(cmd.c_str());

                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + dfile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + dfile;
#endif
                        std::system(cmd.c_str());
                    }
                }
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the IGS observation file site-by-site */
        if (access(fopt->obslist.c_str(), 0) == 0)
        {
            std::ifstream sitlst(fopt->obslist);
            if (!sitlst.is_open())
            {
                Logger::Trace(TERROR, "*** ERROR(FtpUtil::GetHrObsIgs): open site.list = " + fopt->obslist +
                    " file failed, please check it");
                return;
            }

            std::string site, sitname;
            while (getline(sitlst, site))
            {
                if (site[0] == '#') continue;
                CString::trim(site);
                if (site.size() < 4) continue;
                sitname = site.substr(0, 4);
                for (size_t i = 0; i < fopt->hhobs.size(); i++)
                {
                    std::string shh = CString::int2str(fopt->hhobs[i], 2);
                    char tmpdir[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpdir, "%s%c%s", subdir.c_str(), sep, shh.c_str());
                    std::string shhdir = tmpdir;
                    if (access(shhdir.c_str(), 0) == -1)
                    {
                        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                        std::string cmd = "mkdir " + shhdir;
#else           /* for Linux or Mac */
                        std::string cmd = "mkdir -p " + shhdir;
#endif
                        std::system(cmd.c_str());
                    }

                    /* change directory */
#ifdef _WIN32   /* for Windows */
                    _chdir(shhdir.c_str());
#else           /* for Linux or Mac */
                    chdir(shhdir.c_str());
#endif

                    /* 'a' = 97, 'b' = 98, ... */
                    int ii = fopt->hhobs[i] + 97;
                    char ch = ii;
                    std::string sch;
                    sch.push_back(ch);
                    std::vector<std::string> smin = { "00", "15", "30", "45" };
                    for (size_t i = 0; i < smin.size(); i++)
                    {
                        CString::ToLower(sitname);
                        std::string ofile = sitname + sdoy + sch + smin[i] + "." + syy + "o";
                        std::string dfile = sitname + sdoy + sch + smin[i] + "." + syy + "d";
                        if (access(ofile.c_str(), 0) == -1 && access(dfile.c_str(), 0) == -1)
                        {
                            std::string url, cutdirs = " --cut-dirs=8 ";
                            if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBSHR] + "/" + syyyy + "/" + sdoy + "/" +
                                syy + "d" + "/" + shh;
                            else if (ftpname == "IGN")
                            {
                                url = ftparchive_.IGN[IDX_OBSHR] + "/" + syyyy + "/" + sdoy;
                                cutdirs = " --cut-dirs=6 ";
                            }
                            else if (ftpname == "WHU")
                            {
                                url = ftparchive_.WHU[IDX_OBSHR] + "/" + syyyy + "/" + sdoy + "/" + syy + "d" + "/" + shh;
                                cutdirs = " --cut-dirs=6 ";
                            }
                            else url = ftparchive_.CDDIS[IDX_OBSHR] + "/" + syyyy + "/" + sdoy + "/" + syy + "d" + "/" + shh;

                            /* it is OK for '*.Z' or '*.gz' format */
                            std::string dxfile = dfile + ".*";
                            std::string cmd = wgetfull + " " + qr + " -nH -A " + dxfile + cutdirs + url;
                            std::system(cmd.c_str());

                            std::string dgzfile = dfile + ".gz", dzfile = dfile + ".Z";
                            bool isgz = false;
                            if (access(dgzfile.c_str(), 0) == 0)
                            {
                                /* extract '*.gz' */
                                cmd = gzipfull + " -d -f " + dgzfile;
                                std::system(cmd.c_str());
                                isgz = true;
                                if (access(dfile.c_str(), 0) == 0 && access(dzfile.c_str(), 0) == 0)
                                {
                                    /* delete '*.Z' file */
#ifdef _WIN32   /* for Windows */
                                    cmd = "del " + dzfile;
#else           /* for Linux or Mac */
                                    cmd = "rm -rf " + dzfile;
#endif
                                    std::system(cmd.c_str());
                                }
                            }
                            if (access(dfile.c_str(), 0) == -1 && access(dzfile.c_str(), 0) == 0)
                            {
                                /* extract '*.Z' */
                                cmd = gzipfull + " -d -f " + dzfile;
                                std::system(cmd.c_str());
                                isgz = false;
                            }

                            char tmpfile[MAXSTRPATH] = { '\0' };
                            char sep = (char)FILEPATHSEP;
                            sprintf(tmpfile, "%s%c%s", shhdir.c_str(), sep, ofile.c_str());
                            std::string localfile = tmpfile;
                            if (access(dfile.c_str(), 0) == -1)
                            {
                                Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetHrObsIgs): failed to download IGS high-rate observation file " + ofile);

                                std::string url0 = url + '/' + dxfile;
                                if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetHrObsIgs): " << url0 << "  ->  " <<
                                    localfile << "  failed" << std::endl;

                                continue;
                            }

#ifdef _WIN32  /* for Windows */
                            cmd = crx2rnxfull + " " + dfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                            cmd = "cat " + dfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                            std::system(cmd.c_str());

                            if (access(ofile.c_str(), 0) == 0)
                            {
                                Logger::Trace(TINFO, "*** INFO(FtpUtil::GetHrObsIgs): successfully download IGS high-rate observation file " + ofile);

                                if (isgz) dxfile = dgzfile;
                                else dxfile = dzfile;
                                std::string url0 = url + '/' + dxfile;
                                if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetHrObsIgs): " << url0 << "  ->  " <<
                                    localfile << "  OK" << std::endl;

                                /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                                cmd = "del " + dfile;
#else           /* for Linux or Mac */
                                cmd = "rm -rf " + dfile;
#endif
                                std::system(cmd.c_str());
                            }
                        }
                        else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetHrObsIgs): IGS high-rate observation file " +
                            ofile + " or " + dfile + " has existed!");
                    }
                }
            }

            /* close 'site.list' */
            sitlst.close();
        }
    }
} /* end of GetHrObsIgs */

/**
* @brief   : GetDailyObsMgex - download MGEX RINEX daily observation (30s) files (long name "crx") according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetDailyObsMgex(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* creation of sub-directory ('daily') */
    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string subdir = dir + sep + "daily";
    if (access(subdir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        std::string cmd = "mkdir " + subdir;
#else           /* for Linux or Mac */
        std::string cmd = "mkdir -p " + subdir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subdir.c_str());
#else           /* for Linux or Mac */
    chdir(subdir.c_str());
#endif

    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);

    /* FTP archive: CDDIS, IGN, or WHU */
    std::string ftpname = fopt->ftpfrom;
    CString::trim(ftpname);
    CString::ToUpper(ftpname);
    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, crx2rnxfull = fopt->crx2rnxfull, qr = fopt->qr;
    std::string::size_type ipos = fopt->obslist.find_last_of(sep) + 1;
    std::string obslist = fopt->obslist.substr(ipos, fopt->obslist.length() - ipos);
    if ((int)obslist.length() < 9) /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* download all the MGEX observation files */
        std::string url, cutdirs = " --cut-dirs=7 ";
        if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBMD] + "/" + syyyy + "/" + sdoy + "/" + syy + "d";
        else if (ftpname == "IGN")
        {
            url = ftparchive_.IGN[IDX_OBMD] + "/" + syyyy + "/" + sdoy;
            cutdirs = " --cut-dirs=5 ";
        }
        else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_OBMD] + "/" + syyyy + "/" + sdoy + "/" + syy + "d";
        else url = ftparchive_.CDDIS[IDX_OBMD] + "/" + syyyy + "/" + sdoy + "/" + syy + "d";

        /* it is OK for '*.Z' or '*.gz' format */
        std::string crxfile = "*" + syyyy + sdoy + "0000_01D_30S_MO.crx";
        std::string crxxfile = crxfile + ".*";
        std::string cmd = wgetfull + " " + qr + " -nH -A " + crxxfile + cutdirs + url;
        std::system(cmd.c_str());

        /* get the file list */
        std::string suffix = ".crx";
        std::vector<std::string> crxfiles;
        CString::GetFilesAll(subdir, suffix, crxfiles);
        for (size_t i = 0; i < crxfiles.size(); i++)
        {
            if (access(crxfiles[i].c_str(), 0) == 0)
            {
                /* extract it */
                cmd = gzipfull + " -d -f " + crxfiles[i];
                std::system(cmd.c_str());

                crxfile = crxfiles[i].substr(0, (int)crxfiles[i].find_last_of('.'));
                if (access(crxfile.c_str(), 0) == -1) continue;
                std::string rnxfile = crxfile.substr(0, (int)crxfile.find_last_of('.')) + ".rnx";
                /* convert from 'crx' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxfull + " " + crxfile + " -f - > " + rnxfile;
#else          /* for Linux or Mac */
                cmd = "cat " + crxfile + " | " + crx2rnxfull + " -f - > " + rnxfile;
#endif
                std::system(cmd.c_str());

                /* delete 'crx' file */
#ifdef _WIN32   /* for Windows */
                cmd = "del " + crxfile;
#else           /* for Linux or Mac */
                cmd = "rm -rf " + crxfile;
#endif
                std::system(cmd.c_str());

                if (fopt->l2s4obs > 0)
                {
                    std::string site = crxfile.substr(0, 4);
                    CString::ToLower(site);
                    std::string ofile = site + sdoy + "0." + syy + "o";

                    std::string change_filename;
#ifdef _WIN32  /* for Windows */
                    if (fopt->l2s4obs == 1) change_filename = "rename";
                    else if (fopt->l2s4obs == 2) change_filename = "copy";
#else          /* for Linux or Mac */
                    if (fopt->l2s4obs == 1) change_filename = "mv";
                    else if (fopt->l2s4obs == 2) change_filename = "cp";
#endif
                    cmd = change_filename + " " + rnxfile + " " + ofile;
                    std::system(cmd.c_str());
                }
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the MGEX observation file site-by-site */
        if (access(fopt->obslist.c_str(), 0) == 0)
        {
            std::ifstream sitlst(fopt->obslist);
            if (!sitlst.is_open())
            {
                Logger::Trace(TERROR, "*** ERROR(FtpUtil::GetDailyObsMgex): open site.list = " + fopt->obslist +
                    " file failed, please check it");
                return;
            }

            std::string site, sitname;
            while (getline(sitlst, site))
            {
                if (site[0] == '#') continue;
                CString::trim(site);
                if (site.size() < 4) continue;
                sitname = site.substr(0, 4);
                CString::ToLower(sitname);
                std::string ofile = sitname + sdoy + "0." + syy + "o";
                CString::ToUpper(sitname);
                std::string longfile;
                CString::GetFile(subdir, sitname, longfile);
                if (access(ofile.c_str(), 0) == -1 && access(longfile.c_str(), 0) == -1)
                {
                    std::string url, cutdirs = " --cut-dirs=7 ";
                    if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBMD] + "/" + syyyy + "/" + sdoy + "/" + syy + "d";
                    else if (ftpname == "IGN")
                    {
                        url = ftparchive_.IGN[IDX_OBMD] + "/" + syyyy + "/" + sdoy;
                        cutdirs = " --cut-dirs=5 ";
                    }
                    else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_OBMD] + "/" + syyyy + "/" + sdoy + "/" + syy + "d";
                    else url = ftparchive_.CDDIS[IDX_OBMD] + "/" + syyyy + "/" + sdoy + "/" + syy + "d";

                    /* it is OK for '*.Z' or '*.gz' format */
                    CString::ToUpper(sitname);
                    std::string crxfile = sitname + "*" + syyyy + sdoy + "0000_01D_30S_MO.crx";
                    std::string crxxfile = crxfile + ".*";
                    std::string cmd = wgetfull + " " + qr + " -nH -A " + crxxfile + cutdirs + url;
                    std::system(cmd.c_str());

                    std::string sep;
                    sep.push_back((char)FILEPATHSEP);
                    std::string localfile = subdir +sep + ofile;
                    /* extract '*.gz' */
                    std::string crxgzfile = crxfile + ".gz";
                    cmd = gzipfull + " -d -f " + crxgzfile;
                    std::system(cmd.c_str());
                    CString::GetFile(subdir, sitname, crxfile);
                    if (access(crxfile.c_str(), 0) == 0)
                    {
                        longfile = crxfile.substr(0, (int)crxfile.find_last_of('.'));
                        std::string rnxfile = longfile + ".rnx";
#ifdef _WIN32  /* for Windows */
                        cmd = crx2rnxfull + " " + crxfile + " -f - > " + rnxfile;
#else          /* for Linux or Mac */
                        cmd = "cat " + crxfile + " | " + crx2rnxfull + " -f - > " + rnxfile;
#endif
                        std::system(cmd.c_str());

                        /* delete 'crx' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + crxfile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + crxfile;
#endif
                        std::system(cmd.c_str());

                        if (access(rnxfile.c_str(), 0) == 0)
                        {
                            Logger::Trace(TINFO, "*** INFO(FtpUtil::GetDailyObsMgex): successfully download MGEX daily observation file " + ofile);

                            std::string url0 = url + '/' + crxgzfile;
                            if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetDailyObsMgex): " << url0 << "  ->  " <<
                                localfile << "  OK" << std::endl;

                            if (fopt->l2s4obs > 0)
                            {
                                std::string change_filename;
#ifdef _WIN32  /* for Windows */
                                if (fopt->l2s4obs == 1) change_filename = "rename";
                                else if (fopt->l2s4obs == 2) change_filename = "copy";
#else          /* for Linux or Mac */
                                if (fopt->l2s4obs == 1) change_filename = "mv";
                                else if (fopt->l2s4obs == 2) change_filename = "cp";
#endif
                                cmd = change_filename + " " + rnxfile + " " + ofile;
                                std::system(cmd.c_str());
                            }
                        }
                    }
                    else
                    {
                        Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetDailyObsMgex): failed to download MGEX daily observation file " + ofile);

                        std::string url0 = url + '/' + crxgzfile;
                        if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetDailyObsMgex): " << url0 << "  ->  " <<
                            localfile << "  failed" << std::endl;

                        continue;
                    }
                }
                else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetDailyObsMgex): MGEX daily observation file " + ofile + " has existed!");
            }

            /* close 'site.list' */
            sitlst.close();
        }
    }
} /* end of GetDailyObsMgex */

/**
* @brief   : GetHourlyObsMgex - download MGEX RINEX hourly observation (30s) files (long name "crx") according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetHourlyObsMgex(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* creation of sub-directory ('hourly') */
    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string subdir = dir + sep + "hourly";
    if (access(subdir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        std::string cmd = "mkdir " + subdir;
#else           /* for Linux or Mac */
        std::string cmd = "mkdir -p " + subdir;
#endif
        std::system(cmd.c_str());
    }

    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);

    /* FTP archive: CDDIS, IGN, or WHU */
    std::string ftpname = fopt->ftpfrom;
    CString::trim(ftpname);
    CString::ToUpper(ftpname);
    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, crx2rnxfull = fopt->crx2rnxfull, qr = fopt->qr;
    std::string::size_type ipos = fopt->obslist.find_last_of(sep) + 1;
    std::string obslist = fopt->obslist.substr(ipos, fopt->obslist.length() - ipos);
    if ((int)obslist.length() < 9) /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* download all the MGEX observation files */
        for (size_t i = 0; i < fopt->hhobs.size(); i++)
        {
            std::string shh = CString::int2str(fopt->hhobs[i], 2);
            char tmpdir[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            sprintf(tmpdir, "%s%c%s", subdir.c_str(), sep, shh.c_str());
            std::string shhdir = tmpdir;
            if (access(shhdir.c_str(), 0) == -1)
            {
                /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                std::string cmd = "mkdir " + shhdir;
#else           /* for Linux or Mac */
                std::string cmd = "mkdir -p " + shhdir;
#endif
                std::system(cmd.c_str());
            }

            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(shhdir.c_str());
#else           /* for Linux or Mac */
            chdir(shhdir.c_str());
#endif

            std::string url, cutdirs = " --cut-dirs=7 ";
            if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBMH] + "/" + syyyy + "/" + sdoy + "/" + shh;
            else if (ftpname == "IGN")
            {
                url = ftparchive_.IGN[IDX_OBMH] + "/" + syyyy + "/" + sdoy;
                cutdirs = " --cut-dirs=6 ";
            }
            else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_OBMH] + "/" + syyyy + "/" + sdoy + "/" + shh;
            else url = ftparchive_.CDDIS[IDX_OBMH] + "/" + syyyy + "/" + sdoy + "/" + shh;

            /* it is OK for '*.Z' or '*.gz' format */
            std::string crxfile = "*" + syyyy + sdoy + shh + "00_01H_30S_MO.crx";
            std::string crxxfile = crxfile + ".*";
            std::string cmd = wgetfull + " " + qr + " -nH -A " + crxxfile + cutdirs + url;
            std::system(cmd.c_str());

            /* 'a' = 97, 'b' = 98, ... */
            int ii = fopt->hhobs[i] + 97;
            char ch = ii;
            std::string sch;
            sch.push_back(ch);

            /* get the file list */
            std::string suffix = ".crx";
            std::vector<std::string> crxfiles;
            CString::GetFilesAll(shhdir, suffix, crxfiles);
            std::string sitname;
            for (size_t i = 0; i < crxfiles.size(); i++)
            {
                if (access(crxfiles[i].c_str(), 0) == 0)
                {
                    /* extract it */
                    cmd = gzipfull + " -d -f " + crxfiles[i];
                    std::system(cmd.c_str());

                    crxfile = crxfiles[i].substr(0, (int)crxfiles[i].find_last_of('.'));
                    if (access(crxfile.c_str(), 0) == -1) continue;
                    std::string rnxfile = crxfile.substr(0, (int)crxfile.find_last_of('.')) + ".rnx";
                    /* convert from 'crx' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxfull + " " + crxfile + " -f - > " + rnxfile;
#else          /* for Linux or Mac */
                    cmd = "cat " + crxfile + " | " + crx2rnxfull + " -f - > " + rnxfile;
#endif
                    std::system(cmd.c_str());

                    /* delete 'crx' file */
#ifdef _WIN32   /* for Windows */
                    cmd = "del " + crxfile;
#else           /* for Linux or Mac */
                    cmd = "rm -rf " + crxfile;
#endif
                    std::system(cmd.c_str());

                    if (fopt->l2s4obs > 0)
                    {
                        std::string site = crxfile.substr(0, 4);
                        CString::ToLower(site);
                        std::string ofile = site + sdoy + "0." + syy + "o";

                        std::string change_filename;
#ifdef _WIN32  /* for Windows */
                        if (fopt->l2s4obs == 1) change_filename = "rename";
                        else if (fopt->l2s4obs == 2) change_filename = "copy";
#else          /* for Linux or Mac */
                     if (fopt->l2s4obs == 1) change_filename = "mv";
                        else if (fopt->l2s4obs == 2) change_filename = "cp";
#endif
                        cmd = change_filename + " " + rnxfile + " " + ofile;
                        std::system(cmd.c_str());
                    }
                }
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the MGEX observation file site-by-site */
        if (access(fopt->obslist.c_str(), 0) == 0)
        {
            std::ifstream sitlst(fopt->obslist);
            if (!sitlst.is_open())
            {
                Logger::Trace(TERROR, "*** ERROR(FtpUtil::GetHourlyObsMgex): open site.list = " + fopt->obslist +
                    " file failed, please check it");
                return;
            }

            std::string site, sitname;
            while (getline(sitlst, site))
            {
                if (site[0] == '#') continue;
                CString::trim(site);
                if (site.size() < 4) continue;
                sitname = site.substr(0, 4);
                for (size_t i = 0; i < fopt->hhobs.size(); i++)
                {
                    std::string shh = CString::int2str(fopt->hhobs[i], 2);
                    char tmpdir[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpdir, "%s%c%s", subdir.c_str(), sep, shh.c_str());
                    std::string shhdir = tmpdir;
                    if (access(shhdir.c_str(), 0) == -1)
                    {
                        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                        std::string cmd = "mkdir " + shhdir;
#else           /* for Linux or Mac */
                        std::string cmd = "mkdir -p " + shhdir;
#endif
                        std::system(cmd.c_str());
                    }

                    /* change directory */
#ifdef _WIN32   /* for Windows */
                    _chdir(shhdir.c_str());
#else           /* for Linux or Mac */
                    chdir(shhdir.c_str());
#endif

                    /* 'a' = 97, 'b' = 98, ... */
                    int ii = fopt->hhobs[i] + 97;
                    char ch = ii;
                    std::string sch;
                    sch.push_back(ch);
                    CString::ToLower(sitname);
                    std::string ofile = sitname + sdoy + sch + "." + syy + "o";
                    std::string dfile = sitname + sdoy + sch + "." + syy + "d";
                    if (access(ofile.c_str(), 0) == -1 && access(dfile.c_str(), 0) == -1)
                    {
                        std::string url, cutdirs = " --cut-dirs=7 ";
                        if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBMH] + "/" + syyyy + "/" + sdoy + "/" + shh;
                        else if (ftpname == "IGN")
                        {
                            url = ftparchive_.IGN[IDX_OBMH] + "/" + syyyy + "/" + sdoy;
                            cutdirs = " --cut-dirs=6 ";
                        }
                        else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_OBMH] + "/" + syyyy + "/" + sdoy + "/" + shh;
                        else url = ftparchive_.CDDIS[IDX_OBMH] + "/" + syyyy + "/" + sdoy + "/" + shh;

                        /* it is OK for '*.Z' or '*.gz' format */
                        CString::ToUpper(sitname);
                        std::string crxfile = sitname + "*" + syyyy + sdoy + shh + "00_01H_30S_MO.crx";
                        std::string crxxfile = crxfile + ".*";
                        std::string cmd = wgetfull + " " + qr + " -nH -A " + crxxfile + cutdirs + url;
                        std::system(cmd.c_str());

                        /* extract '*.gz' */
                        std::string crxgzFile = crxfile + ".gz";
                        cmd = gzipfull + " -d -f " + crxgzFile;
                        std::system(cmd.c_str());
                        std::string change_filename;
#ifdef _WIN32  /* for Windows */
                        change_filename = "rename";
#else          /* for Linux or Mac */
                        change_filename = "mv";
#endif
                        cmd = change_filename + " " + crxfile + " " + dfile;
                        std::system(cmd.c_str());
                        bool isgz = true;
                        std::string crxzFile;
                        char tmpfile[MAXSTRPATH] = { '\0' };
                        char sep = (char)FILEPATHSEP;
                        sprintf(tmpfile, "%s%c%s", shhdir.c_str(), sep, ofile.c_str());
                        std::string localfile = tmpfile;
                        if (access(dfile.c_str(), 0) == -1)
                        {
                            /* extract '*.Z' */
                            crxzFile = crxfile + ".Z";
                            cmd = gzipfull + " -d -f " + crxzFile;
                            std::system(cmd.c_str());

                            cmd = change_filename + " " + crxfile + " " + dfile;
                            std::system(cmd.c_str());
                            if (access(dfile.c_str(), 0) == -1)
                            {
                                Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetDailyObsMgex): failed to download MGEX daily observation file " + ofile);

                                std::string url0 = url + '/' + crxxfile;
                                if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::): " << url0 << "  ->  " << localfile <<
                                    "  failed" << std::endl;

                                continue;
                            }
                        }

#ifdef _WIN32  /* for Windows */
                        cmd = crx2rnxfull + " " + dfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                        cmd = "cat " + dfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                        std::system(cmd.c_str());

                        if (access(ofile.c_str(), 0) == 0)
                        {
                            Logger::Trace(TINFO, "*** INFO(FtpUtil::GetHourlyObsMgex): successfully download MGEX hourly observation file " + ofile);

                            if (isgz) crxxfile = crxgzFile;
                            else crxxfile = crxzFile;
                            std::string url0 = url + '/' + crxxfile;
                            if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetHourlyObsMgex): " << url0 << "  ->  " <<
                                localfile << "  OK" << std::endl;

                            /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                            cmd = "del " + dfile;
#else           /* for Linux or Mac */
                            cmd = "rm -rf " + dfile;
#endif
                            std::system(cmd.c_str());
                        }
                    }
                    else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetHourlyObsMgex): MGEX hourly observation file " +
                        ofile + " or " + dfile + " has existed!");
                }
            }

            /* close 'site.list' */
            sitlst.close();
        }
    }
} /* end of GetHourlyObsMgex */

/**
* @brief   : GetHrObsMgex - download MGEX RINEX high-rate observation (1s) files (long name "crx") according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetHrObsMgex(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* creation of sub-directory ('highrate') */
    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string subdir = dir + sep + "highrate";
    if (access(subdir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        std::string cmd = "mkdir " + subdir;
#else           /* for Linux or Mac */
        std::string cmd = "mkdir -p " + subdir;
#endif
        std::system(cmd.c_str());
    }

    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);

    /* FTP archive: CDDIS, IGN, or WHU */
    std::string ftpname = fopt->ftpfrom;
    CString::trim(ftpname);
    CString::ToUpper(ftpname);
    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, crx2rnxfull = fopt->crx2rnxfull, qr = fopt->qr;
    std::string::size_type ipos = fopt->obslist.find_last_of(sep) + 1;
    std::string obslist = fopt->obslist.substr(ipos, fopt->obslist.length() - ipos);
    if ((int)obslist.length() < 9) /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* download all the MGEX observation files */
        for (size_t i = 0; i < fopt->hhobs.size(); i++)
        {
            std::string shh = CString::int2str(fopt->hhobs[i], 2);
            char tmpdir[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            sprintf(tmpdir, "%s%c%s", subdir.c_str(), sep, shh.c_str());
            std::string shhdir = tmpdir;
            if (access(shhdir.c_str(), 0) == -1)
            {
                /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                std::string cmd = "mkdir " + shhdir;
#else           /* for Linux or Mac */
                std::string cmd = "mkdir -p " + shhdir;
#endif
                std::system(cmd.c_str());
            }

            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(shhdir.c_str());
#else           /* for Linux or Mac */
            chdir(shhdir.c_str());
#endif

            std::string url, cutdirs = " --cut-dirs=8 ";
            if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBMHR] + "/" + syyyy + "/" + sdoy + "/" + syy + "d" + "/" + shh;
            else if (ftpname == "IGN")
            {
                url = ftparchive_.IGN[IDX_OBMHR] + "/" + syyyy + "/" + sdoy;
                cutdirs = " --cut-dirs=6 ";
            }
            else if (ftpname == "WHU")
            {
                url = ftparchive_.WHU[IDX_OBMHR] + "/" + syyyy + "/" + sdoy + "/" + syy + "d" + "/" + shh;
                cutdirs = " --cut-dirs=6 ";
            }
            else url = ftparchive_.CDDIS[IDX_OBMHR] + "/" + syyyy + "/" + sdoy + "/" + syy + "d" + "/" + shh;

            /* it is OK for '*.Z' or '*.gz' format */
            std::string crxfile = "*_15M_01S_MO.crx";
            std::string crxxfile = crxfile + ".*";
            std::string cmd = wgetfull + " " + qr + " -nH -A " + crxxfile + cutdirs + url;
            std::system(cmd.c_str());

            /* 'a' = 97, 'b' = 98, ... */
            int ii = fopt->hhobs[i] + 97;
            char ch = ii;
            std::string sch;
            sch.push_back(ch);

            std::vector<std::string> smin = { "00", "15", "30", "45" };
            for (size_t i = 0; i < smin.size(); i++)
            {
                /* get the file list */
                std::string suffix = smin[i] + "_15M_01S_MO.crx";
                std::vector<std::string> crxfiles;
                CString::GetFilesAll(shhdir, suffix, crxfiles);
                std::string sitname;
                for (size_t j = 0; j < crxfiles.size(); j++)
                {
                    if (access(crxfiles[j].c_str(), 0) == 0)
                    {
                        /* extract it */
                        cmd = gzipfull + " -d -f " + crxfiles[j];
                        std::system(cmd.c_str());

                        crxfile = crxfiles[j].substr(0, (int)crxfiles[j].find_last_of('.'));
                        if (access(crxfile.c_str(), 0) == -1) continue;
                        std::string site = crxfiles[j].substr(0, 4);
                        CString::ToLower(site);
                        std::string ofile = site + sdoy + sch + smin[i] +  "." + syy + "o";
                        /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                        cmd = crx2rnxfull + " " + crxfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                        cmd = "cat " + crxfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                        std::system(cmd.c_str());

                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + crxfile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + crxfile;
#endif
                        std::system(cmd.c_str());
                    }
                }
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the MGEX observation file site-by-site */
        if (access(fopt->obslist.c_str(), 0) == 0)
        {
            std::ifstream sitlst(fopt->obslist);
            if (!sitlst.is_open())
            {
                Logger::Trace(TERROR, "*** ERROR(FtpUtil::GetHrObsMgex): open site.list = " + fopt->obslist +
                    " file failed, please check it");
                return;
            }

            std::string site, sitname;
            while (getline(sitlst, site))
            {
                if (site[0] == '#') continue;
                CString::trim(site);
                if (site.size() < 4) continue;
                sitname = site.substr(0, 4);
                for (size_t i = 0; i < fopt->hhobs.size(); i++)
                {
                    std::string shh = CString::int2str(fopt->hhobs[i], 2);
                    char tmpdir[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpdir, "%s%c%s", subdir.c_str(), sep, shh.c_str());
                    std::string shhdir = tmpdir;
                    if (access(shhdir.c_str(), 0) == -1)
                    {
                        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                        std::string cmd = "mkdir " + shhdir;
#else           /* for Linux or Mac */
                        std::string cmd = "mkdir -p " + shhdir;
#endif
                        std::system(cmd.c_str());
                    }

                    /* change directory */
#ifdef _WIN32   /* for Windows */
                    _chdir(shhdir.c_str());
#else           /* for Linux or Mac */
                    chdir(shhdir.c_str());
#endif

                    /* 'a' = 97, 'b' = 98, ... */
                    int ii = fopt->hhobs[i] + 97;
                    char ch = ii;
                    std::string sch;
                    sch.push_back(ch);

                    std::vector<std::string> smin = { "00", "15", "30", "45" };
                    for (size_t i = 0; i < smin.size(); i++)
                    {
                        CString::ToLower(sitname);
                        std::string ofile = sitname + sdoy + sch + smin[i] + "." + syy + "o";
                        std::string dfile = sitname + sdoy + sch + smin[i] + "." + syy + "d";
                        if (access(ofile.c_str(), 0) == -1 && access(dfile.c_str(), 0) == -1)
                        {
                            std::string url, cutdirs = " --cut-dirs=8 ";
                            if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBMHR] + "/" + syyyy + "/" + sdoy + "/" +
                                syy + "d" + "/" + shh;
                            else if (ftpname == "IGN")
                            {
                                url = ftparchive_.IGN[IDX_OBMHR] + "/" + syyyy + "/" + sdoy;
                                cutdirs = " --cut-dirs=6 ";
                            }
                            else if (ftpname == "WHU")
                            {
                                url = ftparchive_.WHU[IDX_OBMHR] + "/" + syyyy + "/" + sdoy + "/" + syy + "d" + "/" + shh;
                                cutdirs = " --cut-dirs=6 ";
                            }
                            else url = ftparchive_.CDDIS[IDX_OBMHR] + "/" + syyyy + "/" + sdoy + "/" +
                                syy + "d" + "/" + shh;

                            /* it is OK for '*.Z' or '*.gz' format */
                            CString::ToUpper(sitname);
                            std::string crxfile = sitname + "*" + syyyy + sdoy + shh + smin[i] + "_15M_01S_MO.crx";
                            std::string crxxfile = crxfile + ".*";
                            std::string cmd = wgetfull + " " + qr + " -nH -A " + crxxfile + cutdirs + url;
                            std::system(cmd.c_str());

                            /* extract '*.gz' */
                            std::string crxgzFile = crxfile + ".gz";
                            cmd = gzipfull + " -d -f " + crxgzFile;
                            std::system(cmd.c_str());
                            std::string change_filename;
#ifdef _WIN32  /* for Windows */
                            change_filename = "rename";
#else          /* for Linux or Mac */
                            change_filename = "mv";
#endif
                            cmd = change_filename + " " + crxfile + " " + dfile;
                            std::system(cmd.c_str());
                            bool isgz = true;
                            std::string crxzFile;
                            char tmpfile[MAXSTRPATH] = { '\0' };
                            char sep = (char)FILEPATHSEP;
                            sprintf(tmpfile, "%s%c%s", shhdir.c_str(), sep, ofile.c_str());
                            std::string localfile = tmpfile;
                            if (access(dfile.c_str(), 0) == -1)
                            {
                                /* extract '*.Z' */
                                crxzFile = crxfile + ".Z";
                                cmd = gzipfull + " -d -f " + crxzFile;
                                std::system(cmd.c_str());

                                cmd = change_filename + " " + crxfile + " " + dfile;
                                std::system(cmd.c_str());
                                isgz = false;
                                if (access(dfile.c_str(), 0) == -1)
                                {
                                    Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetHrObsMgex): failed to download MGEX high-rate observation file " + ofile);

                                    std::string url0 = url + '/' + crxxfile;
                                    if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetHrObsMgex): " << url0 << "  ->  " <<
                                        localfile << "  failed" << std::endl;

                                    continue;
                                }
                            }

#ifdef _WIN32  /* for Windows */
                            cmd = crx2rnxfull + " " + dfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                            cmd = "cat " + dfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                            std::system(cmd.c_str());

                            if (access(ofile.c_str(), 0) == 0)
                            {
                                Logger::Trace(TINFO, "*** INFO(FtpUtil::GetHrObsMgex): successfully download MGEX high-rate observation file " + ofile);

                                if (isgz) crxxfile = crxgzFile;
                                else crxxfile = crxzFile;
                                std::string url0 = url + '/' + crxxfile;
                                if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetHrObsMgex): " << url0 << "  ->  " <<
                                    localfile << "  OK" << std::endl;

                                /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                                cmd = "del " + dfile;
#else           /* for Linux or Mac */
                                cmd = "rm -rf " + dfile;
#endif
                                std::system(cmd.c_str());
                            }
                        }
                        else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetHrObsMgex): MGEX high-rate observation file " +
                            ofile + " or " + dfile + " has existed!");
                    }
                }
            }

            /* close 'site.list' */
            sitlst.close();
        }
    }
} /* end of GetHrObsMgex */

/**
* @brief   : GetDailyObsIgm - to download IGS (short name "d") and MGEX RINEX daily observation (30s) files (long name "crx")
*              simutaneously according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetDailyObsIgm(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* creation of sub-directory ('daily') */
    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string subdir = dir + sep + "daily";
    if (access(subdir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        std::string cmd = "mkdir " + subdir;
#else           /* for Linux or Mac */
        std::string cmd = "mkdir -p " + subdir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subdir.c_str());
#else           /* for Linux or Mac */
    chdir(subdir.c_str());
#endif

    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);

    /* FTP archive: CDDIS, IGN, or WHU */
    std::string ftpname = fopt->ftpfrom;
    CString::trim(ftpname);
    CString::ToUpper(ftpname);
    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, crx2rnxfull = fopt->crx2rnxfull, qr = fopt->qr;
    std::string::size_type ipos = fopt->obslist.find_last_of(sep) + 1;
    std::string obslist = fopt->obslist.substr(ipos, fopt->obslist.length() - ipos);
    if ((int)obslist.length() < 9) /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* download all the MGEX observation files */
        std::string url, cutdirs = " --cut-dirs=7 ";
        if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBMD] + "/" + syyyy + "/" + sdoy + "/" + syy + "d";
        else if (ftpname == "IGN")
        {
            url = ftparchive_.IGN[IDX_OBMD] + "/" + syyyy + "/" + sdoy;
            cutdirs = " --cut-dirs=5 ";
        }
        else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_OBMD] + "/" + syyyy + "/" + sdoy + "/" + syy + "d";
        else url = ftparchive_.CDDIS[IDX_OBMD] + "/" + syyyy + "/" + sdoy + "/" + syy + "d";

        /* it is OK for '*.Z' or '*.gz' format */
        std::string crxfile = "*" + syyyy + sdoy + "0000_01D_30S_MO.crx";
        std::string crxxfile = crxfile + ".*";
        std::string cmd = wgetfull + " " + qr + " -nH -A " + crxxfile + cutdirs + url;
        std::system(cmd.c_str());

        /* get the file list */
        std::string suffix = ".crx";
        std::vector<std::string> crxfiles;
        CString::GetFilesAll(subdir, suffix, crxfiles);
        std::string sitname;
        for (size_t i = 0; i < crxfiles.size(); i++)
        {
            if (access(crxfiles[i].c_str(), 0) == 0)
            {
                /* extract it */
                cmd = gzipfull + " -d -f " + crxfiles[i];
                std::system(cmd.c_str());

                crxfile = crxfiles[i].substr(0, (int)crxfiles[i].find_last_of('.'));
                if (access(crxfile.c_str(), 0) == -1) continue;
                std::string site = crxfiles[i].substr(0, 4);
                CString::ToLower(site);
                std::string ofile = site + sdoy + "0." + syy + "o";
                /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxfull + " " + crxfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                cmd = "cat " + crxfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                std::system(cmd.c_str());

                /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                cmd = "del " + crxfile;
#else           /* for Linux or Mac */
                cmd = "rm -rf " + crxfile;
#endif
                std::system(cmd.c_str());
            }
        }

        /* download all the IGS observation files */
        if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBSD] + "/" + syyyy + "/" + sdoy + "/" + syy + "d";
        else if (ftpname == "IGN")
        {
            url = ftparchive_.IGN[IDX_OBSD] + "/" + syyyy + "/" + sdoy;
            cutdirs = " --cut-dirs=5 ";
        }
        else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_OBSD] + "/" + syyyy + "/" + sdoy + "/" + syy + "d";
        else url = ftparchive_.CDDIS[IDX_OBSD] + "/" + syyyy + "/" + sdoy + "/" + syy + "d";

        /* it is OK for '*.Z' or '*.gz' format */
        std::string dfile = "*" + sdoy + "0." + syy + "d";
        std::string dxfile = dfile + ".*";
        cmd = wgetfull + " " + qr + " -nH -A " + dxfile + cutdirs + url;
        std::system(cmd.c_str());

        /* get the file list */
        suffix = "." + syy + "d";
        std::vector<std::string> dfiles;
        CString::GetFilesAll(subdir, suffix, dfiles);
        for (size_t i = 0; i < dfiles.size(); i++)
        {
            if (access(dfiles[i].c_str(), 0) == 0)
            {
                /* extract it */
                cmd = gzipfull + " -d -f " + dfiles[i];
                std::system(cmd.c_str());

                std::string site = dfiles[i].substr(0, 4);
                CString::ToLower(site);
                dfile = site + sdoy + "0." + syy + "d";
                if (access(dfile.c_str(), 0) == -1) continue;
                std::string ofile = site + sdoy + "0." + syy + "o";
                if (access(ofile.c_str(), 0) == 0)
                {
                    /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                    cmd = "del " + dfile;
#else           /* for Linux or Mac */
                    cmd = "rm -rf " + dfile;
#endif
                    std::system(cmd.c_str());

                    continue;
                }
                /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxfull + " " + dfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                cmd = "cat " + dfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                std::system(cmd.c_str());

                /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                cmd = "del " + dfile;
#else           /* for Linux or Mac */
                cmd = "rm -rf " + dfile;
#endif
                std::system(cmd.c_str());
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        if (access(fopt->obslist.c_str(), 0) == 0)
        {
            std::ifstream sitlst(fopt->obslist);
            if (!sitlst.is_open())
            {
                Logger::Trace(TERROR, "*** ERROR(FtpUtil::GetDailyObsIgm): open site.list = " + fopt->obslist +
                    " file failed, please check it");
                return;
            }

            std::string site, sitname;
            while (getline(sitlst, site))
            {
                if (site[0] == '#') continue;
                CString::trim(site);
                if (site.size() < 4) continue;
                sitname = site.substr(0, 4);
                CString::ToLower(sitname);
                std::string ofile = sitname + sdoy + "0." + syy + "o";
                std::string dfile = sitname + sdoy + "0." + syy + "d";
                if (access(ofile.c_str(), 0) == -1 && access(dfile.c_str(), 0) == -1)
                {
                    /* download the MGEX observation file site-by-site */
                    std::string url, cutdirs = " --cut-dirs=7 ";
                    if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBMD] + "/" +
                        syyyy + "/" + sdoy + "/" + syy + "d";
                    else if (ftpname == "IGN")
                    {
                        url = ftparchive_.IGN[IDX_OBMD] + "/" + syyyy + "/" + sdoy;
                        cutdirs = " --cut-dirs=5 ";
                    }
                    else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_OBMD] + "/" +
                        syyyy + "/" + sdoy + "/" + syy + "d";
                    else url = ftparchive_.CDDIS[IDX_OBMD] + "/" + syyyy + "/" + sdoy + "/" + syy + "d";

                    /* it is OK for '*.Z' or '*.gz' format */
                    CString::ToUpper(sitname);
                    std::string crxfile = sitname + "*" + syyyy + sdoy + "0000_01D_30S_MO.crx";
                    std::string crxxfile = crxfile + ".*";
                    std::string cmd = wgetfull + " " + qr + " -nH -A " + crxxfile + cutdirs + url;
                    std::system(cmd.c_str());

                    /* extract '*.gz' */
                    std::string crxgzFile = crxfile + ".gz";
                    cmd = gzipfull + " -d -f " + crxgzFile;
                    std::system(cmd.c_str());
                    std::string change_filename;
#ifdef _WIN32  /* for Windows */
                    change_filename = "rename";
#else          /* for Linux or Mac */
                    change_filename = "mv";
#endif
                    cmd = change_filename + " " + crxfile + " " + dfile;
                    std::system(cmd.c_str());

                    char tmpfile[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpfile, "%s%c%s", subdir.c_str(), sep, ofile.c_str());
                    std::string localfile = tmpfile;
                    if (access(dfile.c_str(), 0) == -1)
                    {
                        /* extract '*.Z' */
                        std::string crxzFile = crxfile + ".Z";
                        cmd = gzipfull + " -d -f " + crxzFile;
                        std::system(cmd.c_str());

                        cmd = change_filename + " " + crxfile + " " + dfile;
                        std::system(cmd.c_str());
                        if (access(dfile.c_str(), 0) == -1)
                        {
                            /* download the IGS observation file site-by-site */
                            if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBSD] + "/" +
                                syyyy + "/" + sdoy + "/" + syy + "d";
                            else if (ftpname == "IGN")
                            {
                                url = ftparchive_.IGN[IDX_OBSD] + "/" + syyyy + "/" + sdoy;
                                cutdirs = " --cut-dirs=5 ";
                            }
                            else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_OBSD] + "/" +
                                syyyy + "/" + sdoy + "/" + syy + "d";
                            else url = ftparchive_.CDDIS[IDX_OBSD] + "/" + syyyy + "/" + sdoy + "/" + syy + "d";

                            /* it is OK for '*.Z' or '*.gz' format */
                            std::string dxfile = dfile + ".*";
                            std::string cmd = wgetfull + " " + qr + " -nH -A " + dxfile + cutdirs + url;
                            std::system(cmd.c_str());

                            std::string dgzfile = dfile + ".gz", dzfile = dfile + ".Z";
                            if (access(dgzfile.c_str(), 0) == 0)
                            {
                                /* extract '*.gz' */
                                cmd = gzipfull + " -d -f " + dgzfile;
                                std::system(cmd.c_str());

                                if (access(dfile.c_str(), 0) == 0 && access(dzfile.c_str(), 0) == 0)
                                {
                                    /* delete '*.Z' file */
#ifdef _WIN32   /* for Windows */
                                    cmd = "del " + dzfile;
#else           /* for Linux or Mac */
                                    cmd = "rm -rf " + dzfile;
#endif
                                    std::system(cmd.c_str());
                                }
                            }
                            if (access(dfile.c_str(), 0) == -1 && access(dzfile.c_str(), 0) == 0)
                            {
                                /* extract '*.Z' */
                                cmd = gzipfull + " -d -f " + dzfile;
                                std::system(cmd.c_str());
                            }
                            if (access(dfile.c_str(), 0) == -1)
                            {
                                Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetDailyObsIgm): failed to download IGM daily observation file " + ofile);

                                std::string url0 = url + '/' + crxxfile;
                                if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetDailyObsIgm): " << url0 << "  ->  " <<
                                    localfile << "  failed" << std::endl;
                                url0 = url + '/' + dxfile;
                                if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetDailyObsIgm): " << url0 << "  ->  " <<
                                    localfile << "  failed" << std::endl;

                                continue;
                            }
                        }
                    }

#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxfull + " " + dfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                    cmd = "cat " + dfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                    std::system(cmd.c_str());

                    if (access(ofile.c_str(), 0) == 0)
                    {
                        Logger::Trace(TINFO, "*** INFO(FtpUtil::GetDailyObsIgm): successfully download IGM daily observation file " + ofile);

                        if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetDailyObsIgm): " << ofile << "  ->  " <<
                            localfile << "  OK" << std::endl;

                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + dfile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + dfile;
#endif
                        std::system(cmd.c_str());
                    }
                }
                else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetDailyObsIgm): IGM daily observation file " + ofile + " or " +
                    dfile + " has existed!");
            }

            /* close 'site.list' */
            sitlst.close();
        }
    }
} /* end of GetDailyObsIgm */

/**
* @brief   : GetHourlyObsIgm  - to download IGS (short name "d") and MGEX RINEX hourly observation (30s) files (long name "crx")
*              simutaneously according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetHourlyObsIgm(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* creation of sub-directory ('hourly') */
    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string subdir = dir + sep + "hourly";
    if (access(subdir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        std::string cmd = "mkdir " + subdir;
#else           /* for Linux or Mac */
        std::string cmd = "mkdir -p " + subdir;
#endif
        std::system(cmd.c_str());
    }

    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);

    /* FTP archive: CDDIS, IGN, or WHU */
    std::string ftpname = fopt->ftpfrom;
    CString::trim(ftpname);
    CString::ToUpper(ftpname);
    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, crx2rnxfull = fopt->crx2rnxfull, qr = fopt->qr;
    std::string::size_type ipos = fopt->obslist.find_last_of(sep) + 1;
    std::string obslist = fopt->obslist.substr(ipos, fopt->obslist.length() - ipos);
    if ((int)obslist.length() < 9) /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        for (size_t i = 0; i < fopt->hhobs.size(); i++)
        {
            std::string shh = CString::int2str(fopt->hhobs[i], 2);
            char tmpdir[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            sprintf(tmpdir, "%s%c%s", subdir.c_str(), sep, shh.c_str());
            std::string shhdir = tmpdir;
            if (access(shhdir.c_str(), 0) == -1)
            {
                /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                std::string cmd = "mkdir " + shhdir;
#else           /* for Linux or Mac */
                std::string cmd = "mkdir -p " + shhdir;
#endif
                std::system(cmd.c_str());
            }

            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(shhdir.c_str());
#else           /* for Linux or Mac */
            chdir(shhdir.c_str());
#endif

            /* download all the MGEX observation files */
            std::string url, cutdirs = " --cut-dirs=7 ";
            if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBMH] + "/" +
                syyyy + "/" + sdoy + "/" + shh;
            else if (ftpname == "IGN")
            {
                url = ftparchive_.IGN[IDX_OBMH] + "/" + syyyy + "/" + sdoy;
                cutdirs = " --cut-dirs=6 ";
            }
            else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_OBMH] + "/" +
                syyyy + "/" + sdoy + "/" + shh;
            else url = ftparchive_.CDDIS[IDX_OBMH] + "/" + syyyy + "/" + sdoy + "/" + shh;

            /* it is OK for '*.Z' or '*.gz' format */
            std::string crxfile = "*" + syyyy + sdoy + shh + "00_01H_30S_MO.crx";
            std::string crxxfile = crxfile + ".*";
            std::string cmd = wgetfull + " " + qr + " -nH -A " + crxxfile + cutdirs + url;
            std::system(cmd.c_str());

            /* 'a' = 97, 'b' = 98, ... */
            int ii = fopt->hhobs[i] + 97;
            char ch = ii;
            std::string sch;
            sch.push_back(ch);

            /* get the file list */
            std::string suffix = ".crx";
            std::vector<std::string> crxfiles;
            CString::GetFilesAll(shhdir, suffix, crxfiles);
            std::string sitname;
            for (size_t i = 0; i < crxfiles.size(); i++)
            {
                if (access(crxfiles[i].c_str(), 0) == 0)
                {
                    /* extract it */
                    cmd = gzipfull + " -d -f " + crxfiles[i];
                    std::system(cmd.c_str());

                    crxfile = crxfiles[i].substr(0, (int)crxfiles[i].find_last_of('.'));
                    if (access(crxfile.c_str(), 0) == -1) continue;
                    std::string site = crxfiles[i].substr(0, 4);
                    CString::ToLower(site);
                    std::string ofile = site + sdoy + sch + "." + syy + "o";
                    /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxfull + " " + crxfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                    cmd = "cat " + crxfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                    std::system(cmd.c_str());

                    /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                    cmd = "del " + crxfile;
#else           /* for Linux or Mac */
                    cmd = "rm -rf " + crxfile;
#endif
                    std::system(cmd.c_str());
                }
            }

            /* download all the IGS observation files */
            if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBSH] + "/" +
                syyyy + "/" + sdoy + "/" + shh;
            else if (ftpname == "IGN")
            {
                url = ftparchive_.IGN[IDX_OBSH] + "/" + syyyy + "/" + sdoy;
                cutdirs = " --cut-dirs=6 ";
            }
            else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_OBSH] + "/" +
                syyyy + "/" + sdoy + "/" + shh;
            else url = ftparchive_.CDDIS[IDX_OBSH] + "/" + syyyy + "/" + sdoy + "/" + shh;

            /* it is OK for '*.Z' or '*.gz' format */
            std::string dfile = "*" + sdoy + sch + "." + syy + "d";
            std::string dxfile = dfile + ".*";
            cmd = wgetfull + " " + qr + " -nH -A " + dxfile + cutdirs + url;
            std::system(cmd.c_str());

            /* get the file list */
            suffix = "." + syy + "d";
            std::vector<std::string> dfiles;
            CString::GetFilesAll(shhdir, suffix, dfiles);
            for (size_t i = 0; i < dfiles.size(); i++)
            {
                if (access(dfiles[i].c_str(), 0) == 0)
                {
                    /* extract it */
                    cmd = gzipfull + " -d -f " + dfiles[i];
                    std::system(cmd.c_str());

                    std::string site = dfiles[i].substr(0, 4);
                    CString::ToLower(site);
                    dfile = site + sdoy + sch + "." + syy + "d";
                    if (access(dfile.c_str(), 0) == -1) continue;
                    std::string ofile = site + sdoy + sch + "." + syy + "o";
                    if (access(ofile.c_str(), 0) == 0)
                    {
                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + dfile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + dfile;
#endif
                        std::system(cmd.c_str());

                        continue;
                    }
                    /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxfull + " " + dfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                    cmd = "cat " + dfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                    std::system(cmd.c_str());

                    /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                    cmd = "del " + dfile;
#else           /* for Linux or Mac */
                    cmd = "rm -rf " + dfile;
#endif
                    std::system(cmd.c_str());
                }
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        if (access(fopt->obslist.c_str(), 0) == 0)
        {
            std::ifstream sitlst(fopt->obslist);
            if (!sitlst.is_open())
            {
                Logger::Trace(TERROR, "*** ERROR(FtpUtil::GetHourlyObsIgm): open site.list = " + fopt->obslist +
                    " file failed, please check it");
                return;
            }

            std::string site, sitname;
            while (getline(sitlst, site))
            {
                if (site[0] == '#') continue;
                CString::trim(site);
                if (site.size() < 4) continue;
                sitname = site.substr(0, 4);
                for (size_t i = 0; i < fopt->hhobs.size(); i++)
                {
                    std::string shh = CString::int2str(fopt->hhobs[i], 2);
                    char tmpdir[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpdir, "%s%c%s", subdir.c_str(), sep, shh.c_str());
                    std::string shhdir = tmpdir;
                    if (access(shhdir.c_str(), 0) == -1)
                    {
                        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                        std::string cmd = "mkdir " + shhdir;
#else           /* for Linux or Mac */
                        std::string cmd = "mkdir -p " + shhdir;
#endif
                        std::system(cmd.c_str());
                    }

                    /* change directory */
#ifdef _WIN32   /* for Windows */
                    _chdir(shhdir.c_str());
#else           /* for Linux or Mac */
                    chdir(shhdir.c_str());
#endif

                    /* 'a' = 97, 'b' = 98, ... */
                    int ii = fopt->hhobs[i] + 97;
                    char ch = ii;
                    std::string sch;
                    sch.push_back(ch);
                    CString::ToLower(sitname);
                    std::string ofile = sitname + sdoy + sch + "." + syy + "o";
                    std::string dfile = sitname + sdoy + sch + "." + syy + "d";
                    if (access(ofile.c_str(), 0) == -1 && access(dfile.c_str(), 0) == -1)
                    {
                        /* download the MGEX observation file site-by-site */
                        std::string url, cutdirs = " --cut-dirs=7 ";
                        if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBMH] + "/" +
                            syyyy + "/" + sdoy + "/" + shh;
                        else if (ftpname == "IGN")
                        {
                            url = ftparchive_.IGN[IDX_OBMH] + "/" + syyyy + "/" + sdoy;
                            cutdirs = " --cut-dirs=6 ";
                        }
                        else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_OBMH] + "/" +
                            syyyy + "/" + sdoy + "/" + shh;
                        else url = ftparchive_.CDDIS[IDX_OBMH] + "/" + syyyy + "/" + sdoy + "/" + shh;

                        /* it is OK for '*.Z' or '*.gz' format */
                        CString::ToUpper(sitname);
                        std::string crxfile = sitname + "*" + syyyy + sdoy + shh + "00_01H_30S_MO.crx";
                        std::string crxxfile = crxfile + ".*";
                        std::string cmd = wgetfull + " " + qr + " -nH -A " + crxxfile + cutdirs + url;
                        std::system(cmd.c_str());

                        /* extract '*.gz' */
                        std::string crxgzFile = crxfile + ".gz";
                        cmd = gzipfull + " -d -f " + crxgzFile;
                        std::system(cmd.c_str());
                        std::string change_filename;
#ifdef _WIN32  /* for Windows */
                        change_filename = "rename";
#else          /* for Linux or Mac */
                        change_filename = "mv";
#endif
                        cmd = change_filename + " " + crxfile + " " + dfile;
                        std::system(cmd.c_str());

                        char tmpfile[MAXSTRPATH] = { '\0' };
                        char sep = (char)FILEPATHSEP;
                        sprintf(tmpfile, "%s%c%s", shhdir.c_str(), sep, ofile.c_str());
                        std::string localfile = tmpfile;
                        if (access(dfile.c_str(), 0) == -1)
                        {
                            /* extract '*.Z' */
                            std::string crxzFile = crxfile + ".Z";
                            cmd = gzipfull + " -d -f " + crxzFile;
                            std::system(cmd.c_str());

                            cmd = change_filename + " " + crxfile + " " + dfile;
                            std::system(cmd.c_str());
                            if (access(dfile.c_str(), 0) == -1)
                            {
                                /* download the IGS observation file site-by-site */
                                if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBSH] + "/" +
                                    syyyy + "/" + sdoy + "/" + shh;
                                else if (ftpname == "IGN")
                                {
                                    url = ftparchive_.IGN[IDX_OBSH] + "/" + syyyy + "/" + sdoy;
                                    cutdirs = " --cut-dirs=6 ";
                                }
                                else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_OBSH] + "/" +
                                    syyyy + "/" + sdoy + "/" + shh;
                                else url = ftparchive_.CDDIS[IDX_OBSH] + "/" + syyyy + "/" + sdoy + "/" + shh;

                                /* it is OK for '*.Z' or '*.gz' format */
                                std::string dxfile = dfile + ".*";
                                std::string cmd = wgetfull + " " + qr + " -nH -A " + dxfile + cutdirs + url;
                                std::system(cmd.c_str());

                                std::string dgzfile = dfile + ".gz", dzfile = dfile + ".Z";
                                if (access(dgzfile.c_str(), 0) == 0)
                                {
                                    /* extract '*.gz' */
                                    cmd = gzipfull + " -d -f " + dgzfile;
                                    std::system(cmd.c_str());

                                    if (access(dfile.c_str(), 0) == 0 && access(dzfile.c_str(), 0) == 0)
                                    {
                                        /* delete '*.Z' file */
#ifdef _WIN32   /* for Windows */
                                        cmd = "del " + dzfile;
#else           /* for Linux or Mac */
                                        cmd = "rm -rf " + dzfile;
#endif
                                        std::system(cmd.c_str());
                                    }
                                }
                                if (access(dfile.c_str(), 0) == -1 && access(dzfile.c_str(), 0) == 0)
                                {
                                    /* extract '*.Z' */
                                    cmd = gzipfull + " -d -f " + dzfile;
                                    std::system(cmd.c_str());
                                }
                                if (access(dfile.c_str(), 0) == -1)
                                {
                                    Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetHourlyObsIgm): failed to download IGM Hourly observation file " + ofile);

                                    std::string url0 = url + '/' + crxxfile;
                                    if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetHourlyObsIgm): " << url0 <<
                                        "  ->  " << localfile << "  failed" << std::endl;
                                    url0 = url + '/' + dxfile;
                                    if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetHourlyObsIgm): " << url0 <<
                                        "  ->  " << localfile << "  failed" << std::endl;

                                    continue;
                                }
                            }
                        }

#ifdef _WIN32  /* for Windows */
                        cmd = crx2rnxfull + " " + dfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                        cmd = "cat " + dfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                        std::system(cmd.c_str());

                        if (access(ofile.c_str(), 0) == 0)
                        {
                            Logger::Trace(TINFO, "*** INFO(FtpUtil::GetHourlyObsIgm ): successfully download IGM hourly observation file " + ofile);

                            if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetHourlyObsIgm): " << ofile << "  ->  " <<
                                localfile << "  OK" << std::endl;

                            /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                            cmd = "del " + dfile;
#else           /* for Linux or Mac */
                            cmd = "rm -rf " + dfile;
#endif
                            std::system(cmd.c_str());
                        }
                    }
                    else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetHourlyObsIgm ): IGM hourly observation file " + ofile +
                        " or " + dfile + " has existed!");
                }
            }

            /* close 'site.list' */
            sitlst.close();
        }
    }
} /* end of GetHourlyObsIgm */

/**
* @brief   : GetHrObsIgm - to download IGS (short name "d") and MGEX RINEX high-rate observation (1s) files (long name "crx")
*              simutaneously according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetHrObsIgm(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* creation of sub-directory ('highrate') */
    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string subdir = dir + sep + "highrate";
    if (access(subdir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        std::string cmd = "mkdir " + subdir;
#else           /* for Linux or Mac */
        std::string cmd = "mkdir -p " + subdir;
#endif
        std::system(cmd.c_str());
    }

    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);

    /* FTP archive: CDDIS, IGN, or WHU */
    std::string ftpname = fopt->ftpfrom;
    CString::trim(ftpname);
    CString::ToUpper(ftpname);
    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, crx2rnxfull = fopt->crx2rnxfull, qr = fopt->qr;
    std::string::size_type ipos = fopt->obslist.find_last_of(sep) + 1;
    std::string obslist = fopt->obslist.substr(ipos, fopt->obslist.length() - ipos);
    if ((int)obslist.length() < 9) /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        for (size_t i = 0; i < fopt->hhobs.size(); i++)
        {
            std::string shh = CString::int2str(fopt->hhobs[i], 2);
            char tmpdir[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            sprintf(tmpdir, "%s%c%s", subdir.c_str(), sep, shh.c_str());
            std::string shhdir = tmpdir;
            if (access(shhdir.c_str(), 0) == -1)
            {
                /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                std::string cmd = "mkdir " + shhdir;
#else           /* for Linux or Mac */
                std::string cmd = "mkdir -p " + shhdir;
#endif
                std::system(cmd.c_str());
            }

            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(shhdir.c_str());
#else           /* for Linux or Mac */
            chdir(shhdir.c_str());
#endif

            /* download all the MGEX observation files */
            std::string url, cutdirs = " --cut-dirs=8 ";
            if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBMHR] + "/" +
                syyyy + "/" + sdoy + "/" + syy + "d" + "/" + shh;
            else if (ftpname == "IGN")
            {
                url = ftparchive_.IGN[IDX_OBMHR] + "/" + syyyy + "/" + sdoy;
                cutdirs = " --cut-dirs=6 ";
            }
            else if (ftpname == "WHU")
            {
                url = ftparchive_.WHU[IDX_OBMHR] + "/" + syyyy + "/" + sdoy + "/" + syy + "d" + "/" + shh;
                cutdirs = " --cut-dirs=6 ";
            }
            else url = ftparchive_.CDDIS[IDX_OBMHR] + "/" + syyyy + "/" + sdoy + "/" + syy + "d" + "/" + shh;

            /* it is OK for '*.Z' or '*.gz' format */
            std::string crxfile = "*_15M_01S_MO.crx";
            std::string crxxfile = crxfile + ".*";
            std::string cmd = wgetfull + " " + qr + " -nH -A " + crxxfile + cutdirs + url;
            std::system(cmd.c_str());

            /* 'a' = 97, 'b' = 98, ... */
            int ii = fopt->hhobs[i] + 97;
            char ch = ii;
            std::string sch;
            sch.push_back(ch);

            std::vector<std::string> smin = { "00", "15", "30", "45" };
            for (size_t i = 0; i < smin.size(); i++)
            {
                /* get the file list */
                std::string suffix = smin[i] + "_15M_01S_MO.crx";
                std::vector<std::string> crxfiles;
                CString::GetFilesAll(shhdir, suffix, crxfiles);
                std::string sitname;
                for (size_t j = 0; j < crxfiles.size(); j++)
                {
                    if (access(crxfiles[j].c_str(), 0) == 0)
                    {
                        /* extract it */
                        cmd = gzipfull + " -d -f " + crxfiles[j];
                        std::system(cmd.c_str());

                        crxfile = crxfiles[j].substr(0, (int)crxfiles[j].find_last_of('.'));
                        if (access(crxfile.c_str(), 0) == -1) continue;
                        std::string site = crxfiles[j].substr(0, 4);
                        CString::ToLower(site);
                        std::string ofile = site + sdoy + sch + smin[i] + "." + syy + "o";
                        /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                        cmd = crx2rnxfull + " " + crxfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                        cmd = "cat " + crxfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                        std::system(cmd.c_str());

                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + crxfile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + crxfile;
#endif
                        std::system(cmd.c_str());
                    }
                }
            }

            /* download all the IGS observation files */
            if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBSHR] + "/" +
                syyyy + "/" + sdoy + "/" + syy + "d" + "/" + shh;
            else if (ftpname == "IGN")
            {
                url = ftparchive_.IGN[IDX_OBSHR] + "/" + syyyy + "/" + sdoy;
                cutdirs = " --cut-dirs=6 ";
            }
            else if (ftpname == "WHU")
            {
                url = ftparchive_.WHU[IDX_OBSHR] + "/" + syyyy + "/" + sdoy + "/" + syy + "d" + "/" + shh;
                cutdirs = " --cut-dirs=6 ";
            }
            else url = ftparchive_.CDDIS[IDX_OBSHR] + "/" + syyyy + "/" + sdoy + "/" + syy + "d" + "/" + shh;

            /* it is OK for '*.Z' or '*.gz' format */
            std::string dfile = "*" + sdoy + sch + "*." + syy + "d";
            std::string dxfile = dfile + ".*";
            cmd = wgetfull + " " + qr + " -nH -A " + dxfile + cutdirs + url;
            std::system(cmd.c_str());

            for (size_t i = 0; i < smin.size(); i++)
            {
                /* get the file list */
                std::string suffix = smin[i] + "." + syy + "d";
                std::vector<std::string> dfiles;
                CString::GetFilesAll(shhdir, suffix, dfiles);
                std::string sitname;
                for (size_t j = 0; j < dfiles.size(); j++)
                {
                    if (access(dfiles[j].c_str(), 0) == 0)
                    {
                        /* extract it */
                        cmd = gzipfull + " -d -f " + dfiles[j];
                        std::system(cmd.c_str());

                        std::string site = dfiles[j].substr(0, 4);
                        CString::ToLower(site);
                        dfile = site + sdoy + sch + smin[i] + "." + syy + "d";
                        if (access(dfile.c_str(), 0) == -1) continue;
                        std::string ofile = site + sdoy + sch + smin[i] + "." + syy + "o";
                        /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                        cmd = crx2rnxfull + " " + dfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                        cmd = "cat " + dfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                        std::system(cmd.c_str());

                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + dfile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + dfile;
#endif
                        std::system(cmd.c_str());
                    }
                }
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the MGEX observation file site-by-site */
        if (access(fopt->obslist.c_str(), 0) == 0)
        {
            std::ifstream sitlst(fopt->obslist);
            if (!sitlst.is_open())
            {
                Logger::Trace(TERROR, "*** ERROR(FtpUtil::GetHrObsIgm): open site.list = " + fopt->obslist +
                    " file failed, please check it");

                return;
            }

            std::string site, sitname;
            while (getline(sitlst, site))
            {
                if (site[0] == '#') continue;
                CString::trim(site);
                if (site.size() < 4) continue;
                sitname = site.substr(0, 4);
                for (size_t i = 0; i < fopt->hhobs.size(); i++)
                {
                    std::string shh = CString::int2str(fopt->hhobs[i], 2);
                    char tmpdir[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpdir, "%s%c%s", subdir.c_str(), sep, shh.c_str());
                    std::string shhdir = tmpdir;
                    if (access(shhdir.c_str(), 0) == -1)
                    {
                        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                        std::string cmd = "mkdir " + shhdir;
#else           /* for Linux or Mac */
                        std::string cmd = "mkdir -p " + shhdir;
#endif
                        std::system(cmd.c_str());
                    }

                    /* change directory */
#ifdef _WIN32   /* for Windows */
                    _chdir(shhdir.c_str());
#else           /* for Linux or Mac */
                    chdir(shhdir.c_str());
#endif

                    /* 'a' = 97, 'b' = 98, ... */
                    int ii = fopt->hhobs[i] + 97;
                    char ch = ii;
                    std::string sch;
                    sch.push_back(ch);

                    std::vector<std::string> smin = { "00", "15", "30", "45" };
                    for (size_t i = 0; i < smin.size(); i++)
                    {
                        CString::ToLower(sitname);
                        std::string ofile = sitname + sdoy + sch + smin[i] + "." + syy + "o";
                        std::string dfile = sitname + sdoy + sch + smin[i] + "." + syy + "d";
                        if (access(ofile.c_str(), 0) == -1 && access(dfile.c_str(), 0) == -1)
                        {
                            std::string url, cutdirs = " --cut-dirs=8 ";
                            if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBMHR] + "/" +
                                syyyy + "/" + sdoy + "/" + syy + "d" + "/" + shh;
                            else if (ftpname == "IGN")
                            {
                                url = ftparchive_.IGN[IDX_OBMHR] + "/" + syyyy + "/" + sdoy;
                                cutdirs = " --cut-dirs=6 ";
                            }
                            else if (ftpname == "WHU")
                            {
                                url = ftparchive_.WHU[IDX_OBMHR] + "/" + syyyy + "/" + sdoy + "/" + syy + "d" + "/" + shh;
                                cutdirs = " --cut-dirs=6 ";
                            }
                            else url = ftparchive_.CDDIS[IDX_OBMHR] + "/" + syyyy + "/" + sdoy + "/" +
                                syy + "d" + "/" + shh;

                            /* it is OK for '*.Z' or '*.gz' format */
                            CString::ToUpper(sitname);
                            std::string crxfile = sitname + "*" + syyyy + sdoy + shh + smin[i] + "_15M_01S_MO.crx";
                            std::string crxxfile = crxfile + ".*";
                            std::string cmd = wgetfull + " " + qr + " -nH -A " + crxxfile + cutdirs + url;
                            std::system(cmd.c_str());

                            /* extract '*.gz' */
                            std::string crxgzFile = crxfile + ".gz";
                            cmd = gzipfull + " -d -f " + crxgzFile;
                            std::system(cmd.c_str());
                            std::string change_filename;
#ifdef _WIN32  /* for Windows */
                            change_filename = "rename";
#else          /* for Linux or Mac */
                            change_filename = "mv";
#endif
                            cmd = change_filename + " " + crxfile + " " + dfile;
                            std::system(cmd.c_str());

                            char tmpfile[MAXSTRPATH] = { '\0' };
                            char sep = (char)FILEPATHSEP;
                            sprintf(tmpfile, "%s%c%s", shhdir.c_str(), sep, ofile.c_str());
                            std::string localfile = tmpfile;
                            if (access(dfile.c_str(), 0) == -1)
                            {
                                /* extract '*.Z' */
                                std::string crxzFile = crxfile + ".Z";
                                cmd = gzipfull + " -d -f " + crxzFile;
                                std::system(cmd.c_str());

                                cmd = change_filename + " " + crxfile + " " + dfile;
                                std::system(cmd.c_str());
                                if (access(dfile.c_str(), 0) == -1)
                                {
                                    if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBSHR] + "/" +
                                        syyyy + "/" + sdoy + "/" + syy + "d" + "/" + shh;
                                    else if (ftpname == "IGN")
                                    {
                                        url = ftparchive_.IGN[IDX_OBSHR] + "/" + syyyy + "/" + sdoy;
                                        cutdirs = " --cut-dirs=6 ";
                                    }
                                    else if (ftpname == "WHU")
                                    {
                                        url = ftparchive_.WHU[IDX_OBSHR] + "/" + syyyy + "/" + sdoy + "/" + syy +
                                            "d" + "/" + shh;
                                        cutdirs = " --cut-dirs=6 ";
                                    }
                                    else url = ftparchive_.CDDIS[IDX_OBSHR] + "/" + syyyy + "/" + sdoy + "/" + syy +
                                        "d" + "/" + shh;

                                    /* it is OK for '*.Z' or '*.gz' format */
                                    std::string dxfile = dfile + ".*";
                                    cmd = wgetfull + " " + qr + " -nH -A " + dxfile + cutdirs + url;
                                    std::system(cmd.c_str());

                                    std::string dgzfile = dfile + ".gz", dzfile = dfile + ".Z";
                                    if (access(dgzfile.c_str(), 0) == 0)
                                    {
                                        /* extract '*.gz' */
                                        cmd = gzipfull + " -d -f " + dgzfile;
                                        std::system(cmd.c_str());

                                        if (access(dfile.c_str(), 0) == 0 && access(dzfile.c_str(), 0) == 0)
                                        {
                                            /* delete '*.Z' file */
#ifdef _WIN32   /* for Windows */
                                            cmd = "del " + dzfile;
#else           /* for Linux or Mac */
                                            cmd = "rm -rf " + dzfile;
#endif
                                            std::system(cmd.c_str());
                                        }
                                    }
                                    if (access(dfile.c_str(), 0) == -1 && access(dzfile.c_str(), 0) == 0)
                                    {
                                        /* extract '*.Z' */
                                        cmd = gzipfull + " -d -f " + dzfile;
                                        std::system(cmd.c_str());
                                    }
                                    if (access(dfile.c_str(), 0) == -1)
                                    {
                                        Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetHrObsIgm): failed to download IGM high-rate observation file " + ofile);

                                        std::string url0 = url + '/' + crxxfile;
                                        if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetHrObsIgm): " << url0 <<
                                            "  ->  " << localfile << "  failed" << std::endl;
                                        url0 = url + '/' + dxfile;
                                        if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetHrObsIgm): " << url0 <<
                                            "  ->  " << localfile << "  failed" << std::endl;

                                        continue;
                                    }
                                }
                            }

#ifdef _WIN32  /* for Windows */
                            cmd = crx2rnxfull + " " + dfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                            cmd = "cat " + dfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                            std::system(cmd.c_str());

                            if (access(ofile.c_str(), 0) == 0)
                            {
                                Logger::Trace(TINFO, "*** INFO(FtpUtil::GetHrObsIgm): successfully download IGM high-rate observation file " + ofile);

                                if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetHrObsIgm): " << ofile << "  ->  " <<
                                    localfile << "  OK" << std::endl;

                                /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                                cmd = "del " + dfile;
#else           /* for Linux or Mac */
                                cmd = "rm -rf " + dfile;
#endif
                                std::system(cmd.c_str());
                            }
                        }
                        else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetHrObsIgm): IGM high-rate observation file " + ofile +
                            " or " + dfile + " has existed!");
                    }
                }
            }

            /* close 'site.list' */
            sitlst.close();
        }
    }
} /* end of GetHrObsIgm */

/**
* @brief   : GetDailyObsCut - download Curtin University of Technology (CUT) RINEX daily observation (30s) files (long name "crx")
*              according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetDailyObsCut(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* creation of sub-directory ('daily') */
    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string subdir = dir + sep + "daily";
    if (access(subdir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        std::string cmd = "mkdir " + subdir;
#else           /* for Linux or Mac */
        std::string cmd = "mkdir -p " + subdir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subdir.c_str());
#else           /* for Linux or Mac */
    chdir(subdir.c_str());
#endif

    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);

    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, crx2rnxfull = fopt->crx2rnxfull, qr = fopt->qr;
    std::string url = "http://saegnss2.curtin.edu/ldc/rinex3/daily/" + syyyy + "/" + sdoy;
    std::string cutdirs = " --cut-dirs=5 ";
    /* download the Curtin University of Technology (CUT) observation file site-by-site */
    if (access(fopt->obslist.c_str(), 0) == 0)
    {
        std::ifstream sitlst(fopt->obslist);
        if (!sitlst.is_open())
        {
            Logger::Trace(TERROR, "*** ERROR(FtpUtil::GetDailyObsCut): open site.list = " + fopt->obslist + " file failed, please check it");
            return;
        }

        std::string site, sitname;
        while (getline(sitlst, site))
        {
            if (site[0] == '#') continue;
            CString::trim(site);
            if (site.size() < 4) continue;
            sitname = site.substr(0, 4);
            CString::ToLower(sitname);
            std::string ofile = sitname + sdoy + "0." + syy + "o";
            if (access(ofile.c_str(), 0) == -1)
            {
                /* it is OK for '*.gz' format */
                CString::ToUpper(sitname);
                std::string crxfile = sitname + "00AUS_R_" + syyyy + sdoy + "0000_01D_30S_MO.crx";
                std::string crxgzFile = crxfile + ".gz";
                std::string cmd = wgetfull + " " + qr + " -nH " + cutdirs + url + "/" + crxgzFile;
                std::system(cmd.c_str());

                /* extract '*.gz' */
                cmd = gzipfull + " -d -f " + crxgzFile;
                std::system(cmd.c_str());

                char tmpfile[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpfile, "%s%c%s", subdir.c_str(), sep, ofile.c_str());
                std::string localfile = tmpfile;
                if (access(crxfile.c_str(), 0) == -1)
                {
                    Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetDailyObsCut): failed to download CUT daily observation file " + ofile);

                    std::string url0 = url + '/' + crxgzFile;
                    if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetDailyObsCut): " << url0 << "  ->  " <<
                        localfile << "  failed" << std::endl;

                    continue;
                }

#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxfull + " " + crxfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                cmd = "cat " + crxfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                std::system(cmd.c_str());

                if (access(ofile.c_str(), 0) == 0)
                {
                    Logger::Trace(TINFO, "*** INFO(FtpUtil::GetDailyObsCut): successfully download CUT daily observation file " + ofile);

                    std::string url0 = url + '/' + crxgzFile;
                    if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetDailyObsCut): " << url0 << "  ->  " <<
                        localfile << "  OK" << std::endl;

                    /* delete 'crx' file */
#ifdef _WIN32   /* for Windows */
                    cmd = "del " + crxfile;
#else           /* for Linux or Mac */
                    cmd = "rm -rf " + crxfile;
#endif
                    std::system(cmd.c_str());
                }
            }
            else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetDailyObsCut): CUT daily observation file " +
                ofile + " has existed!");
        }

        /* close 'site.list' */
        sitlst.close();
    }
} /* end of GetDailyObsCut */

/**
* @brief   : Get30sObsHk - download Hong Kong CORS RINEX daily observation (30s) files (long name "crx")
               according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::Get30sObsHk(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* creation of sub-directory ('daily') */
    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string subdir = dir + sep + "30s";
    if (access(subdir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        std::string cmd = "mkdir " + subdir;
#else           /* for Linux or Mac */
        std::string cmd = "mkdir -p " + subdir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subdir.c_str());
#else           /* for Linux or Mac */
    chdir(subdir.c_str());
#endif

    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);

    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, crx2rnxfull = fopt->crx2rnxfull, qr = fopt->qr;
    std::string url0 = "https://rinex.geodetic.gov.hk/rinex3/" + syyyy + "/" + sdoy;
    std::string cutdirs = " --cut-dirs=5 ";
    /* download the Hong Kong CORS observation file site-by-site */
    if (access(fopt->obslist.c_str(), 0) == 0)
    {
        std::ifstream sitlst(fopt->obslist);
        if (!sitlst.is_open())
        {
            Logger::Trace(TERROR, "*** ERROR(FtpUtil::Get30sObsHk): open site.list = " + fopt->obslist +
                " file failed, please check it");
            return;
        }

        std::string site, sitname;
        while (getline(sitlst, site))
        {
            if (site[0] == '#') continue;
            CString::trim(site);
            if (site.size() < 4) continue;
            sitname = site.substr(0, 4);
            CString::ToLower(sitname);
            std::string ofile = sitname + sdoy + "0." + syy + "o";
            std::string url = url0 + "/" + sitname + "/30s";
            if (access(ofile.c_str(), 0) == -1)
            {
                /* it is OK for '*.gz' format */
                CString::ToUpper(sitname);
                std::string crxfile = sitname + "00HKG_R_" + syyyy + sdoy + "0000_01D_30S_MO.crx";
                std::string crxgzFile = crxfile + ".gz";
                std::string cmd = wgetfull + " " + qr + " -nH " + cutdirs + url + "/" + crxgzFile;
                std::system(cmd.c_str());

                /* extract '*.gz' */
                cmd = gzipfull + " -d -f " + crxgzFile;
                std::system(cmd.c_str());

                char tmpfile[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpfile, "%s%c%s", subdir.c_str(), sep, ofile.c_str());
                std::string localfile = tmpfile;
                if (access(crxfile.c_str(), 0) == -1)
                {
                    Logger::Trace(TWARNING, "*** WARNING(FtpUtil::Get30sObsHk): failed to download HK CORS 30s observation file " + ofile);

                    std::string url0 = url + '/' + crxgzFile;
                    if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::Get30sObsHk): " << url0 << "  ->  " <<
                        localfile << "  failed" << std::endl;

                    continue;
                }

#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxfull + " " + crxfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                cmd = "cat " + crxfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                std::system(cmd.c_str());

                if (access(ofile.c_str(), 0) == 0)
                {
                    Logger::Trace(TINFO, "*** INFO(FtpUtil::Get30sObsHk): successfully download HK CORS 30s observation file " + ofile);

                    std::string url0 = url + '/' + crxgzFile;
                    if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::Get30sObsHk): " << url0 << "  ->  " <<
                        localfile << "  OK" << std::endl;

                    /* delete 'crx' file */
#ifdef _WIN32   /* for Windows */
                    cmd = "del " + crxfile;
#else           /* for Linux or Mac */
                    cmd = "rm -rf " + crxfile;
#endif
                    std::system(cmd.c_str());
                }
            }
            else Logger::Trace(TINFO, "*** INFO(FtpUtil::Get30sObsHk): HK CORS 30s observation file " +
                ofile + " has existed!");
        }

        /* close 'site.list' */
        sitlst.close();
    }
} /* end of Get30sObsHk */

/**
* @brief   : Get5sObsHk - download Hong Kong CORS RINEX daily observation (5s) files (long name "crx")
               according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::Get5sObsHk(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* creation of sub-directory ('daily') */
    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string subdir = dir + sep + "5s";
    if (access(subdir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        std::string cmd = "mkdir " + subdir;
#else           /* for Linux or Mac */
        std::string cmd = "mkdir -p " + subdir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subdir.c_str());
#else           /* for Linux or Mac */
    chdir(subdir.c_str());
#endif

    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);

    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, crx2rnxfull = fopt->crx2rnxfull, qr = fopt->qr;
    std::string url0 = "https://rinex.geodetic.gov.hk/rinex3/" + syyyy + "/" + sdoy;
    std::string cutdirs = " --cut-dirs=5 ";
    /* download the Hong Kong CORS observation file site-by-site */
    if (access(fopt->obslist.c_str(), 0) == 0)
    {
        std::ifstream sitlst(fopt->obslist);
        if (!sitlst.is_open())
        {
            Logger::Trace(TERROR, "*** ERROR(FtpUtil::Get5sObsHk): open site.list = " + fopt->obslist +
                " file failed, please check it");

            return;
        }

        std::string site, sitname;
        while (getline(sitlst, site))
        {
            if (site[0] == '#') continue;
            CString::trim(site);
            if (site.size() < 4) continue;
            sitname = site.substr(0, 4);
            for (size_t i = 0; i < fopt->hhobs.size(); i++)
            {
                std::string shh = CString::int2str(fopt->hhobs[i], 2);
                char tmpdir[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpdir, "%s%c%s", subdir.c_str(), sep, shh.c_str());
                std::string shhdir = tmpdir;
                if (access(shhdir.c_str(), 0) == -1)
                {
                    /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                    std::string cmd = "mkdir " + shhdir;
#else           /* for Linux or Mac */
                    std::string cmd = "mkdir -p " + shhdir;
#endif
                    std::system(cmd.c_str());
                }

                /* change directory */
#ifdef _WIN32   /* for Windows */
                _chdir(shhdir.c_str());
#else           /* for Linux or Mac */
                chdir(shhdir.c_str());
#endif

                /* 'a' = 97, 'b' = 98, ... */
                int ii = fopt->hhobs[i] + 97;
                char ch = ii;
                std::string sch;
                sch.push_back(ch);
                CString::ToLower(sitname);
                std::string ofile = sitname + sdoy + sch + "." + syy + "o";
                std::string url = url0 + "/" + sitname + "/5s";
                if (access(ofile.c_str(), 0) == -1)
                {
                    /* it is OK for '*.gz' format */
                    CString::ToUpper(sitname);
                    std::string crxfile = sitname + "00HKG_R_" + syyyy + sdoy + shh + "00_01H_05S_MO.crx";
                    std::string crxgzFile = crxfile + ".gz";
                    std::string cmd = wgetfull + " " + qr + " -nH " + cutdirs + url + "/" + crxgzFile;
                    std::system(cmd.c_str());

                    /* extract '*.gz' */
                    cmd = gzipfull + " -d -f " + crxgzFile;
                    std::system(cmd.c_str());

                    char tmpfile[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpfile, "%s%c%s", shhdir.c_str(), sep, ofile.c_str());
                    std::string localfile = tmpfile;
                    if (access(crxfile.c_str(), 0) == -1)
                    {
                        Logger::Trace(TWARNING, "*** WARNING(FtpUtil::Get5sObsHk): failed to download HK CORS 5s observation file " + ofile);

                        std::string url0 = url + '/' + crxgzFile;
                        if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::Get5sObsHk): " << url0 << "  ->  " <<
                            localfile << "  failed" << std::endl;

                        continue;
                    }

#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxfull + " " + crxfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                    cmd = "cat " + crxfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                    std::system(cmd.c_str());

                    if (access(ofile.c_str(), 0) == 0)
                    {
                        Logger::Trace(TINFO, "*** INFO(FtpUtil::Get5sObsHk): successfully download HK CORS 5s observation file " + ofile);

                        std::string url0 = url + '/' + crxgzFile;
                        if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::Get5sObsHk): " << url0 << "  ->  " <<
                            localfile << "  OK" << std::endl;

                        /* delete 'crx' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + crxfile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + crxfile;
#endif
                        std::system(cmd.c_str());
                    }
                }
                else Logger::Trace(TINFO, "*** INFO(FtpUtil::Get5sObsHk): HK CORS 5s observation file " + ofile + " has existed!");
            }
        }

        /* close 'site.list' */
        sitlst.close();
    }
} /* end of Get5sObsHk */

/**
* @brief   : Get1sObsHk - download Hong Kong CORS RINEX daily observation (1s) files (long name "crx")
               according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::Get1sObsHk(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* creation of sub-directory ('daily') */
    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string subdir = dir + sep + "1s";
    if (access(subdir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        std::string cmd = "mkdir " + subdir;
#else           /* for Linux or Mac */
        std::string cmd = "mkdir -p " + subdir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subdir.c_str());
#else           /* for Linux or Mac */
    chdir(subdir.c_str());
#endif

    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);

    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, crx2rnxfull = fopt->crx2rnxfull, qr = fopt->qr;
    std::string url0 = "https://rinex.geodetic.gov.hk/rinex3/" + syyyy + "/" + sdoy;
    std::string cutdirs = " --cut-dirs=5 ";
    /* download the Hong Kong CORS observation file site-by-site */
    if (access(fopt->obslist.c_str(), 0) == 0)
    {
        std::ifstream sitlst(fopt->obslist);
        if (!sitlst.is_open())
        {
            Logger::Trace(TERROR, "*** ERROR(FtpUtil::Get1sObsHk): open site.list = " + fopt->obslist +
                " file failed, please check it");
            return;
        }

        std::string site, sitname;
        while (getline(sitlst, site))
        {
            if (site[0] == '#') continue;
            CString::trim(site);
            if (site.size() < 4) continue;
            sitname = site.substr(0, 4);
            for (size_t i = 0; i < fopt->hhobs.size(); i++)
            {
                std::string shh = CString::int2str(fopt->hhobs[i], 2);
                char tmpdir[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpdir, "%s%c%s", subdir.c_str(), sep, shh.c_str());
                std::string shhdir = tmpdir;
                if (access(shhdir.c_str(), 0) == -1)
                {
                    /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                    std::string cmd = "mkdir " + shhdir;
#else           /* for Linux or Mac */
                    std::string cmd = "mkdir -p " + shhdir;
#endif
                    std::system(cmd.c_str());
                }

                /* change directory */
#ifdef _WIN32   /* for Windows */
                _chdir(shhdir.c_str());
#else           /* for Linux or Mac */
                chdir(shhdir.c_str());
#endif

                /* 'a' = 97, 'b' = 98, ... */
                int ii = fopt->hhobs[i] + 97;
                char ch = ii;
                std::string sch;
                sch.push_back(ch);
                CString::ToLower(sitname);
                std::string ofile = sitname + sdoy + sch + "." + syy + "o";
                std::string url = url0 + "/" + sitname + "/1s";
                if (access(ofile.c_str(), 0) == -1)
                {
                    /* it is OK for '*.gz' format */
                    CString::ToUpper(sitname);
                    std::string crxfile = sitname + "00HKG_R_" + syyyy + sdoy + shh + "00_01H_01S_MO.crx";
                    std::string crxgzFile = crxfile + ".gz";
                    std::string cmd = wgetfull + " " + qr + " -nH " + cutdirs + url + "/" + crxgzFile;
                    std::system(cmd.c_str());

                    /* extract '*.gz' */
                    cmd = gzipfull + " -d -f " + crxgzFile;
                    std::system(cmd.c_str());

                    char tmpfile[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpfile, "%s%c%s", shhdir.c_str(), sep, ofile.c_str());
                    std::string localfile = tmpfile;
                    if (access(crxfile.c_str(), 0) == -1)
                    {
                        Logger::Trace(TWARNING, "*** WARNING(FtpUtil::Get1sObsHk): failed to download HK CORS 1s observation file " + ofile);

                        std::string url0 = url + '/' + crxgzFile;
                        if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::Get1sObsHk): " << url0 << "  ->  " <<
                            localfile << "  failed" << std::endl;

                        continue;
                    }

#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxfull + " " + crxfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                    cmd = "cat " + crxfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                    std::system(cmd.c_str());

                    if (access(ofile.c_str(), 0) == 0)
                    {
                        Logger::Trace(TINFO, "*** INFO(FtpUtil::Get1sObsHk): successfully download HK CORS 1s observation file " + ofile);

                        std::string url0 = url + '/' + crxgzFile;
                        if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::Get1sObsHk): " << url0 << "  ->  " <<
                            localfile << "  failed" << std::endl;

                        /* delete 'crx' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + crxfile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + crxfile;
#endif
                        std::system(cmd.c_str());
                    }
                }
                else Logger::Trace(TINFO, "*** INFO(FtpUtil::Get1sObsHk): HK CORS 1s observation file " +
                    ofile + " has existed!");
            }
        }

        /* close 'site.list' */
        sitlst.close();
    }
} /* end of Get1sObsHk */

/**
* @brief   : GetDailyObsNgs - download NGS/NOAA CORS RINEX daily observation (30s) files (short name "d")
               according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetDailyObsNgs(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* creation of sub-directory ('daily') */
    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string subdir = dir + sep + "daily";
    if (access(subdir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        std::string cmd = "mkdir " + subdir;
#else           /* for Linux or Mac */
        std::string cmd = "mkdir -p " + subdir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subdir.c_str());
#else           /* for Linux or Mac */
    chdir(subdir.c_str());
#endif

    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);

    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, crx2rnxfull = fopt->crx2rnxfull, qr = fopt->qr;
    std::string url = "https://noaa-cors-pds.s3.amazonaws.com/rinex/" + syyyy + "/" + sdoy;
    std::string cutdirs = " --cut-dirs=4 ";
    /* download the NGS/NOAA CORS observation file site-by-site */
    if (access(fopt->obslist.c_str(), 0) == 0)
    {
        std::ifstream sitlst(fopt->obslist);
        if (!sitlst.is_open())
        {
            Logger::Trace(TERROR, "*** ERROR(FtpUtil::GetDailyObsNgs): open site.list = " + fopt->obslist +
                " file failed, please check it");
            return;
        }

        std::string site, sitname;
        while (getline(sitlst, site))
        {
            if (site[0] == '#') continue;
            CString::trim(site);
            if (site.size() < 4) continue;
            sitname = site.substr(0, 4);
            CString::ToLower(sitname);
            std::string ofile = sitname + sdoy + "0." + syy + "o";
            if (access(ofile.c_str(), 0) == -1)
            {
                /* it is OK for '*.gz' format */
                std::string dfile = sitname + sdoy + "0." + syy + "d";
                std::string dgzfile = dfile + ".gz";
                std::string cmd = wgetfull + " " + qr + " -nH " + cutdirs + url + "/" + sitname + "/" + dgzfile;
                std::system(cmd.c_str());

                /* extract '*.gz' */
                cmd = gzipfull + " -d -f " + dgzfile;
                std::system(cmd.c_str());

                char tmpfile[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpfile, "%s%c%s", subdir.c_str(), sep, ofile.c_str());
                std::string localfile = tmpfile;
                if (access(dfile.c_str(), 0) == -1)
                {
                    Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetDailyObsNgs): failed to download NGS/NOAA CORS daily observation file " + ofile);

                    std::string url0 = url + '/' + dgzfile;
                    if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetDailyObsNgs): " << url0 << "  ->  " <<
                        localfile << "  failed" << std::endl;

                    continue;
                }

#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxfull + " " + dfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                cmd = "cat " + dfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                std::system(cmd.c_str());

                if (access(ofile.c_str(), 0) == 0)
                {
                    Logger::Trace(TINFO, "*** INFO(FtpUtil::GetDailyObsNgs): successfully download NGS/NOAA CORS daily observation file " + ofile);

                    std::string url0 = url + '/' + dgzfile;
                    if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetDailyObsNgs): " << url0 << "  ->  " <<
                        localfile << "  OK" << std::endl;

                    /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                    cmd = "del " + dfile;
#else           /* for Linux or Mac */
                    cmd = "rm -rf " + dfile;
#endif
                    std::system(cmd.c_str());
                }
            }
            else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetDailyObsNgs): NGS/NOAA CORS daily observation file " +
                ofile + " has existed!");
        }

        /* close 'site.list' */
        sitlst.close();
    }
} /* end of GetDailyObsNgs */

/**
* @brief   : GetDailyObsEpn - download EUREF Permanent Network (EPN) RINEX daily observation (30s) files
               (long name 'crx' and short name "d") according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetDailyObsEpn(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* creation of sub-directory ('daily') */
    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string subdir = dir + sep + "daily";
    if (access(subdir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        std::string cmd = "mkdir " + subdir;
#else           /* for Linux or Mac */
        std::string cmd = "mkdir -p " + subdir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subdir.c_str());
#else           /* for Linux or Mac */
    chdir(subdir.c_str());
#endif

    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);

    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, crx2rnxfull = fopt->crx2rnxfull, qr = fopt->qr;
    std::string url = "ftp://ftp.epncb.oma.be/pub/obs/" + syyyy + "/" + sdoy;
    std::string cutdirs = " --cut-dirs=4 ";
    std::string::size_type ipos = fopt->obslist.find_last_of(sep) + 1;
    std::string obslist = fopt->obslist.substr(ipos, fopt->obslist.length() - ipos);
    if ((int)obslist.length() < 9) /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* it is OK for '*.gz' format */
        std::string crxfile = "*" + syyyy + sdoy + "0000_01D_30S_MO.crx";
        std::string crxgzFile = crxfile + ".gz";
        std::string cmd = wgetfull + " " + qr + " -nH -A " + crxgzFile + cutdirs + url;
        std::system(cmd.c_str());

        /* get the file list */
        std::string suffix = ".crx";
        std::vector<std::string> crxfiles;
        CString::GetFilesAll(subdir, suffix, crxfiles);
        std::string sitname;
        for (size_t i = 0; i < crxfiles.size(); i++)
        {
            if (access(crxfiles[i].c_str(), 0) == 0)
            {
                /* extract it */
                cmd = gzipfull + " -d -f " + crxfiles[i];
                std::system(cmd.c_str());

                crxfile = crxfiles[i].substr(0, (int)crxfiles[i].find_last_of('.'));
                if (access(crxfile.c_str(), 0) == -1) continue;
                std::string site = crxfiles[i].substr(0, 4);
                CString::ToLower(site);
                std::string ofile = site + sdoy + "0." + syy + "o";
                /* convert from 'crx' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxfull + " " + crxfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                cmd = "cat " + crxfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                std::system(cmd.c_str());

                /* delete crxfile */
#ifdef _WIN32   /* for Windows */
                cmd = "del " + crxfile;
#else           /* for Linux or Mac */
                cmd = "rm -rf " + crxfile;
#endif
                std::system(cmd.c_str());
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the EPN observation file site-by-site */
        if (access(fopt->obslist.c_str(), 0) == 0)
        {
            std::ifstream sitlst(fopt->obslist);
            if (!sitlst.is_open())
            {
                Logger::Trace(TERROR, "*** ERROR(FtpUtil::GetDailyObsEpn): open site.list = " + fopt->obslist +
                    " file failed, please check it");
                return;
            }

            std::string site, sitname;
            while (getline(sitlst, site))
            {
                if (site[0] == '#') continue;
                CString::trim(site);
                if (site.size() < 4) continue;
                sitname = site.substr(0, 4);
                CString::ToLower(sitname);
                std::string ofile = sitname + sdoy + "0." + syy + "o";
                std::string dfile = sitname + sdoy + "0." + syy + "d";
                if (access(ofile.c_str(), 0) == -1)
                {
                    /* it is OK for '*.gz' format */
                    CString::ToUpper(sitname);
                    std::string crxfile = sitname + "*" + syyyy + sdoy + "0000_01D_30S_MO.crx";
                    std::string crxgzFile = crxfile + ".gz";
                    std::string cmd = wgetfull + " " + qr + " -nH -A " + crxgzFile + cutdirs + url;
                    std::system(cmd.c_str());

                    /* extract '*.gz' */
                    cmd = gzipfull + " -d -f " + crxgzFile;
                    std::system(cmd.c_str());
                    std::string change_filename;
#ifdef _WIN32  /* for Windows */
                    change_filename = "rename";
#else          /* for Linux or Mac */
                    change_filename = "mv";
#endif
                    cmd = change_filename + " " + crxfile + " " + dfile;
                    std::system(cmd.c_str());

                    char tmpfile[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpfile, "%s%c%s", subdir.c_str(), sep, ofile.c_str());
                    std::string localfile = tmpfile;
                    if (access(dfile.c_str(), 0) == -1)
                    {
                        Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetDailyObsEpn): failed to download EPN daily observation file " + ofile);

                        std::string url0 = url + '/' + crxgzFile;
                        if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetDailyObsEpn): " << url0 << "  ->  " <<
                            localfile << "  failed" << std::endl;

                        continue;
                    }

#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxfull + " " + dfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                    cmd = "cat " + dfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                    std::system(cmd.c_str());

                    if (access(ofile.c_str(), 0) == 0)
                    {
                        Logger::Trace(TINFO, "*** INFO(FtpUtil::GetDailyObsEpn): successfully download EPN daily observation file " + ofile);

                        std::string url0 = url + '/' + crxgzFile;
                        if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetDailyObsEpn): " << url0 << "  ->  " <<
                            localfile << "  OK" << std::endl;

                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + dfile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + dfile;
#endif
                        std::system(cmd.c_str());
                    }
                }
                else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetDailyObsEpn): EPN daily observation file " +
                    ofile + " has existed!");
            }

            /* close 'site.list' */
            sitlst.close();
        }
    }
} /* end of GetDailyObsEpn */

/**
* @brief   : GetDailyObsPbo - to download Plate Boundary Observatory (PBO) RINEX daily observation (15s) files
               (RINEX version 3.xx, long name "crx") according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetDailyObsPbo(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* creation of sub-directory ('daily') */
    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string subdir = dir + sep + "daily";
    if (access(subdir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        std::string cmd = "mkdir " + subdir;
#else           /* for Linux or Mac */
        std::string cmd = "mkdir -p " + subdir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subdir.c_str());
#else           /* for Linux or Mac */
    chdir(subdir.c_str());
#endif

    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);

    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, crx2rnxfull = fopt->crx2rnxfull, qr = fopt->qr;
    std::string url = "https://data.unavco.org/archive/gnss/rinex3/obs/" + syyyy + "/" + sdoy;
    std::string cutdirs = " --cut-dirs=5 ";
    std::string key = "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCIsImtpZCI6Im80WDNMM1p0QkN6MmZ5RktMVW9mWiJ9.eyJpc3MiOiJodHRwczovL2xvZ2luLmVhcnRoc2NvcGUub3JnLyIsInN1YiI6ImF1dGgwfDY1MzM5MWY0NGEwYjBiZTZiN2M4YTZmZiIsImF1ZCI6WyJodHRwczovL2FjY291bnQuZWFydGhzY29wZS5vcmciLCJodHRwczovL2VhcnRoc2NvcGUtcHJvZC51cy5hdXRoMC5jb20vdXNlcmluZm8iXSwiaWF0IjoxNjk3ODkwNzUyLCJleHAiOjE2OTc5MTk1NTIsImF6cCI6ImpNeFFiYktQRXN6V1gwYTkzQUdsQlpaOHJuOTE3b2N4Iiwic2NvcGUiOiJvcGVuaWQgcHJvZmlsZSBlbWFpbCIsInBlcm1pc3Npb25zIjpbXX0.TkNApGtx6Jhr-Es7IbuWNhMHtP5anSVLybTC5OkJqEnyfPLcEMrOuTYbMYQ7t_xFKMQwWKiB8tU9mIfaYBq-UQezkrMTRAVT5cvtUMrezImXzPxG8LThqgL9RHHBcNmHGkcxLONlFzpxu4AUgU2Y-j4a0cSDSAIehK0ELqQ7IXpBQsfeaQ3m65s0yen_fBHzthLPe0cjnXGEbk6LU4rnhqz5DxG46z-4lg1OAt91c8y_CmHlyoreuD34xnoO0qqURxcRpzRi9LoXIq_wphaacaIz-Jf6e1PXbKoZ3yWH5EYMd_nLX5Ozvjt971MERy7FRDWbe6A6vZHwfk5_QxjGMQ";
    std::string auth = " --header \"Authorization: Bearer " + key + "\"";
    /* download the PBO observation file site-by-site */
    if (access(fopt->obslist.c_str(), 0) == 0)
    {
        std::ifstream sitlst(fopt->obslist);
        if (!sitlst.is_open())
        {
            Logger::Trace(TERROR, "*** ERROR(FtpUtil::GetDailyObsPbo3): open site.list = " + fopt->obslist +
                " file failed, please check it");

            return;
        }

        std::string site, sitname;
        while (getline(sitlst, site))
        {
            if (site[0] == '#') continue;
            CString::trim(site);
            if (site.size() < 4) continue;
            sitname = site.substr(0, 4);
            CString::ToLower(sitname);
            std::string ofile = sitname + sdoy + "0." + syy + "o";
            std::string dfile = sitname + sdoy + "0." + syy + "d";
            if (access(ofile.c_str(), 0) == -1 && access(dfile.c_str(), 0) == -1)
            {
                /* it is OK for '*.gz' format */
                CString::ToUpper(sitname);
                std::string crxfile = sitname + "00USA_R_" + syyyy + sdoy + "0000_01D_15S_MO.crx";
                std::string crxgzfile = crxfile + ".gz";
                std::string cmd = wgetfull + " " + url + "/" + crxgzfile + auth;
                std::cout << cmd << std::endl;
                std::system(cmd.c_str());

                /* extract '*.gz' */
                cmd = gzipfull + " -d -f " + crxgzfile;
                std::system(cmd.c_str());

                std::string sep;
                sep.push_back((char)FILEPATHSEP);
                std::string localfile = subdir + sep + ofile;
                if (access(crxfile.c_str(), 0) == -1)
                {
                    Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetDailyObsPbo3): failed to download PBO daily observation file " + ofile);

                    std::string url0 = url + '/' + crxgzfile;
                    if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetDailyObsPbo3): " << url0 << "  ->  " <<
                        localfile << "  failed" << std::endl;
                }
                else
                {
                    std::string change_filename;
#ifdef _WIN32  /* for Windows */
                    change_filename = "rename";
#else          /* for Linux or Mac */
                    change_filename = "mv";
#endif
                    cmd = change_filename + " " + crxfile + " " + dfile;
                    std::system(cmd.c_str());

#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxfull + " " + dfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                    cmd = "cat " + dfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                    std::system(cmd.c_str());

                    if (access(ofile.c_str(), 0) == 0)
                    {
                        Logger::Trace(TINFO, "*** INFO(FtpUtil::GetDailyObsPbo3): successfully download PBO daily observation file " + ofile);

                        std::string url0 = url + '/' + crxgzfile;
                        if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetDailyObsPbo3): " << url0 << "  ->  " <<
                            localfile << "  OK" << std::endl;

                        /* delete 'd' file */
    #ifdef _WIN32   /* for Windows */
                        cmd = "del " + dfile;
    #else           /* for Linux or Mac */
                        cmd = "rm -rf " + dfile;
    #endif
                        std::system(cmd.c_str());
                    }
                }
            }
            else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetDailyObsPbo3): PBO daily observation file " +
                ofile + " has existed!");
        }

        /* close 'site.list' */
        sitlst.close();
    }
} /* end of GetDailyObsPbo */

/**
* @brief   : GetDailyObsChi - to download Centro Sismologico Nacional of Universidad de Chile RINEX daily observation (1s) files
               (RINEX version 2.xx, short name "d") according to 'site.list' file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetDailyObsChi(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* creation of sub-directory ('daily') */
    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string subdir = dir + sep + "daily";
    if (access(subdir.c_str(), 0) == -1)
    {
        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
        std::string cmd = "mkdir " + subdir;
#else           /* for Linux or Mac */
        std::string cmd = "mkdir -p " + subdir;
#endif
        std::system(cmd.c_str());
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(subdir.c_str());
#else           /* for Linux or Mac */
    chdir(subdir.c_str());
#endif

    /* compute day of year */

    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);

    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);

    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, crx2rnxfull = fopt->crx2rnxfull, qr = fopt->qr;
    std::string url = "http://gps.csn.uchile.cl/data/" + syyyy + "/" + sdoy;
    std::string cutdirs = " --cut-dirs=3 ";
    std::string::size_type ipos = fopt->obslist.find_last_of(sep) + 1;
    std::string obslist = fopt->obslist.substr(ipos, fopt->obslist.length() - ipos);
    if ((int)obslist.length() < 9) /* the option of 'all' is selected; the length of "site.list" is nine */
    {
        /* it is OK for '*.Z' or '*.gz' format */
        std::string dfile = "*" + sdoy + "0." + syy + "d";
        std::string dxfile = dfile + ".*";
        std::string cmd = wgetfull + " " + qr + " -nH -A " + dxfile + cutdirs + url;
        std::system(cmd.c_str());

        /* get the file list */
        std::string suffix = "." + syy + "d";
        std::vector<std::string> dfiles;
        CString::GetFilesAll(subdir, suffix, dfiles);
        std::string sitname;
        for (size_t i = 0; i < dfiles.size(); i++)
        {
            if (access(dfiles[i].c_str(), 0) == 0)
            {
                /* extract it */
                cmd = gzipfull + " -d -f " + dfiles[i];
                std::system(cmd.c_str());

                std::string site = dfiles[i].substr(0, 4);
                dfile = site + sdoy + "0." + syy + "d";
                if (access(dfile.c_str(), 0) == -1) continue;
                std::string ofile = site + sdoy + "0." + syy + "o";
                /* convert from 'd' file to 'o' file */
#ifdef _WIN32  /* for Windows */
                cmd = crx2rnxfull + " " + dfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                cmd = "cat " + dfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                std::system(cmd.c_str());

                /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                cmd = "del " + dfile;
#else           /* for Linux or Mac */
                cmd = "rm -rf " + dfile;
#endif
                std::system(cmd.c_str());
            }
        }
    }
    else  /* the option of full path of site list file is selected */
    {
        /* download the Centro Sismologico Nacional of Universidad de Chile observation file site-by-site */
        if (access(fopt->obslist.c_str(), 0) == 0)
        {
            std::ifstream sitlst(fopt->obslist);
            if (!sitlst.is_open())
            {
                Logger::Trace(TERROR, "*** ERROR(FtpUtil::GetDailyObsChi): open site.list = " + fopt->obslist +
                    " file failed, please check it");
                return;
            }

            std::string site, sitname;
            while (getline(sitlst, site))
            {
                if (site[0] == '#') continue;
                CString::trim(site);
                if (site.size() < 4) continue;
                sitname = site.substr(0, 4);
                CString::ToLower(sitname);
                std::string ofile = sitname + sdoy + "0." + syy + "o";
                std::string dfile = sitname + sdoy + "0." + syy + "d";
                if (access(ofile.c_str(), 0) == -1 && access(dfile.c_str(), 0) == -1)
                {
                    /* it is OK for '*.Z' or '*.gz' format */
                    std::string dxfile = dfile + ".*";
                    std::string cmd = wgetfull + " " + qr + " -nH -A " + dxfile + cutdirs + url;
                    std::system(cmd.c_str());

                    std::string dgzfile = dfile + ".gz", dzfile = dfile + ".Z";
                    bool isgz = false;
                    if (access(dgzfile.c_str(), 0) == 0)
                    {
                        /* extract '*.gz' */
                        cmd = gzipfull + " -d -f " + dgzfile;
                        std::system(cmd.c_str());
                        isgz = true;

                        if (access(dfile.c_str(), 0) == 0 && access(dzfile.c_str(), 0) == 0)
                        {
                            /* delete '*.Z' file */
#ifdef _WIN32   /* for Windows */
                            cmd = "del " + dzfile;
#else           /* for Linux or Mac */
                            cmd = "rm -rf " + dzfile;
#endif
                            std::system(cmd.c_str());
                        }
                    }

                    if (access(dfile.c_str(), 0) == -1 && access(dzfile.c_str(), 0) == 0)
                    {
                        /* extract '*.Z' */
                        cmd = gzipfull + " -d -f " + dzfile;
                        std::system(cmd.c_str());
                        isgz = false;
                    }

                    char tmpfile[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpfile, "%s%c%s", subdir.c_str(), sep, ofile.c_str());
                    std::string localfile = tmpfile;
                    if (access(dfile.c_str(), 0) == -1)
                    {
                        Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetDailyObsChi): failed to download CHI daily observation file " + ofile);

                        std::string url0 = url + '/' + dxfile;
                        if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetDailyObsChi): " << url0 << "  ->  " <<
                            localfile << "  failed" << std::endl;

                        continue;
                    }

#ifdef _WIN32  /* for Windows */
                    cmd = crx2rnxfull + " " + dfile + " -f - > " + ofile;
#else          /* for Linux or Mac */
                    cmd = "cat " + dfile + " | " + crx2rnxfull + " -f - > " + ofile;
#endif
                    std::system(cmd.c_str());

                    if (access(ofile.c_str(), 0) == 0)
                    {
                        Logger::Trace(TINFO, "*** INFO(FtpUtil::GetDailyObsChi): successfully download CHI daily observation file " + ofile);

                        if (isgz) dxfile = dgzfile;
                        else dxfile = dzfile;
                        std::string url0 = url + '/' + dxfile;
                        if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetDailyObsChi): " << url0 << "  ->  " <<
                            localfile << "  OK" << std::endl;

                        /* delete 'd' file */
#ifdef _WIN32   /* for Windows */
                        cmd = "del " + dfile;
#else           /* for Linux or Mac */
                        cmd = "rm -rf " + dfile;
#endif
                        std::system(cmd.c_str());
                    }
                }
                else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetDailyObsChi): CHI daily observation file " +
                    ofile + " has existed!");
            }

            /* close 'site.list' */
            sitlst.close();
        }
    }
} /* end of GetDailyObsChi */

/**
* @brief   : GetNav - download daily GPS, GLONASS and mixed RINEX broadcast ephemeris files
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetNav(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);

    std::string ftpname = fopt->ftpfrom;
    CString::trim(ftpname);
    CString::ToUpper(ftpname);
    std::string nTyp(fopt->navtype);
    CString::trim(nTyp);
    CString::ToLower(nTyp);
    std::string navSys(fopt->navsys);
    CString::trim(navSys);
    CString::ToLower(navSys);
    std::string navFrom = fopt->navfrom;
    CString::trim(navFrom);
    CString::ToUpper(navFrom);
    if (nTyp == "daily")
    {
        /* change directory */
#ifdef _WIN32   /* for Windows */
        _chdir(dir.c_str());
#else           /* for Linux or Mac */
        chdir(dir.c_str());
#endif

        std::string navfile, nav0File;
        if (navSys == "gps")
        {
            /* GPS broadcast ephemeris file */
            navfile = "brdc" + sdoy + "0." + syy + "n";
            nav0File = "brdc" + sdoy + "0." + syy + "n";
        }
        else if (navSys == "glo")
        {
            /* GLONASS broadcast ephemeris file */
            navfile = "brdc" + sdoy + "0." + syy + "g";
            nav0File = "brdc" + sdoy + "0." + syy + "g";
        }
        else if (navSys == "mixed3")
        {
            /* multi-GNSS broadcast ephemeris file in RINEX 3.xx format */
            navfile = "BRDC00" + navFrom + "_R_" + syyyy + sdoy + "0000_01D_MN.rnx";
            nav0File = "brdm" + sdoy + "0." + syy + "p";
        }
        else if (navSys == "mixed4")
        {
            /* multi-GNSS broadcast ephemeris file in RINEX 4.xx format */
            navfile = "BRD400DLR_S_" + syyyy + sdoy + "0000_01D_MN.rnx";
            nav0File = "brd4" + sdoy + "0." + syy + "p";
        }

        if (access(navfile.c_str(), 0) == -1 && access(nav0File.c_str(), 0) == -1)
        {
            std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, qr = fopt->qr;
            std::string url, cutdirs = " --cut-dirs=6 ";
            if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_NAV] + "/" +
                syyyy + "/brdc";
            else if (ftpname == "IGN")
            {
                url = ftparchive_.IGN[IDX_NAV] + "/" + syyyy + "/" + sdoy;
                cutdirs = " --cut-dirs=5 ";
            }
            else if (ftpname == "WHU")
            {
                if (yyyy >= 2020) url = ftparchive_.WHU[IDX_NAV] + "/" + syyyy + "/brdc";
                else
                {
                    if (navSys == "gps") url = ftparchive_.WHU[IDX_NAV] + "/" + syyyy + "/" + sdoy + "/" + syy + "n";
                    else if (navSys == "glo") url = ftparchive_.WHU[IDX_NAV] + "/" + syyyy + "/" + sdoy + "/" + syy + "g";
                    if (navSys == "mixed3") url = ftparchive_.WHU[IDX_NAV] + "/" + syyyy + "/" + sdoy + "/" + syy + "p";
                }
                cutdirs = " --cut-dirs=7 ";
            }
            else url = ftparchive_.CDDIS[IDX_NAV] + "/" + syyyy + "/brdc";

            if (navSys == "mixed3")
            {
                if (navFrom == "DLR")
                {
                    navfile = "BRDM00" + navFrom + "_S_" + syyyy + sdoy + "0000_01D_MN.rnx";
                    if (ftpname != "CDDIS" && ftpname != "WHU")
                    {
                        Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetNav): DLR mixed broadcast ephemeris file is ONLY downloaded from CDDIS or WHU FTP!");
                        return;
                    }
                }
                else if (navFrom == "IGN")
                {
                    if (ftpname != "IGN")
                    {
                        Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetNav): IGN mixed broadcast ephemeris file is ONLY downloaded from IGN FTP!");
                        return;
                    }
                }
                else if (navFrom == "GOP")
                {
                    url = "ftp://ftp.pecny.cz/LDC/orbits_brd/gop3/" + syyyy;
                    cutdirs = " --cut-dirs=4 ";
                }
                else if (navFrom == "WRD")
                {
                    url = "https://igs.bkg.bund.de/root_ftp/IGS/BRDC/" + syyyy + "/" + sdoy;
                    cutdirs = " --cut-dirs=5 ";
                }
            }

            /* it is OK for '*.Z' or '*.gz' format */
            std::string cmd, url0, navxFile;
            if (navFrom == "WRD")
            {
                std::string navgzfile = navfile + ".gz";
                url0 = url + "/" + navgzfile;
                cmd = wgetfull + " " + qr + " -nH " + cutdirs + url0;
            }
            else
            {
                navxFile = navfile + ".*";
                cmd = wgetfull + " " + qr + " -nH -A " + navxFile + cutdirs + url;
                url0 = url + "/" + navxFile;
            }
            std::system(cmd.c_str());

            std::string navgzfile = navfile + ".gz", navzfile = navfile + ".Z";
            bool isgz = false;
            if (access(navgzfile.c_str(), 0) == 0)
            {
                /* extract '*.gz' */
                cmd = gzipfull + " -d -f " + navgzfile;
                std::system(cmd.c_str());
                isgz = true;
            }
            else if (access(navzfile.c_str(), 0) == 0)
            {
                /* extract '*.Z' */
                cmd = gzipfull + " -d -f " + navzfile;
                std::system(cmd.c_str());
                isgz = false;
            }

            char tmpfile[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            sprintf(tmpfile, "%s%c%s", dir.c_str(), sep, navfile.c_str());
            std::string localfile = tmpfile;
            if (access(navfile.c_str(), 0) == -1)
            {
                Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetNav): failed to download broadcast ephemeris file " + navfile);

                if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetNav): " << url0 << "  ->  " <<
                    localfile << "  failed" << std::endl;
            }

            /* to check if it needs to convert broadcast ehemeris files with long name to those with short name */
            if (fopt->l2s4nav > 0 && (navSys == "mixed3" || navSys == "mixed4"))
            {
                std::string change_filename;
#ifdef _WIN32  /* for Windows */
                if (fopt->l2s4nav == 1) change_filename = "rename";
                else if (fopt->l2s4nav == 2) change_filename = "copy";
#else          /* for Linux or Mac */
                if (fopt->l2s4nav == 1) change_filename = "mv";
                else if (fopt->l2s4nav == 2) change_filename = "cp";
#endif
                cmd = change_filename + " " + navfile + " " + nav0File;
                std::system(cmd.c_str());
            }

            if (navFrom == "GOP")
            {
                /* delete some temporary directories */
                std::string tmpdir = "log";
                if (access(tmpdir.c_str(), 0) == 0)
                {
#ifdef _WIN32  /* for Windows */
                    cmd = "rd /s /q " + tmpdir;
#else          /* for Linux or Mac */
                    cmd = "rm -rf " + tmpdir;
#endif
                    std::system(cmd.c_str());
                }
            }

            if (access(navfile.c_str(), 0) == 0)
            {
                Logger::Trace(TINFO, "*** INFO(FtpUtil::GetNav): successfully download broadcast ephemeris file " + navfile);

                if (isgz) navxFile = navgzfile;
                else navxFile = navzfile;
                std::string url0 = url + '/' + navxFile;
                if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetNav): " << url0 << "  ->  " <<
                    localfile << "  OK" << std::endl;
            }
        }
        else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetNav): broadcast ephemeris file " + navfile +
            " or " + nav0File + " has existed!");
    }
    else if (nTyp == "hourly")
    {
        std::string sitfile = fopt->navlist;
        /* download the broadcast ephemeris file site-by-site */
        if (access(sitfile.c_str(), 0) == 0)
        {
            std::ifstream sitlst(sitfile.c_str());
            if (!sitlst.is_open())
            {
                Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetHourlyObsMgex): open site.list = " + sitfile +
                    " file failed, please check it");
                return;
            }

            std::string sitname;
            while (getline(sitlst, sitname))
            {
                if (sitname[0] == '#') continue;
                CString::trim(sitname);
                if (sitname.size() != 4) continue;
                for (size_t i = 0; i < fopt->hhnav.size(); i++)
                {
                    std::string shh = CString::int2str(fopt->hhnav[i], 2);
                    char tmpdir[MAXSTRPATH] = { '\0' };
                    char sep = (char)FILEPATHSEP;
                    sprintf(tmpdir, "%s%c%s", dir.c_str(), sep, shh.c_str());
                    std::string shhdir = tmpdir;
                    if (access(shhdir.c_str(), 0) == -1)
                    {
                        /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                        std::string cmd = "mkdir " + shhdir;
#else           /* for Linux or Mac */
                        std::string cmd = "mkdir -p " + shhdir;
#endif
                        std::system(cmd.c_str());
                    }

                    /* change directory */
#ifdef _WIN32   /* for Windows */
                    _chdir(shhdir.c_str());
#else           /* for Linux or Mac */
                    chdir(shhdir.c_str());
#endif

                    /* 'a' = 97, 'b' = 98, ... */
                    int ii = fopt->hhnav[i] + 97;
                    char ch = ii;
                    std::string sch;
                    sch.push_back(ch);
                    CString::ToLower(sitname);
                    std::string navnsFile = sitname + sdoy + sch + "." + syy + "n";  /* short file name */
                    std::string navgsFile = sitname + sdoy + sch + "." + syy + "g";

                    CString::ToUpper(sitname);
                    std::string navnlFile = sitname + "*_R_" + syyyy + sdoy + shh + "00_01H_GN.rnx";  /* long file name */
                    std::string navglFile = sitname + "*_R_" + syyyy + sdoy + shh + "00_01H_RN.rnx";
                    std::string navclFile = sitname + "*_R_" + syyyy + sdoy + shh + "00_01H_CN.rnx";
                    std::string navelFile = sitname + "*_R_" + syyyy + sdoy + shh + "00_01H_EN.rnx";
                    std::string navjlFile = sitname + "*_R_" + syyyy + sdoy + shh + "00_01H_JN.rnx";
                    std::string navilFile = sitname + "*_R_" + syyyy + sdoy + shh + "00_01H_IN.rnx";
                    std::string navmlFile = sitname + "*_R_" + syyyy + sdoy + shh + "00_01H_MN.rnx";
                    std::vector<std::string> navfiles;
                    if (navSys == "gps")
                    {
                        navfiles.push_back(navnsFile);
                        navfiles.push_back(navnlFile);
                    }
                    else if (navSys == "glo")
                    {
                        navfiles.push_back(navgsFile);
                        navfiles.push_back(navglFile);
                    }
                    else if (navSys == "bds") navfiles.push_back(navclFile);
                    else if (navSys == "gal") navfiles.push_back(navelFile);
                    else if (navSys == "qzs") navfiles.push_back(navjlFile);
                    else if (navSys == "irn") navfiles.push_back(navilFile);
                    else if (navSys == "mixed") navfiles.push_back(navmlFile);
                    else if (navSys == "all")
                    {
                        navfiles.push_back(navnsFile);
                        navfiles.push_back(navnlFile);
                        navfiles.push_back(navgsFile);
                        navfiles.push_back(navglFile);
                        navfiles.push_back(navclFile);
                        navfiles.push_back(navelFile);
                        navfiles.push_back(navjlFile);
                        navfiles.push_back(navilFile);
                        navfiles.push_back(navmlFile);
                    }

                    CString::ToLower(sitname);
                    std::string navns0File = sitname + sdoy + sch + "." + syy + "n";
                    std::string navgs0File = sitname + sdoy + sch + "." + syy + "g";
                    std::string navnl0File = sitname + sdoy + sch + "." + syy + "gn";
                    std::string navgl0File = sitname + sdoy + sch + "." + syy + "rn";
                    std::string navcl0File = sitname + sdoy + sch + "." + syy + "cn";
                    std::string navel0File = sitname + sdoy + sch + "." + syy + "en";
                    std::string navjl0File = sitname + sdoy + sch + "." + syy + "jn";
                    std::string navil0File = sitname + sdoy + sch + "." + syy + "in";
                    std::string navml0File = sitname + sdoy + sch + "." + syy + "mn";
                    std::vector<std::string> nav0Files;
                    if (navSys == "gps")
                    {
                        nav0Files.push_back(navns0File);
                        nav0Files.push_back(navnl0File);
                    }
                    else if (navSys == "glo")
                    {
                        nav0Files.push_back(navgs0File);
                        nav0Files.push_back(navgl0File);
                    }
                    else if (navSys == "bds") nav0Files.push_back(navcl0File);
                    else if (navSys == "gal") nav0Files.push_back(navel0File);
                    else if (navSys == "qzs") nav0Files.push_back(navjl0File);
                    else if (navSys == "irn") nav0Files.push_back(navil0File);
                    else if (navSys == "mixed") nav0Files.push_back(navml0File);
                    else if (navSys == "all")
                    {
                        nav0Files.push_back(navns0File);
                        nav0Files.push_back(navnl0File);
                        nav0Files.push_back(navgs0File);
                        nav0Files.push_back(navgl0File);
                        nav0Files.push_back(navcl0File);
                        nav0Files.push_back(navel0File);
                        nav0Files.push_back(navjl0File);
                        nav0Files.push_back(navil0File);
                        nav0Files.push_back(navml0File);
                    }

                    for (size_t i = 0; i < navfiles.size(); i++)
                    {
                        std::string url, cutdirs = " --cut-dirs=7 ";
                        if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBMH] + "/" +
                            syyyy + "/" + sdoy + "/" + shh;
                        else if (ftpname == "IGN")
                        {
                            url = ftparchive_.IGN[IDX_OBMH] + "/" + syyyy + "/" + sdoy;
                            cutdirs = " --cut-dirs=6 ";
                        }
                        else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_OBMH] + "/" +
                            syyyy + "/" + sdoy + "/" + shh;
                        else url = ftparchive_.CDDIS[IDX_OBMH] + "/" + syyyy + "/" + sdoy + "/" + shh;

                        /* it is OK for '*.Z' or '*.gz' format */
                        std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, qr = fopt->qr;
                        std::string navxFile = navfiles[i] + ".*";
                        std::string cmd = wgetfull + " " + qr + " -nH -A " + navxFile + cutdirs + url;
                        std::system(cmd.c_str());

                        /* extract '*.gz' */
                        std::string navgzfile = navfiles[i] + ".gz";
                        cmd = gzipfull + " -d -f " + navgzfile;
                        std::system(cmd.c_str());
                        std::string change_filename;
#ifdef _WIN32  /* for Windows */
                        change_filename = "rename";
#else          /* for Linux or Mac */
                        change_filename = "mv";
#endif
                        cmd = change_filename + " " + navfiles[i] + " " + nav0Files[i];
                        std::system(cmd.c_str());
                        bool isgz = true;
                        std::string navzfile;
                        char tmpfile[MAXSTRPATH] = { '\0' };
                        char sep = (char)FILEPATHSEP;
                        sprintf(tmpfile, "%s%c%s", shhdir.c_str(), sep, nav0Files[i].c_str());
                        std::string localfile = tmpfile;
                        if (access(nav0Files[i].c_str(), 0) == -1)
                        {
                            /* extract '*.Z' */
                            navzfile = navfiles[i] + ".Z";
                            cmd = gzipfull + " -d -f " + navzfile;
                            std::system(cmd.c_str());

                            cmd = change_filename + " " + navfiles[i] + " " + nav0Files[i];
                            std::system(cmd.c_str());
                            isgz = false;
                            if (access(nav0Files[i].c_str(), 0) == 0)
                            {
                                Logger::Trace(TINFO, "*** INFO(FtpUtil::GetNav): successfully download hourly broadcast ephemeris file " + navfiles[i]);

                                if (isgz) navxFile = navgzfile;
                                else navxFile = navzfile;
                                std::string url0 = url + '/' + navxFile;
                                if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetNav): " << url0 << "  ->  " <<
                                    localfile << "  OK" << std::endl;

                                continue;
                            }
                            else
                            {
                                Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetNav): failed to download hourly broadcast ephemeris file " + navfiles[i]);

                                std::string url0 = url + '/' + navxFile;
                                if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetNav): " << url0 << "  ->  " <<
                                    localfile << "  failed" << std::endl;
                            }
                        }
                        else
                        {
                            Logger::Trace(TINFO, "*** INFO(FtpUtil::GetNav): successfully download hourly broadcast ephemeris file " + navfiles[i]);

                            if (isgz) navxFile = navgzfile;
                            else navxFile = navzfile;
                            std::string url0 = url + '/' + navxFile;
                            if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetNav): " << url0 << "  ->  " <<
                                localfile << "  OK" << std::endl;

                            continue;
                        }
                    }
                }
            }

            /* close 'site.list' */
            sitlst.close();
        }
    }
} /* end of GetNav */

/**
* @brief   : GetOrbClk - download IGS/MGEX precise orbit and clock files
* @param[I]: ts (start time)
* @param[I]: dirs (orbit and clock directories)
* @param[I]: prodType (1:real-time products; 2:ultra-rapid; 3:rapid; 4: IGS final; 5: MGEX final)
* @param[I]: ac (analysis center, i.e., 'igs', 'cod', et al.)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetOrbClk(gtime_t ts, std::vector<std::string> dirs, int prodType, std::string ac, const ftpopt_t* fopt)
{
    /* compute GPS week and day of week */
    int wwww = 0;
    double sow = 0.0;
    int dow = GTime::time2gpst(ts, wwww, sow);
    std::string swwww = CString::int2str(wwww, 4);
    std::string sdow(std::to_string(dow));
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string sdoy = CString::int2str(doy, 3);

    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string ftpname = fopt->ftpfrom;
    CString::trim(ftpname);
    CString::ToUpper(ftpname);
    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, qr = fopt->qr;
    CString::trim(ac);
    CString::ToLower(ac);
    if (prodType == PROD_REALTIME)          /* for CNES real-time orbit and clock products from CNES offline files  */
    {
        std::string sp3file = ac + swwww + sdow + ".sp3", clkfile = ac + swwww + sdow + ".clk";
        std::vector<std::string> sp3clkfiles = { sp3file, clkfile };
        std::string sp3gzfile = sp3file + ".gz", clkgzfile = clkfile + ".gz";
        std::vector<std::string> sp3clkgzfiles = { sp3gzfile, clkgzfile };
        for (size_t i = 0; i < sp3clkfiles.size(); i++)
        {
            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(dirs[i].c_str());
#else           /* for Linux or Mac */
            chdir(dirs[i].c_str());
#endif

            if (access(sp3clkfiles[i].c_str(), 0) == -1)
            {
                std::string url = "http://www.ppp-wizard.net/products/REAL_TIME/" + sp3clkgzfiles[i];
                std::string cmd = wgetfull + " " + qr + " -nH --cut-dirs=2 " + url;
                std::system(cmd.c_str());

                /* extract '*.gz' */
                cmd = gzipfull + " -d -f " + sp3clkgzfiles[i];
                std::system(cmd.c_str());

                std::string localfile = dirs[i] + sep + sp3clkfiles[i];
                if (access(sp3clkfiles[i].c_str(), 0) == 0)
                {
                    if (i == 0) Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOrbClk): successfully download CNES real-time precise orbit file " + sp3clkfiles[i]);
                    else if (i == 1) Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOrbClk): successfully download CNES real-time precise clock file " + sp3clkfiles[i]);


                    if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetOrbClk): " << url << "  ->  " <<
                        localfile << "  OK" << std::endl;
                }
                else
                {
                    if (i == 0) Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetOrbClk): failed to download CNES real-time precise orbit file " + sp3clkfiles[i]);
                    else if (i == 1) Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetOrbClk): failed to download CNES real-time precise clock file " + sp3clkfiles[i]);

                    if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetOrbClk): " << url << "  ->  " <<
                        localfile << "  failed" << std::endl;
                }

                /* delete some temporary directories */
                std::vector<std::string> tmpdir = { "FORMAT_BIAIS_OFFI1", "FORMATBIAS_OFF_v1" };
                for (size_t i = 0; i < tmpdir.size(); i++)
                {
                    if (access(tmpdir[i].c_str(), 0) == 0)
                    {
#ifdef _WIN32  /* for Windows */
                        cmd = "rd /s /q " + tmpdir[i];
#else          /* for Linux or Mac */
                        cmd = "rm -rf " + tmpdir[i];
#endif
                        std::system(cmd.c_str());
                    }
                }
            }
            else
            {
                if (i == 0) Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOrbClk): CNES real-time precise orbit file " +
                    sp3clkfiles[i] + " has existed!");
                else if (i == 1) Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOrbClk): CNES real-time precise clock file " +
                    sp3clkfiles[i] + " has existed!");
            }
        }
    }
    else if (prodType == PROD_ULTRA_RAPID)  /* for ultra-rapid orbit and clock products  */
    {
        /* change directory */
#ifdef _WIN32   /* for Windows */
        _chdir(dirs[0].c_str());
#else           /* for Linux or Mac */
        chdir(dirs[0].c_str());
#endif

        std::string url, cutdirs, acname, acFile;
        int idx = 0;
        if (ac == "esa_u")       /* ESA */
        {
            url = "http://navigation-office.esa.int/products/gnss-products/" + swwww;
            cutdirs = " --cut-dirs=3 ";
            acFile = "esu";
            acname = "ESA";
            idx = 0;
        }
        else if (ac == "gfz_u")  /* GFZ */
        {
            url = "ftp://ftp.gfz-potsdam.de/pub/GNSS/products/ultra/w" + swwww;
            cutdirs = " --cut-dirs=5 ";
            acFile = "gfu";
            acname = "GFZ";
            idx = 1;
        }
        else if (ac == "igs_u")  /* IGS */
        {
            if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_SP3] + "/" + swwww;
            else if (ftpname == "IGN") url = ftparchive_.IGN[IDX_SP3] + "/" + swwww;
            else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_SP3] + "/" + swwww;
            else url = ftparchive_.CDDIS[IDX_SP3] + "/" + swwww;
            cutdirs = " --cut-dirs=4 ";
            acFile = "igu";
            acname = "IGS";
            idx = 2;
        }
        else if (ac == "whu_u")  /* WHU */
        {
            if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_SP3M] + "/" + swwww;
            else if (ftpname == "IGN") url = ftparchive_.IGN[IDX_SP3M] + "/" + swwww;
            else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_SP3M] + "/" + swwww;
            else url = ftparchive_.CDDIS[IDX_SP3M] + "/" + swwww;
            cutdirs = " --cut-dirs=5 ";
            acname = "WHU";
            acFile = "wuu";
            idx = 3;
        }

        for (size_t i = 0; i < fopt->hhorbclk[idx].size(); i++)
        {
            std::string shh = CString::int2str(fopt->hhorbclk[idx][i], 2);
            std::string sp3fileshort = acFile + swwww + sdow + "_" + shh + ".sp3";
            if (access(sp3fileshort.c_str(), 0) == -1)
            {
                std::string cmd, url0, sp3zfile, sp3gzfile, sp3xfile;
                bool isgz = false;
                if (ac == "esa_u")  /* ESA */
                {
                    sp3zfile = sp3fileshort + ".Z";
                    url0 = url + "/" + sp3zfile;
                    cmd = wgetfull + " " + qr + " -nH " + cutdirs + url0;
                    std::system(cmd.c_str());

                    if (access(sp3zfile.c_str(), 0) == 0)
                    {
                        /* extract '*.Z' */
                        cmd = gzipfull + " -d -f " + sp3zfile;
                        std::system(cmd.c_str());
                        isgz = false;
                    }
                    else
                    {
                        sp3gzfile = sp3fileshort + ".gz";
                        url0 = url + "/" + sp3gzfile;
                        cmd = wgetfull + " " + qr + " -nH " + cutdirs + url0;
                        std::system(cmd.c_str());

                        if (access(sp3gzfile.c_str(), 0) == 0)
                        {
                            /* extract '*.gz' */
                            cmd = gzipfull + " -d -f " + sp3gzfile;
                            std::system(cmd.c_str());
                            isgz = true;
                        }
                    }
                }
                else if (ac == "whu_u")  /* WHU */
                {
                    std::string sp30File = "WUM0MGXULA_" + syyyy + sdoy + shh + "00_01D_*_ORB.SP3";
                    /* it is OK for '*.Z' or '*.gz' format */
                    sp3xfile = sp30File + ".*";
                    cmd = wgetfull + " " + qr + " -nH -A " + sp3xfile + cutdirs + url;
                    std::system(cmd.c_str());

                    /* extract '*.gz' */
                    sp3gzfile = sp30File + ".gz";
                    cmd = gzipfull + " -d -f " + sp3gzfile;
                    std::system(cmd.c_str());
                    std::string change_filename;
#ifdef _WIN32  /* for Windows */
                    change_filename = "rename";
#else          /* for Linux or Mac */
                    change_filename = "mv";
#endif
                    cmd = change_filename + " " + sp30File + " " + sp3fileshort;
                    std::system(cmd.c_str());
                    isgz = true;
                    if (access(sp3fileshort.c_str(), 0) == -1)
                    {
                        /* extract '*.Z' */
                        sp3zfile = sp30File + ".Z";
                        cmd = gzipfull + " -d -f " + sp3zfile;
                        std::system(cmd.c_str());

                        cmd = change_filename + " " + sp30File + " " + sp3fileshort;
                        std::system(cmd.c_str());
                        isgz = false;
                    }

                    url0 = url + '/' + sp3xfile;
                }
                else
                {
                    /* it is OK for '*.Z' or '*.gz' format */
                    sp3xfile = sp3fileshort + ".*";
                    cmd = wgetfull + " " + qr + " -nH -A " + sp3xfile + cutdirs + url;
                    std::system(cmd.c_str());

                    sp3gzfile = sp3fileshort + ".gz";
                    sp3zfile = sp3fileshort + ".Z";
                    if (access(sp3gzfile.c_str(), 0) == 0)
                    {
                        /* extract '*.gz' */
                        cmd = gzipfull + " -d -f " + sp3gzfile;
                        std::system(cmd.c_str());
                        isgz = true;
                    }
                    if (access(sp3fileshort.c_str(), 0) == -1 && access(sp3zfile.c_str(), 0) == 0)
                    {
                        /* extract '*.Z' */
                        cmd = gzipfull + " -d -f " + sp3zfile;
                        std::system(cmd.c_str());
                        isgz = false;
                    }

                    url0 = url + '/' + sp3xfile;
                }

                std::string localfile = dirs[0] + sep + sp3fileshort;
                if (access(sp3fileshort.c_str(), 0) == 0)
                {
                    Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOrbClk): successfully download " + acname +
                        " ultra-rapid precise orbit file " + sp3fileshort);

                    if (isgz) sp3xfile = sp3gzfile;
                    else sp3xfile = sp3zfile;
                    url0 = url + '/' + sp3xfile;
                    if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetOrbClk): " << url0 << "  ->  " <<
                        localfile << "  OK" << std::endl;
                }
                else
                {
                    Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetOrbClk): failed to download " + acname +
                        " ultra-rapid precise orbit file " + sp3fileshort);

                    if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetOrbClk): " << url0 << "  ->  " <<
                        localfile << "  failed" << std::endl;
                }

                if (ac == "igs_u")
                {
                    /* delete some temporary directories */
                    std::string tmpdir = "repro3";
                    if (access(tmpdir.c_str(), 0) == 0)
                    {
#ifdef _WIN32  /* for Windows */
                        cmd = "rd /s /q " + tmpdir;
#else          /* for Linux or Mac */
                        cmd = "rm -rf " + tmpdir;
#endif
                        std::system(cmd.c_str());
                    }
                }
            }
            else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOrbClk): " + acname + " ultra-rapid precise orbit file " +
                sp3fileshort + " has existed!");
        }
    }
    else if (prodType == PROD_RAPID)  /* for rapid orbit and clock products  */
    {
        std::string url, cutdirs, acname;
        std::string sp3file, clkfile;
        if (ac == "cod_r")  /* CODE */
        {
            sp3file = "COD" + swwww + sdow + ".EPH_M";
            clkfile = "COD" + swwww + sdow + ".CLK_M";
            url = "ftp://ftp.aiub.unibe.ch/CODE/" + syyyy + "_M";
            cutdirs = " --cut-dirs=2 ";
            acname = "CODE";
        }
        else if (ac == "emr_r")  /* NRCan */
        {
            sp3file = "emr" + swwww + sdow + ".sp3";
            clkfile = "emr" + swwww + sdow + ".clk";
            url = "ftp://rtopsdata1.geod.nrcan.gc.ca/gps/products/rapid/" + swwww;
            cutdirs = " --cut-dirs=4 ";
            acname = "NRCan";
        }
        else if (ac == "esa_r")  /* ESA */
        {
            sp3file = "esr" + swwww + sdow + ".sp3";
            clkfile = "esr" + swwww + sdow + ".clk";
            url = "http://navigation-office.esa.int/products/gnss-products/" + swwww;
            cutdirs = " --cut-dirs=3 ";
            acname = "ESA";
        }
        else if (ac == "gfz_r")  /* GFZ */
        {
            sp3file = "gfz" + swwww + sdow + ".sp3";
            clkfile = "gfz" + swwww + sdow + ".clk";
            url = "ftp://ftp.gfz-potsdam.de/pub/GNSS/products/rapid/w" + swwww;
            cutdirs = " --cut-dirs=5 ";
            acname = "GFZ";
        }
        else if (ac == "igs_r")  /* IGS */
        {
            sp3file = "igr" + swwww + sdow + ".sp3";
            clkfile = "igr" + swwww + sdow + ".clk";
            if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_SP3] + "/" + swwww;
            else if (ftpname == "IGN") url = ftparchive_.IGN[IDX_SP3] + "/" + swwww;
            else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_SP3] + "/" + swwww;
            else url = ftparchive_.CDDIS[IDX_SP3] + "/" + swwww;
            cutdirs = " --cut-dirs=4 ";
            acname = "IGS";
        }

        std::vector<std::string> sp3clkfiles = { sp3file, clkfile };
        std::string sp3gzfile = sp3file + ".gz", clkgzfile = clkfile + ".gz";
        std::vector<std::string> sp3clkgzfiles = { sp3gzfile, clkgzfile };
        std::string sp3zfile = sp3file + ".Z", clkzfile = clkfile + ".Z";
        std::vector<std::string> sp3clkzfiles = { sp3zfile, clkzfile };
        std::string sp3xfile = sp3file + ".*", clkxfile = clkfile + ".*";
        std::vector<std::string> sp3clkxfiles = { sp3xfile, clkxfile };
        for (size_t i = 0; i < sp3clkfiles.size(); i++)
        {
            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(dirs[i].c_str());
#else           /* for Linux or Mac */
            chdir(dirs[i].c_str());
#endif

            if (access(sp3clkfiles[i].c_str(), 0) == -1)
            {
                std::string cmd, url0;
                bool isgz = false;
                if (ac == "esa_r")  /* ESA */
                {
                    url0 = url + "/" + sp3clkzfiles[i];
                    cmd = wgetfull + " " + qr + " -nH " + cutdirs + url0;
                    std::system(cmd.c_str());

                    if (access(sp3zfile.c_str(), 0) == 0)
                    {
                        /* extract '*.Z' */
                        cmd = gzipfull + " -d -f " + sp3clkzfiles[i];
                        std::system(cmd.c_str());
                        isgz = false;
                    }
                    else
                    {
                        url0 = url + "/" + sp3clkgzfiles[i];
                        cmd = wgetfull + " " + qr + " -nH " + cutdirs + url0;
                        std::system(cmd.c_str());

                        if (access(sp3clkgzfiles[i].c_str(), 0) == 0)
                        {
                            /* extract '*.gz' */
                            cmd = gzipfull + " -d -f " + sp3clkgzfiles[i];
                            std::system(cmd.c_str());
                            isgz = true;
                        }
                    }
                }
                else
                {
                    /* it is OK for '*.Z' or '*.gz' format */
                    cmd = wgetfull + " " + qr + " -nH -A " + sp3clkxfiles[i] + cutdirs + url;
                    std::system(cmd.c_str());

                    if (access(sp3clkgzfiles[i].c_str(), 0) == 0)
                    {
                        /* extract '*.gz' */
                        cmd = gzipfull + " -d -f " + sp3clkgzfiles[i];
                        std::system(cmd.c_str());
                        isgz = true;

                        if (access(sp3clkfiles[i].c_str(), 0) == 0 && access(sp3clkzfiles[i].c_str(), 0) == 0)
                        {
                            /* delete '*.Z' file */
#ifdef _WIN32   /* for Windows */
                            cmd = "del " + sp3clkzfiles[i];
#else           /* for Linux or Mac */
                            cmd = "rm -rf " + sp3clkzfiles[i];
#endif
                            std::system(cmd.c_str());
                        }
                    }
                    if (access(sp3clkfiles[i].c_str(), 0) == -1 && access(sp3clkzfiles[i].c_str(), 0) == 0)
                    {
                        /* extract '*.Z' */
                        cmd = gzipfull + " -d -f " + sp3clkzfiles[i];
                        std::system(cmd.c_str());
                        isgz = false;
                    }

                    url0 = url + "/" + sp3clkxfiles[i];
                }

                std::string localfile = dirs[i] + sep + sp3clkfiles[i];
                if (access(sp3clkfiles[i].c_str(), 0) == 0)
                {
                    if (i == 0) Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOrbClk): successfully download " + acname +
                        " rapid precise orbit file " + sp3clkfiles[i]);
                    else if (i == 1) Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOrbClk): successfully download " + acname +
                        " rapid precise clock file " + sp3clkfiles[i]);

                    std::string sp3clkxfile;
                    if (isgz) sp3clkxfile = sp3clkgzfiles[i];
                    else sp3clkxfile = sp3clkzfiles[i];
                    std::string url0 = url + '/' + sp3clkxfile;
                    if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetOrbClk): " << url0 << "  ->  " <<
                        localfile << "  OK" << std::endl;
                }
                else
                {
                    if (i == 0) Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetOrbClk): failed to download " +
                        acname + " rapid precise orbit file " + sp3clkfiles[i]);
                    else if (i == 1) Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetOrbClk): failed to download " +
                        acname + " rapid precise clock file " + sp3clkfiles[i]);

                    if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetOrbClk): " << url0 << "  ->  " <<
                        localfile << "  failed" << std::endl;
                }

                if (ac == "emr_r" || ac == "igs_r")
                {
                    /* delete some temporary directories */
                    std::string tmpdir;
                    if (ac == "emr_r") tmpdir = "dcm";
                    else if (ac == "igs_r") tmpdir = "repro3";
                    if (access(tmpdir.c_str(), 0) == 0)
                    {
#ifdef _WIN32  /* for Windows */
                        cmd = "rd /s /q " + tmpdir;
#else          /* for Linux or Mac */
                        cmd = "rm -rf " + tmpdir;
#endif
                        std::system(cmd.c_str());
                    }
                }
            }
            else
            {
                if (i == 0) Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOrbClk): " + acname +
                    " rapid precise orbit file " + sp3clkfiles[i] + " has existed!");
                else if (i == 1) Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOrbClk): " + acname +
                    " rapid precise clock file " + sp3clkfiles[i] + " has existed!");
            }
        }
    }
    else if (prodType == PROD_FINAL_IGS)  /* for IGS final orbit and clock products  */
    {
        std::string sp3filelong, clkfilelong, sp3fileshort, clkfileshort, substr, acname;
        if (ac == "igs")
        {
            sp3fileshort = ac + swwww + sdow + ".sp3";
            clkfileshort = ac + swwww + sdow + ".clk_30s";
            sp3filelong= "IGS0OPSFIN_" + syyyy + sdoy + "0000_01D_15M_ORB.SP3";
            clkfilelong= "IGS0OPSFIN_" + syyyy + sdoy + "0000_01D_30S_CLK.CLK";
            acname = "IGS";
        }
        else if (ac == "cod")
        {
            sp3fileshort = ac + swwww + sdow + ".eph";
            clkfileshort = ac + swwww + sdow + ".clk_05s";
            sp3filelong  = "COD0OPSFIN_" + syyyy + sdoy + "0000_01D_05M_ORB.SP3";
            clkfilelong  = "COD0OPSFIN_" + syyyy + sdoy + "0000_01D_05S_CLK.CLK";
            acname = "CODE";
        }
        else if (ac == "emr")
        {
            sp3fileshort = ac + swwww + sdow + ".sp3";
            clkfileshort = ac + swwww + sdow + ".clk";
            sp3filelong = "EMR0OPSRAP_" + syyyy + sdoy + "0000_01D_15M_ORB.SP3";
            clkfilelong = "EMR0OPSRAP_" + syyyy + sdoy + "0000_01D_05M_CLK.CLK";
            acname = "NRCan";
        }
        else if (ac == "esa")
        {
            sp3fileshort = ac + swwww + sdow + ".sp3";
            clkfileshort = ac + swwww + sdow + ".clk";
            sp3filelong = "ESA0OPSFIN_" + syyyy + sdoy + "0000_01D_05M_ORB.SP3";
            clkfilelong = "ESA0OPSFIN_" + syyyy + sdoy + "0000_01D_30S_CLK.CLK";
            acname = "ESA";
        }
        else if (ac == "gfz")
        {
            sp3fileshort= ac+ swwww + sdow + ".sp3";
            clkfileshort= ac + swwww + sdow + ".clk";
            sp3filelong = "GFZ0OPSFIN_" + syyyy + sdoy + "0000_01D_05M_ORB.SP3";
            clkfilelong = "GFZ0OPSFIN_" + syyyy + sdoy + "0000_01D_30S_CLK.CLK";
            acname = "GFZ";
        }
        else if (ac == "grg")
        {
            sp3fileshort = ac + swwww + sdow + ".sp3";
            clkfileshort = ac + swwww + sdow + ".clk";
            sp3filelong = "GRG0OPSFIN_" + syyyy + sdoy + "0000_01D_05M_ORB.SP3";
            clkfilelong = "GRG0OPSFIN_" + syyyy + sdoy + "0000_01D_30S_CLK.CLK";
            acname = "CNES";
        }
        else if (ac == "jgx")
        {
            sp3fileshort = ac + swwww + sdow + ".sp3";
            clkfileshort = ac + swwww + sdow + ".clk";
            sp3filelong = "JGX0OPSFIN_" + syyyy + sdoy + "0000_01D_05M_ORB.SP3";
            clkfilelong = "JGX0OPSFIN_" + syyyy + sdoy + "0000_01D_30S_CLK.CLK";
            acname = "JGX";
        }
        else if (ac == "jpl")
        {
            sp3fileshort = ac + swwww + sdow + ".sp3";
            clkfileshort = ac + swwww + sdow + ".clk";
            sp3filelong = "JPL0OPSFIN_" + syyyy + sdoy + "0000_01D_05M_ORB.SP3";
            clkfilelong = "JPL0OPSFIN_" + syyyy + sdoy + "0000_01D_30S_CLK.CLK";
            acname = "JPL";
        }
        else if (ac == "mit")
        {
            sp3fileshort = ac + swwww + sdow + ".sp3";
            clkfileshort = ac + swwww + sdow + ".clk";
            sp3filelong = "MIT0OPSFIN_" + syyyy + sdoy + "0000_01D_05M_ORB.SP3";
            clkfilelong = "MIT0OPSFIN_" + syyyy + sdoy + "0000_01D_05M_CLK.CLK";
            acname = "MIT";
        }

        std::vector<std::string> sp3clkfilelong = { sp3filelong, clkfilelong };
        std::vector<std::string> sp3clkxfilelong = { sp3filelong + ".*", clkfilelong + ".*" };
        std::vector<std::string> sp3clkfileshort = { sp3fileshort, clkfileshort };
        std::vector<std::string> sp3clkxfileshort = { sp3fileshort + ".*", clkfileshort + ".*" };

        std::string url, cutdirs = " --cut-dirs=4 ";
        if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_SP3] + "/" + swwww;
        else if (ftpname == "IGN") url = ftparchive_.IGN[IDX_SP3] + "/" + swwww;
        else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_SP3] + "/" + swwww;
        else url = ftparchive_.CDDIS[IDX_SP3] + "/" + swwww;
        for (size_t i = 0; i < sp3clkfilelong.size(); i++)
        {
            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(dirs[i].c_str());
#else           /* for Linux or Mac */
            chdir(dirs[i].c_str());
#endif

            int isfound = 0;  /* 1: long file name; 2: short filename */
            if (access(sp3clkfilelong[i].c_str(), 0) == -1 && access(sp3clkfileshort[i].c_str(), 0) == -1)
            {
                /* it is OK for '*.Z' or '*.gz' format */
                std::string cmd = wgetfull + " " + qr + " -nH -A " + sp3clkxfilelong[i] + cutdirs + url;
                std::system(cmd.c_str());

                std::string sp3clkxfile, sp3clkfile;
                CString::GetFile(dirs[i], sp3clkfilelong[i], sp3clkxfile);
                if (access(sp3clkxfile.c_str(), 0) == 0)
                {
                    sp3clkfile = sp3clkfilelong[i];
                    isfound = 1;
                }
                else
                {
                    /* it is OK for '*.Z' or '*.gz' format */
                    cmd = wgetfull + " " + qr + " -nH -A " + sp3clkxfileshort[i] + cutdirs + url;
                    std::system(cmd.c_str());

                    CString::GetFile(dirs[i], sp3clkfileshort[i], sp3clkxfile);
                    if (access(sp3clkxfile.c_str(), 0) == 0)
                    {
                        sp3clkfile = sp3clkfileshort[i];
                        isfound = 2;
                    }
                }

                if (isfound == 0)
                {
                    if (i == 0) Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetOrbClk): failed to download " +
                        acname + " final precise orbit file " + sp3clkfile);
                    else if (i == 1) Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetOrbClk): failed to download " +
                        acname + " final precise clock file " + sp3clkfile);

                    std::string localfile = dirs[i] + sep + sp3clkfile;
                    std::string url0 = url + '/' + sp3clkfile;
                    if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetOrbClk): " << url0 << "  ->  " <<
                        localfile << "  failed" << std::endl;
                }
                else
                {
                    /* extract '*.gz' or '*.Z' */
                    cmd = gzipfull + " -d -f " + sp3clkxfile;
                    std::system(cmd.c_str());
                    if (access(sp3clkfile.c_str(), 0) == 0)
                    {
                        std::string localfile = dirs[i] + sep + sp3clkfile;
                        if (i == 0) Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOrbClk): successfully download " +
                            acname + " final precise orbit file " + sp3clkfile);
                        else if (i == 1) Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOrbClk): successfully download " +
                            acname + " final precise clock file " + sp3clkfile);

                        std::string url0 = url + '/' + sp3clkxfile;
                        if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetOrbClk): " << url0 << "  ->  " <<
                            localfile << "  OK" << std::endl;

                        /* to check if it needs to convert precise orbit and clock files with long name to those with short name */
                        if (isfound == 1 && fopt->l2s4oc > 0)
                        {
                            std::string change_filename;
#ifdef _WIN32  /* for Windows */
                            if (fopt->l2s4oc == 1) change_filename = "rename";
                            else if (fopt->l2s4oc == 2) change_filename = "copy";
#else          /* for Linux or Mac */
                            if (fopt->l2s4oc == 1) change_filename = "mv";
                            else if (fopt->l2s4oc == 2) change_filename = "cp";
#endif
                            cmd = change_filename + " " + sp3clkfilelong[i] + " " + sp3clkfileshort[i];
                            std::system(cmd.c_str());
                        }
                    }
                }

                /* delete some temporary directories */
                std::string tmpdir = "repro3";
                if (access(tmpdir.c_str(), 0) == 0)
                {
#ifdef _WIN32  /* for Windows */
                    cmd = "rd /s /q " + tmpdir;
#else          /* for Linux or Mac */
                    cmd = "rm -rf " + tmpdir;
#endif
                    std::system(cmd.c_str());
                }
            }
            else
            {
                if (i == 0) Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOrbClk): " + acname + " final precise orbit file " +
                    sp3clkfilelong[i] + " or " + sp3clkfileshort[i] + " has existed!");
                else if (i == 1) Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOrbClk): " + acname + " final precise clock file " +
                    sp3clkfilelong[i] + " or " + sp3clkfileshort[i] + " has existed!");
            }
        }
    }
    else if (prodType == PROD_FINAL_MGEX)  /* for MGEX final orbit and clock products */
    {
        std::string sp3filelong, clkfilelong, sp3fileshort, clkfileshort, substr, acname;
        if (ac == "cod_m")
        {
            sp3filelong = "COD0MGXFIN_" + syyyy + sdoy + "0000_01D_" + "*_ORB.SP3";
            clkfilelong = "COD0MGXFIN_" + syyyy + sdoy + "0000_01D_" + "*_CLK.CLK";
            sp3fileshort = "com" + swwww + sdow + ".sp3";
            clkfileshort = "com" + swwww + sdow + ".clk";
            substr = "COD0MGXFIN_" + syyyy + sdoy + "0000_01D_";
            acname = "CODE";
        }
        else if (ac == "gfz_m")
        {
            sp3filelong = "GFZ0MGXRAP_" + syyyy + sdoy + "0000_01D_" + "*_ORB.SP3";
            clkfilelong = "GFZ0MGXRAP_" + syyyy + sdoy + "0000_01D_" + "*_CLK.CLK";
            sp3fileshort = "gbm" + swwww + sdow + ".sp3";
            clkfileshort = "gbm" + swwww + sdow + ".clk";
            substr = "GFZ0MGXRAP_" + syyyy + sdoy + "0000_01D_";
            acname = "GFZ";
        }
        else if (ac == "grg_m")
        {
            sp3filelong = "GRG0MGXFIN_" + syyyy + sdoy + "0000_01D_" + "*_ORB.SP3";
            clkfilelong = "GRG0MGXFIN_" + syyyy + sdoy + "0000_01D_" + "*_CLK.CLK";
            sp3fileshort = "grm" + swwww + sdow + ".sp3";
            clkfileshort = "grm" + swwww + sdow + ".clk";
            substr = "GRG0MGXFIN_" + syyyy + sdoy + "0000_01D_";
            acname = "CNES";
        }
        else if (ac == "iac_m")
        {
            sp3filelong = "IAC0MGXFIN_" + syyyy + sdoy + "0000_01D_" + "*_ORB.SP3";
            clkfilelong = "IAC0MGXFIN_" + syyyy + sdoy + "0000_01D_" + "*_CLK.CLK";
            sp3fileshort = "iac" + swwww + sdow + ".sp3";
            clkfileshort = "iac" + swwww + sdow + ".clk";
            substr = "IAC0MGXFIN_" + syyyy + sdoy + "0000_01D_";
            acname = "IAC";
        }
        else if (ac == "jax_m")
        {
            sp3filelong = "JAX0MGXFIN_" + syyyy + sdoy + "0000_01D_" + "*_ORB.SP3";
            clkfilelong = "JAX0MGXFIN_" + syyyy + sdoy + "0000_01D_" + "*_CLK.CLK";
            sp3fileshort = "jax" + swwww + sdow + ".sp3";
            clkfileshort = "jax" + swwww + sdow + ".clk";
            substr = "JAX0MGXFIN_" + syyyy + sdoy + "0000_01D_";
            acname = "JAXA";
        }
        else if (ac == "sha_m")
        {
            sp3filelong = "SHA0MGXRAP_" + syyyy + sdoy + "0000_01D_" + "*_ORB.SP3";
            clkfilelong = "SHA0MGXRAP_" + syyyy + sdoy + "0000_01D_30S_CLK.CLK";
            sp3fileshort = "sha" + swwww + sdow + ".sp3";
            clkfileshort = "sha" + swwww + sdow + ".clk";
            substr = "SHA0MGXRAP_" + syyyy + sdoy + "0000_01D_";
            acname = "SHAO";
        }
        else if (ac == "whu_m")
        {
            sp3filelong = "WUM0MGXFIN_" + syyyy + sdoy + "0000_01D_" + "*_ORB.SP3";
            clkfilelong = "WUM0MGXFIN_" + syyyy + sdoy + "0000_01D_" + "*_CLK.CLK";
            sp3fileshort = "wum" + swwww + sdow + ".sp3";
            clkfileshort = "wum" + swwww + sdow + ".clk";
            substr = "WUM0MGXFIN_" + syyyy + sdoy + "0000_01D_";
            acname = "WHU";
        }

        std::vector<std::string> sp3clkfilelong = { sp3filelong, clkfilelong };
        std::vector<std::string> sp3clkfileshort = { sp3fileshort, clkfileshort };
        std::string sp3xfilelong = sp3filelong + ".*", clkxfilelong = clkfilelong + ".*";
        std::vector<std::string> sp3clkxfilelong = { sp3xfilelong, clkxfilelong };

        std::string url, cutdirs = " --cut-dirs=5 ";
        if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_SP3M] + "/" + swwww;
        else if (ftpname == "IGN") url = ftparchive_.IGN[IDX_SP3M] + "/" + swwww;
        else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_SP3M] + "/" + swwww;
        else url = ftparchive_.CDDIS[IDX_SP3M] + "/" + swwww;
        for (size_t i = 0; i < sp3clkfilelong.size(); i++)
        {
            /* change directory */
#ifdef _WIN32   /* for Windows */
            _chdir(dirs[i].c_str());
#else           /* for Linux or Mac */
            chdir(dirs[i].c_str());
#endif
            std::string sp3clkfilelong;
            CString::GetFile(dirs[i], substr, sp3clkfilelong);
            if (access(sp3clkfilelong.c_str(), 0) == -1 && access(sp3clkfileshort[i].c_str(), 0) == -1)
            {
                /* it is OK for '*.Z' or '*.gz' format */
                std::string cmd = wgetfull + " " + qr + " -nH -A " + sp3clkxfilelong[i] + cutdirs + url;
                std::system(cmd.c_str());

                /* the option of 'igsdepot.ign.fr' was added by fzhou @ SDUST, 2022/11/20 22:01:05 */
                std::string sp3clkxfile;
                CString::GetFile(dirs[i], substr, sp3clkxfile);
                if (access(sp3clkxfile.c_str(), 0) == -1)
                {
                    cutdirs = " --cut-dirs=4 ";
                    if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_SP3] + "/" + swwww;
                    else if (ftpname == "IGN") url = ftparchive_.IGN[IDX_SP3] + "/" + swwww;
                    else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_SP3] + "/" + swwww;
                    else url = ftparchive_.CDDIS[IDX_SP3] + "/" + swwww;

                    /* it is OK for '*.Z' or '*.gz' format */
                    cmd = wgetfull + " " + qr + " -nH -A " + sp3clkxfilelong[i] + cutdirs + url;
                    std::system(cmd.c_str());
                }

                CString::GetFile(dirs[i], substr, sp3clkxfile);
                if (access(sp3clkxfile.c_str(), 0) == -1)
                {
                    cutdirs = " --cut-dirs=5 ";
                    std::string urlx = "ftp://igsdepot.ign.fr/pub/igs/products/mgex/" + swwww;
                    /* it is OK for '*.Z' or '*.gz' format */
                    cmd = wgetfull + " " + qr + " -nH -A " + sp3clkxfilelong[i] + cutdirs + urlx;
                    std::system(cmd.c_str());
                }

                CString::GetFile(dirs[i], substr, sp3clkxfile);
                if (access(sp3clkxfile.c_str(), 0) == -1)
                {
                    if (i == 0) Logger::Trace(TWARNING, "*** ERROR(FtpUtil::GetOrbClk): failed to download " +
                        acname + " MGEX final precise orbit file " + sp3clkfilelong[i]);
                    else if (i == 1) Logger::Trace(TWARNING, "*** ERROR(FtpUtil::GetOrbClk): failed to download " +
                        acname + " MGEX final precise clock file " + sp3clkfilelong[i]);

                    std::string localfile = dirs[i] + sep + sp3clkfilelong[i];
                    std::string url0 = url + '/' + sp3clkxfilelong[i];
                    if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetOrbClk): " << url0 << "  ->  " <<
                        localfile << "  failed" << std::endl;
                }
                else
                {
                    /* extract '*.gz' or '*.Z' */
                    cmd = gzipfull + " -d -f " + sp3clkxfile;
                    std::system(cmd.c_str());

                    std::string sp3clkfile;
                    CString::GetFile(dirs[i], substr, sp3clkfile);
                    std::string localfile = dirs[i] + sep + sp3clkfile;
                    if (access(sp3clkfile.c_str(), 0) == 0)
                    {
                        if (i == 0) Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOrbClk): successfully download " +
                            acname + " final precise orbit file " + sp3clkfile);
                        else if (i == 1) Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOrbClk): successfully download " +
                            acname + " final precise clock file " + sp3clkfile);

                        std::string url0 = url + '/' + sp3clkxfile;
                        if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetOrbClk): " << url0 << "  ->  " <<
                            localfile << "  OK" << std::endl;

                        /* to check if it needs to convert precise orbit and clock files with long name to those with short name */
                        if (fopt->l2s4oc > 0)
                        {
                            std::string change_filename;
#ifdef _WIN32  /* for Windows */
                            if (fopt->l2s4oc == 1) change_filename = "rename";
                            else if (fopt->l2s4oc == 2) change_filename = "copy";
#else          /* for Linux or Mac */
                            if (fopt->l2s4oc == 1) change_filename = "mv";
                            else if (fopt->l2s4oc == 2) change_filename = "cp";
#endif
                            cmd = change_filename + " " + sp3clkfile + " " + sp3clkfileshort[i];
                            std::system(cmd.c_str());
                        }
                    }
                }
            }
            else
            {
                if (i == 0) Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOrbClk): " + acname + " MGEX final precise orbit file " +
                    sp3clkfilelong[i] + " or " + sp3clkfileshort[i] + " has existed!");
                else if (i == 1) Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOrbClk): " + acname + " MGEX final precise clock file " +
                    sp3clkfilelong[i] + " or " + sp3clkfileshort[i] + " has existed!");
            }
        }
    }
} /* end of GetOrbClk */

/**
* @brief   : GetEop - download earth rotation parameter files
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetEop(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* compute GPS week and day of week */
    int wwww = 0;
    double sow = 0.0;
    int dow = GTime::time2gpst(ts, wwww, sow);
    std::string swwww = CString::int2str(wwww, 4);
    std::string sdow(std::to_string(dow));
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string sdoy0 = CString::int2str(doy - dow, 3);

    std::string ac(fopt->eopfrom);
    CString::trim(ac);
    CString::ToLower(ac);

    /* esu: ESA ultra-rapid EOP products
       gfu: GFZ ultra-rapid EOP products
       igu: IGS ultra-rapid EOP products
    */
    std::vector<std::string> ultraAc = { "esa_u", "gfz_u", "igs_u" };
    bool isUltra = false;
    for (size_t i = 0; i < ultraAc.size(); i++)
    {
        if (ac == ultraAc[i])
        {
            isUltra = true;
            break;
        }
    }

    /* cod: CODE final EOP products
       emr: NRCan final EOP products
       esa: ESA final EOP products
       gfz: GFZ final EOP products
       grg: CNES final EOP products
       igs: IGS final EOP products
       jpl: JPL final EOP products
    */
    std::vector<std::string> finalAcIGS = { "cod", "emr", "esa", "gfz", "grg", "igs", "jpl", "mit" };
    bool isIGS = false;
    for (size_t i = 0; i < finalAcIGS.size(); i++)
    {
        if (ac == finalAcIGS[i])
        {
            isIGS = true;
            break;
        }
    }

    if (!isUltra && !isIGS)
    {
        Logger::Trace(TERROR, "*** ERROR(FtpUtil::GetEop): Please check the setting of the 2nd item of getEop in configure file!");
        return;
    }

    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(dir.c_str());
#else           /* for Linux or Mac */
    chdir(dir.c_str());
#endif

    std::string ftpname = fopt->ftpfrom;
    CString::trim(ftpname);
    CString::ToUpper(ftpname);
    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, qr = fopt->qr;
    if (isUltra)  /* for ultra-rapid EOP products  */
    {
        std::string url, cutdirs, acname, acFile;
        if (ac == "esa_u")       /* ESA */
        {
            url = "http://navigation-office.esa.int/products/gnss-products/" + swwww;
            cutdirs = " --cut-dirs=3 ";
            acFile = "esu";
            acname = "ESA";
        }
        else if (ac == "gfz_u")  /* GFZ */
        {
            url = "ftp://ftp.gfz-potsdam.de/pub/GNSS/products/ultra/w" + swwww;
            cutdirs = " --cut-dirs=5 ";
            acFile = "gfu";
            acname = "GFZ";
        }
        else if (ac == "igs_u")  /* IGS */
        {
            if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_EOP] + "/" + swwww;
            else if (ftpname == "IGN") url = ftparchive_.IGN[IDX_EOP] + "/" + swwww;
            else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_EOP] + "/" + swwww;
            else url = ftparchive_.CDDIS[IDX_SP3] + "/" + swwww;
            cutdirs = " --cut-dirs=4 ";
            acFile = "igu";
            acname = "IGS";
        }

        for (size_t i = 0; i < fopt->hheop.size(); i++)
        {
            std::string shh = CString::int2str(fopt->hheop[i], 2);
            std::string eopfile = acFile + swwww + sdow + "_" + shh + ".erp";
            if (access(eopfile.c_str(), 0) == -1)
            {
                std::string cmd, url0, eopzfile, eopgzfile, eopxfile;
                bool isgz = false;
                if (ac == "esa_u")  /* ESA */
                {
                    eopzfile = eopfile + ".Z";
                    url0 = url + "/" + eopzfile;
                    cmd = wgetfull + " " + qr + " -nH " + cutdirs + url0;
                    std::system(cmd.c_str());

                    if (access(eopzfile.c_str(), 0) == 0)
                    {
                        /* extract '*.Z' */
                        cmd = gzipfull + " -d -f " + eopzfile;
                        std::system(cmd.c_str());
                        isgz = false;
                    }
                    else
                    {
                        eopgzfile = eopfile + ".gz";
                        url0 = url + "/" + eopgzfile;
                        cmd = wgetfull + " " + qr + " -nH " + cutdirs + url0;
                        std::system(cmd.c_str());

                        if (access(eopgzfile.c_str(), 0) == 0)
                        {
                            /* extract '*.gz' */
                            cmd = gzipfull + " -d -f " + eopgzfile;
                            std::system(cmd.c_str());
                            isgz = true;
                        }
                    }
                }
                else
                {
                    /* it is OK for '*.Z' or '*.gz' format */
                    eopxfile = eopfile + ".*";
                    cmd = wgetfull + " " + qr + " -nH -A " + eopxfile + cutdirs + url;
                    std::system(cmd.c_str());

                    eopgzfile = eopfile + ".gz";
                    eopzfile = eopfile + ".Z";
                    if (access(eopgzfile.c_str(), 0) == 0)
                    {
                        /* extract '*.gz' */
                        cmd = gzipfull + " -d -f " + eopgzfile;
                        std::system(cmd.c_str());
                        isgz = true;
                    }
                    if (access(eopfile.c_str(), 0) == -1 && access(eopzfile.c_str(), 0) == 0)
                    {
                        /* extract '*.Z' */
                        cmd = gzipfull + " -d -f " + eopzfile;
                        std::system(cmd.c_str());
                        isgz = false;
                    }

                    url0 = url + '/' + eopxfile;
                }

                char tmpfile[MAXSTRPATH] = { '\0' };
                char sep = (char)FILEPATHSEP;
                sprintf(tmpfile, "%s%c%s", dir.c_str(), sep, eopfile.c_str());
                std::string localfile = tmpfile;
                if (access(eopfile.c_str(), 0) == 0)
                {
                    Logger::Trace(TINFO, "*** INFO(FtpUtil::GetEop): successfully download " + acname +
                        " ultra-rapid EOP file " + eopfile);

                    if (isgz) eopxfile = eopgzfile;
                    else eopxfile = eopzfile;
                    std::string url0 = url + '/' + eopxfile;
                    if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetEop): " << url0 << "  ->  " <<
                        localfile << "  OK" << std::endl;
                }
                else
                {
                    Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetEop): failed to download " + acname +
                        " ultra-rapid EOP file " + eopfile);

                    if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetEop): " << url0 << "  ->  " <<
                        localfile << "  failed" << std::endl;
                }

                if (ac == "igs_u")
                {
                    /* delete some temporary directories */
                    std::string tmpdir = "repro3";
                    if (access(tmpdir.c_str(), 0) == 0)
                    {
#ifdef _WIN32  /* for Windows */
                        cmd = "rd /s /q " + tmpdir;
#else          /* for Linux or Mac */
                        cmd = "rm -rf " + tmpdir;
#endif
                        std::system(cmd.c_str());
                    }
                }
            }
            else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetEop): " + acname + " ultra-rapid EOP file " +
                eopfile + " has existed!");
        }
    }
    else if (isIGS)  /* for IGS final EOP products  */
    {
        std::string acname;
        if (ac == "cod") acname = "CODE";
        else if (ac == "emr") acname = "NRCan";
        else if (ac == "esa") acname = "ESA";
        else if (ac == "gfz") acname = "GFZ";
        else if (ac == "grg") acname = "CNES";
        else if (ac == "igs") acname = "IGS";
        else if (ac == "jpl") acname = "JPL";
        else if (ac == "mit") acname = "MIT";

        std::string eopfileshort = ac + swwww + "7.erp";
        CString::ToUpper(ac);
        std::string eopfilelong = ac + "0OPSFIN_" + syyyy + sdoy0 + "0000_07D_01D_ERP.ERP";
        int isfound = 0;  /* 1: long file name; 2: short filename */
        if (access(eopfilelong.c_str(), 0) == -1 && access(eopfileshort.c_str(), 0) == -1)
        {
            /* download the EOP file */
            std::string url, cutdirs = " --cut-dirs=4 ";
            if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_EOP] + "/" + swwww;
            else if (ftpname == "IGN") url = ftparchive_.IGN[IDX_EOP] + "/" + swwww;
            else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_EOP] + "/" + swwww;
            else url = ftparchive_.CDDIS[IDX_EOP] + "/" + swwww;

            /* it is OK for '*.Z' or '*.gz' format */
            std::string eopxfilelong = eopfilelong + ".*";
            std::string cmd = wgetfull + " " + qr + " -nH -A " + eopxfilelong + cutdirs + url;
            std::system(cmd.c_str());

            std::string eopxfile, eopfile;
            CString::GetFile(dir, eopfilelong, eopxfile);
            if (access(eopxfile.c_str(), 0) == 0)
            {
                eopfile = eopfilelong;
                isfound = 1;
            }
            else
            {
                /* it is OK for '*.Z' or '*.gz' format */
                std::string eopxfileshort = eopfileshort + ".*";
                cmd = wgetfull + " " + qr + " -nH -A " + eopxfileshort + cutdirs + url;
                std::system(cmd.c_str());

                CString::GetFile(dir, eopfileshort, eopxfile);
                if (access(eopxfile.c_str(), 0) == 0)
                {
                    eopfile = eopfileshort;
                    isfound = 2;
                }
            }

            char tmpfile[MAXSTRPATH] = { '\0' };
            char sep = (char)FILEPATHSEP;
            if (isfound == 0)
            {
                Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetEop): failed to download " + acname +
                    " final EOP file " + eopfile);

                sprintf(tmpfile, "%s%c%s", dir.c_str(), sep, eopfile.c_str());
                std::string localfile = tmpfile;
                std::string url0 = url + '/' + eopxfilelong;
                if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetEop): " << url0 << "  ->  " <<
                    localfile << "  failed" << std::endl;
            }
            else
            {
                /* extract '*.gz' or '*.Z' */
                cmd = gzipfull + " -d -f " + eopxfile;
                std::system(cmd.c_str());

                sprintf(tmpfile, "%s%c%s", dir.c_str(), sep, eopfile.c_str());
                std::string localfile = tmpfile;
                if (access(eopfile.c_str(), 0) == 0)
                {
                    Logger::Trace(TINFO, "*** INFO(FtpUtil::GetEop): successfully download " + acname +
                        " final EOP file " + eopfile);

                    std::string url0 = url + '/' + eopxfile;
                    if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetEop): " << url0 << "  ->  " <<
                        localfile << "  OK" << std::endl;

                    /* to check if it needs to convert precise orbit and clock files with long name to those with short name */
                    if (fopt->l2s4eop > 0)
                    {
                        std::string change_filename;
#ifdef _WIN32  /* for Windows */
                        if (fopt->l2s4eop == 1) change_filename = "rename";
                        else if (fopt->l2s4eop == 2) change_filename = "copy";
#else          /* for Linux or Mac */
                        if (fopt->l2s4eop == 1) change_filename = "mv";
                        else if (fopt->l2s4eop == 2) change_filename = "cp";
#endif
                        cmd = change_filename + " " + eopfilelong + " " + eopfileshort;
                        std::system(cmd.c_str());
                    }
                }

                /* delete some temporary directories */
                std::string tmpdir = "repro3";
                if (access(tmpdir.c_str(), 0) == 0)
                {
#ifdef _WIN32  /* for Windows */
                    cmd = "rd /s /q " + tmpdir;
#else          /* for Linux or Mac */
                    cmd = "rm -rf " + tmpdir;
#endif
                    std::system(cmd.c_str());
                }
            }
        }
        else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetEop): " + acname + " final EOP file " + eopfilelong +
            " or " + eopfileshort + " has existed!");
    }
} /* end of GetEop */

/**
* @brief   : GetObx - to download ORBEX (ORBit EXchange format) files
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetObx(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(dir.c_str());
#else           /* for Linux or Mac */
    chdir(dir.c_str());
#endif

    /* compute GPS week and day of week */
    int wwww = 0;
    double sow = 0.0;
    int dow = GTime::time2gpst(ts, wwww, sow);
    std::string swwww = CString::int2str(wwww, 4);
    std::string sdow(std::to_string(dow));
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string sdoy = CString::int2str(doy, 3);

    std::string ac(fopt->obxfrom);
    CString::trim(ac);
    CString::ToLower(ac);

    /* cod: CODE multi-GNSS final OBX
       gfz: GFZ multi-GNSS final OBX
       grg: CNES multi-GNSS final OBX
       whu: WHU multi-GNSS final OBX
    */
    std::vector<std::string> acMGEX = {"cod", "gfz", "grg", "whu"};
    std::vector<std::string> acs;
    if (ac == "all") acs = acMGEX;
    else acs.push_back(ac);

    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string ftpname = fopt->ftpfrom;
    CString::trim(ftpname);
    CString::ToUpper(ftpname);
    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, qr = fopt->qr;
    std::string ac_i;
    for (size_t i = 0; i < acs.size(); i++)
    {
        ac_i = acs[i];
        if (ac_i == "cnt")  /* for CNES real-time ORBEX files */
        {
            std::string obxfile = ac + swwww + sdow + ".obx";
            if (access(obxfile.c_str(), 0) == -1)
            {
                std::string obxgzFile = obxfile + ".gz";
                std::string url = "http://www.ppp-wizard.net/products/REAL_TIME/" + obxgzFile;
                std::string cmd = wgetfull + " " + qr + " -nH --cut-dirs=2 " + url;
                std::system(cmd.c_str());

                /* extract '*.gz' */
                cmd = gzipfull + " -d -f " + obxgzFile;
                std::system(cmd.c_str());

                std::string localfile = dir + sep + obxfile;
                if (access(obxfile.c_str(), 0) == 0)
                {
                    Logger::Trace(TINFO, "*** INFO(FtpUtil::GetObx): successfully download CNES real-time ORBEX file " + obxfile);

                    if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetObx): " << url << "  ->  " <<
                        localfile << "  OK" << std::endl;
                }
                else
                {
                    Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetObx): failed to download CNES real-time ORBEX file " + obxfile);

                    if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetObx): " << url << "  ->  " <<
                        localfile << "  failed" << std::endl;

                }

                /* delete some temporary directories */
                std::vector<std::string> tmpdir = { "FORMAT_BIAIS_OFFI1", "FORMATBIAS_OFF_v1" };
                for (size_t i = 0; i < tmpdir.size(); i++)
                {
                    if (access(tmpdir[i].c_str(), 0) == 0)
                    {
    #ifdef _WIN32  /* for Windows */
                        cmd = "rd /s /q " + tmpdir[i];
    #else          /* for Linux or Mac */
                        cmd = "rm -rf " + tmpdir[i];
    #endif
                        std::system(cmd.c_str());
                    }
                }
            }
            else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetObx): CNES real-time ORBEX file " + obxfile + " has existed!");
        }
        else  /* for MGEX final/rapid ORBEX files  */
        {
            std::string obxfile, substr, acname;
            if (ac_i == "cod")
            {
                obxfile = "COD0MGXFIN_" + syyyy + sdoy + "0000_01D_" + "*_ATT.OBX";
                substr = "COD0MGXFIN_" + syyyy + sdoy + "0000_01D_";
                acname = "CODE";
            }
            else if (ac_i == "gfz")
            {
                obxfile = "GFZ0MGXRAP_" + syyyy + sdoy + "0000_01D_" + "*_ATT.OBX";
                substr = "GFZ0MGXRAP_" + syyyy + sdoy + "0000_01D_";
                acname = "GFZ";
            }
            else if (ac_i == "grg")
            {
                obxfile = "GRG0MGXFIN_" + syyyy + sdoy + "0000_01D_" + "*_ATT.OBX";
                substr = "GRG0MGXFIN_" + syyyy + sdoy + "0000_01D_";
                acname = "CNES";
            }
            else if (ac_i == "whu")
            {
                obxfile = "WUM0MGXFIN_" + syyyy + sdoy + "0000_01D_" + "*_ATT.OBX";
                substr = "WUM0MGXFIN_" + syyyy + sdoy + "0000_01D_";
                acname = "WHU";
            }

            std::string obx0file;
            CString::GetFile(dir, substr, obx0file);
            if (access(obx0file.c_str(), 0) == -1)
            {
                std::string cmd, url, cutdirs = " --cut-dirs=5 ";
                if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OBXM] + "/" + swwww;
                else if (ftpname == "IGN") url = ftparchive_.IGN[IDX_OBXM] + "/" + swwww;
                else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_OBXM] + "/" + swwww;
                else url = ftparchive_.CDDIS[IDX_OBXM] + "/" + swwww;

                /* it is OK for '*.Z' or '*.gz' format */
                std::string obxxfile = obxfile + ".*";
                cmd = wgetfull + " " + qr + " -nH -A " + obxxfile + cutdirs + url;
                std::system(cmd.c_str());

                CString::GetFile(dir, substr, obxxfile);
                if (access(obxxfile.c_str(), 0) == -1)
                {
                    cutdirs = " --cut-dirs=4 ";
                    if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_SP3] + "/" + swwww;
                    else if (ftpname == "IGN") url = ftparchive_.IGN[IDX_SP3] + "/" + swwww;
                    else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_SP3] + "/" + swwww;
                    else url = ftparchive_.CDDIS[IDX_SP3] + "/" + swwww;

                    obxxfile = obxfile + ".*";
                    cmd = wgetfull + " " + qr + " -nH -A " + obxxfile + cutdirs + url;
                    std::system(cmd.c_str());

                    CString::GetFile(dir, substr, obxxfile);
                    if (access(obxxfile.c_str(), 0) == -1)
                    {
                        cutdirs = " --cut-dirs=5 ";
                        std::string urlx = "ftp://igsdepot.ign.fr/pub/igs/products/mgex/" + swwww;
                        /* it is OK for '*.Z' or '*.gz' format */
                        obxxfile = obxfile + ".*";
                        cmd = wgetfull + " " + qr + " -nH -A " + obxxfile + cutdirs + urlx;
                        std::system(cmd.c_str());
                    }
                }

                std::string localfile;
                CString::GetFile(dir, substr, obxxfile);
                if (access(obxxfile.c_str(), 0) == -1)
                {
                    Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetObx): failed to download " +
                        acname + " MGEX ORBEX file " + obxfile);

                    localfile = dir + sep + obxfile;
                    std::string url0 = url + '/' + obxfile;
                    if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetObx): " << url0 << "  ->  " <<
                        localfile << "  failed" << std::endl;
                }
                else
                {
                    cmd = gzipfull + " -d -f " + obxxfile;
                    std::system(cmd.c_str());

                    CString::GetFile(dir, substr, obx0file);
                    if (access(obx0file.c_str(), 0) == 0)
                    {
                        Logger::Trace(TINFO, "*** INFO(FtpUtil::GetObx): successfully download " +
                            acname + " MGEX ORBEX file " + obx0file);

                        localfile = dir + sep + obx0file;
                        std::string url0 = url + '/' + obxxfile;
                        if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetObx): " << url0 << "  ->  " <<
                            localfile << "  OK" << std::endl;
                    }
                }
            }
            else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetObx): " + acname + " MGEX ORBEX file " +
                obxfile + " has existed!");
        }
    }
} /* end of GetObx */

/**
* @brief   : GetDsb - to download CODE DCB or MGEX daily DSB (i.e., from CAS) file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetDsb(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(dir.c_str());
#else           /* for Linux or Mac */
    chdir(dir.c_str());
#endif

    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string sdoy = CString::int2str(doy, 3);

    std::string ftpname = fopt->ftpfrom;
    CString::trim(ftpname);
    CString::ToUpper(ftpname);
    std::string ac(fopt->dsbfrom);
    CString::trim(ac);
    CString::ToLower(ac);
    std::vector<std::string> acs;
    if (ac == "all")
    {
        acs.push_back("cod");
        acs.push_back("cas");
    }
    else acs.push_back(ac);
    std::string ac_i;
    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, qr = fopt->qr;
    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    for (size_t i = 0; i < acs.size(); i++)
    {
        ac_i = acs[i];
        if (ac_i == "cas")  /* MGEX daily DSB (i.e., from CAS) */
        {
            std::string dsbfile = "CAS0MGXRAP_" + syyyy + sdoy + "0000_01D_01D_DCB.BSX";
            if (access(dsbfile.c_str(), 0) == -1)
            {
                std::string url, cutdirs = " --cut-dirs=5 ";
                if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_DSBM] + "/" + syyyy;
                else if (ftpname == "IGN")
                {
                    url = ftparchive_.IGN[IDX_DSBM] + "/" + syyyy;
                    cutdirs = " --cut-dirs=6 ";
                }
                else if (ftpname == "WHU")
                {
                    url = ftparchive_.WHU[IDX_DSBM] + "/" + syyyy;
                    cutdirs = " --cut-dirs=6 ";
                }
                else url = ftparchive_.CDDIS[IDX_DSBM] + "/" + syyyy;

                /* it is OK for '*.Z' or '*.gz' format */
                std::string dsbxfile = dsbfile + ".*";
                std::string cmd = wgetfull + " " + qr + " -nH -A " + dsbxfile + cutdirs + url;
                std::system(cmd.c_str());

                std::string localfile = dir + sep + dsbfile;
                CString::GetFile(dir, dsbfile, dsbxfile);
                if (access(dsbxfile.c_str(), 0) == -1)
                {
                    Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetDsb): failed to download multi-GNSS DSB file " + dsbfile);

                    std::string url0 = url + '/' + dsbxfile;
                    if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetDsb): " << url0 << "  ->  " <<
                        localfile << "  failed" << std::endl;
                }
                else
                {
                    /* extract '*.gz' or '*.Z' */
                    cmd = gzipfull + " -d -f " + dsbxfile;
                    std::system(cmd.c_str());

                    Logger::Trace(TINFO, "*** INFO(FtpUtil::GetDsb): successfully download multi-GNSS DSB file " + dsbfile);

                    std::string url0 = url + '/' + dsbxfile;
                    if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetDsb): " << url0 << "  ->  " <<
                        localfile << "  OK" << std::endl;
                }
            }
            else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetDsb): multi-GNSS DSB file " + dsbfile + " has existed!");
        }
        else if (ac_i == "cod")  /* CODE DCB */
        {
            int yy = GTime::yyyy2yy(yyyy);
            std::string syy = CString::int2str(yy, 2);
            std::vector<double> ep;
            ep = GTime::time2ymdhms(ts);
            int mm = (int)ep[1];
            std::string smm = CString::int2str(mm, 2);

            std::string dcbfile, dcb0file;
            std::vector<std::string> dcbtype = { "P1P2", "P1C1", "P2C2" };
            for (size_t i = 0; i < dcbtype.size(); i++)
            {
                dcbfile = dcbtype[i] + syy + smm + ".DCB";
                dcb0file = dcbtype[i] + syy + smm + ".DCB";
                if (dcbtype[i] == "P2C2") dcbfile = dcbtype[i] + syy + smm + "_RINEX.DCB";
                if (access(dcbfile.c_str(), 0) == -1 && access(dcb0file.c_str(), 0) == -1)
                {
                    /* download CODE DCB file */
                    std::string url = "ftp://ftp.aiub.unibe.ch/CODE/" + syyyy;
                    /* it is OK for '*.Z' or '*.gz' format */
                    std::string dcbxfile = dcbfile + ".*";
                    std::string cmd = wgetfull + " " + qr + " -nH -A " + dcbxfile + " --cut-dirs=2 " + url;
                    std::system(cmd.c_str());

                    std::string localfile = dir + sep + dcb0file;
                    CString::GetFile(dir, dcbfile, dcbxfile);
                    if (access(dcbxfile.c_str(), 0) == -1)
                    {
                        Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetDsb): failed to download CODE DCB file " + dcbfile);

                        std::string url0 = url + '/' + dcbxfile;
                        if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetDsb): " << url0 << "  ->  " <<
                            localfile << "  failed" << std::endl;
                    }
                    else
                    {
                        /* extract '*.gz' or '*.Z' */
                        cmd = gzipfull + " -d -f " + dcbxfile;
                        std::system(cmd.c_str());

                        if (dcbtype[i] == "P2C2")
                        {
                            std::string change_filename;
#ifdef _WIN32  /* for Windows */
                            change_filename = "rename";
#else          /* for Linux or Mac */
                            change_filename = "mv";
#endif
                            cmd = change_filename + " " + dcbfile + " " + dcb0file;
                            std::system(cmd.c_str());
                        }

                        if (access(dcb0file.c_str(), 0) == 0)
                        {
                            Logger::Trace(TINFO, "*** INFO(FtpUtil::GetDsb): successfully download CODE DCB file " + dcb0file);

                            std::string url0 = url + '/' + dcbxfile;
                            if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetDsb): " << url0 << "  ->  " <<
                                localfile << "  OK" << std::endl;
                        }
                    }
                }
                else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetDsb): CODE DCB file " + dcb0file + " or " +
                    dcbfile + " has existed!");
            }
        }
    }
} /* end of GetDsb */

/**
* @brief   : GetOsb - to download CNES real-time or MGEX final OSB files
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetOsb(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(dir.c_str());
#else           /* for Linux or Mac */
    chdir(dir.c_str());
#endif

    /* compute GPS week and day of week */
    int wwww = 0;
    double sow = 0.0;
    int dow = GTime::time2gpst(ts, wwww, sow);
    std::string swwww = CString::int2str(wwww, 4);
    std::string sdow(std::to_string(dow));
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string sdoy = CString::int2str(doy, 3);

    std::string ac(fopt->osbfrom);
    CString::trim(ac);
    CString::ToLower(ac);

    /* cod: CODE multi-GNSS final OSB
       gfz: GFZ multi-GNSS final OSB
       grg: CNES multi-GNSS final OSB
       whu: WHU multi-GNSS final OSB
    */
    std::vector<std::string> acMGEX = {"cas", "cod", "gfz", "grg", "whu"};
    std::vector<std::string> acs;
    if (ac == "all") acs = acMGEX;
    else acs.push_back(ac);

    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string ftpname = fopt->ftpfrom;
    CString::trim(ftpname);
    CString::ToUpper(ftpname);
    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, qr = fopt->qr;
    std::string ac_i;
    for (size_t i = 0; i < acs.size(); i++)
    {
        ac_i = acs[i];
        if (ac_i == "cnt")  /* CNES real-time OSB from CNES offline files */
        {
            std::string osbfile = ac + swwww + sdow + ".bia";
            if (access(osbfile.c_str(), 0) == -1)
            {
                std::string osbgzfile = osbfile + ".gz";
                std::string url = "http://www.ppp-wizard.net/products/REAL_TIME/" + osbgzfile;
                std::string cmd = wgetfull + " " + qr + " -nH --cut-dirs=2 " + url;
                std::system(cmd.c_str());

                /* extract '*.gz' */
                cmd = gzipfull + " -d -f " + osbgzfile;
                std::system(cmd.c_str());

                std::string localfile = dir + sep + osbfile;
                if (access(osbfile.c_str(), 0) == 0)
                {
                    Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOsb): successfully download CNES real-time OSB file " + osbfile);

                    if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetOsb): " << url << "  ->  " <<
                        localfile << "  OK" << std::endl;
                }
                else
                {
                    Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetOsb): failed to download CNES real-time OSB file " + osbfile);

                    if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetOsb): " << url << "  ->  " <<
                        localfile << "  failed" << std::endl;
                }

                /* delete some temporary directories */
                std::vector<std::string> tmpdir = { "FORMAT_BIAIS_OFFI1", "FORMATBIAS_OFF_v1" };
                for (size_t i = 0; i < tmpdir.size(); i++)
                {
                    if (access(tmpdir[i].c_str(), 0) == 0)
                    {
    #ifdef _WIN32  /* for Windows */
                        cmd = "rd /s /q " + tmpdir[i];
    #else          /* for Linux or Mac */
                        cmd = "rm -rf " + tmpdir[i];
    #endif
                        std::system(cmd.c_str());
                    }
                }
            }
            else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOsb): CNES real-time OSB file " + osbfile + " has existed!");
        }
        else  /* for MGEX final/rapid OSB files */
        {
            std::string ac = ac_i;
            CString::ToUpper(ac);
            std::string osbfile = ac + "0MGXFIN_" + syyyy + sdoy + "0000_01D_01D_OSB.BIA";
            if (ac_i == "cas" || ac_i == "gfz") osbfile = ac + "0MGXRAP_" + syyyy + sdoy + "0000_01D_01D_OSB.BIA";
            if (access(osbfile.c_str(), 0) == -1)
            {
                /* it is OK for '*.Z' or '*.gz' format */
                std::string osbxfile = osbfile + ".*";
                std::string cmd, url;
                if (ac_i == "cas")
                {
                    std::string url0 = "ftp://ftp.gipp.org.cn/product/dcb/mgex/" + syyyy;
                    std::string cutdirs0 = " --cut-dirs=4 ";
                    cmd = wgetfull + " " + qr + " -nH -A " + osbxfile + cutdirs0 + url0;
                    std::system(cmd.c_str());
                }
                else
                {
                    std::string cutdirs = " --cut-dirs=5 ";
                    if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_OSBM] + "/" + swwww;
                    else if (ftpname == "IGN") url = ftparchive_.IGN[IDX_OSBM] + "/" + swwww;
                    else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_OSBM] + "/" + swwww;
                    else url = ftparchive_.CDDIS[IDX_OSBM] + "/" + swwww;

                    cmd = wgetfull + " " + qr + " -nH -A " + osbxfile + cutdirs + url;
                    std::system(cmd.c_str());

                    CString::GetFile(dir, osbfile, osbxfile);
                    if (access(osbxfile.c_str(), 0) == -1)
                    {
                        cutdirs = " --cut-dirs=4 ";
                        if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_SP3] + "/" + swwww;
                        else if (ftpname == "IGN") url = ftparchive_.IGN[IDX_SP3] + "/" + swwww;
                        else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_SP3] + "/" + swwww;
                        else url = ftparchive_.CDDIS[IDX_SP3] + "/" + swwww;

                        osbxfile = osbfile + ".*";
                        cmd = wgetfull + " " + qr + " -nH -A " + osbxfile + cutdirs + url;
                        std::system(cmd.c_str());

                        CString::GetFile(dir, osbfile, osbxfile);
                        if (access(osbxfile.c_str(), 0) == -1)
                        {
                            cutdirs = " --cut-dirs=5 ";
                            std::string urlx = "ftp://igsdepot.ign.fr/pub/igs/products/mgex/" + swwww;
                            /* it is OK for '*.Z' or '*.gz' format */
                            osbxfile = osbfile + ".*";
                            cmd = wgetfull + " " + qr + " -nH -A " + osbxfile + cutdirs + urlx;
                            std::system(cmd.c_str());
                        }
                    }
                }

                std::string localfile;
                CString::GetFile(dir, osbfile, osbxfile);
                if (access(osbxfile.c_str(), 0) == -1)
                {
                    Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetOsb): failed to download " +
                        ac + " MGEX OSB file " + osbfile);

                    localfile = dir + sep + osbfile;
                    std::string url0 = url + '/' + osbfile;
                    if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetOsb): " << url0 << "  ->  " <<
                        localfile << "  failed" << std::endl;
                }
                else
                {
                    cmd = gzipfull + " -d -f " + osbxfile;
                    std::system(cmd.c_str());
                    if (access(osbfile.c_str(), 0) == 0)
                    {
                        Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOsb): successfully download " + ac +
                            " MGEX OSB file " + osbfile);

                        localfile = dir + sep + osbfile;
                        std::string url0 = url + '/' + osbxfile;
                        if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetOsb): " << url0 << "  ->  " <<
                            localfile << "  OK" << std::endl;
                    }
                }
            }
            else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetOsb): " + ac + " MGEX OSB file " + osbfile + " has existed!");
        }

    }
} /* end of GetOsb */

/**
* @brief   : GetSnx - to download IGS weekly SINEX file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetSnx(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(dir.c_str());
#else           /* for Linux or Mac */
    chdir(dir.c_str());
#endif

    /* compute GPS week and day of week */
    int wwww = 0;
    double sow = 0.0;
    int dow = GTime::time2gpst(ts, wwww, sow);
    std::string swwww = CString::int2str(wwww, 4);
    std::string sdow(std::to_string(dow));
    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string sdoy = CString::int2str(doy, 3);
    std::string sdoy0 = CString::int2str(doy - dow, 3);

    std::string ftpname = fopt->ftpfrom;
    CString::trim(ftpname);
    CString::ToUpper(ftpname);
    std::string snxfileshort = "igs" + swwww + ".snx";
    std::string snx0filelong = "IGS0OPSSNX_" + syyyy + sdoy0 + "0000_07D_07D_CRD.SNX";
    std::string snx1filelong = "IGS0OPSSNX_" + syyyy + sdoy0 + "0000_07D_07D_SOL.SNX";
    int isfound = 0;  /* 1: long file name; 2: short filename */
    if (access(snxfileshort.c_str(), 0) == -1 && access(snx0filelong.c_str(), 0) == -1)
    {
        std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, qr = fopt->qr;
        std::string url, cutdirs = " --cut-dirs=4 ";
        if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_SNX] + "/" + swwww;
        else if (ftpname == "IGN") url = ftparchive_.IGN[IDX_SNX] + "/" + swwww;
        else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_SNX] + "/" + swwww;
        else url = ftparchive_.CDDIS[IDX_SNX] + "/" + swwww;

        /* it is OK for '*.Z' or '*.gz' format */
        std::string snxxfile = snx0filelong + ".*";
        std::string cmd = wgetfull + " " + qr + " -nH -A " + snxxfile + cutdirs + url;
        std::system(cmd.c_str());

        std::string snxfile;
        CString::GetFile(dir, snx0filelong, snxxfile);
        if (access(snxxfile.c_str(), 0) == 0)
        {
            snxfile = snx0filelong;
            isfound = 1;
        }
        else
        {
            snxfile = "igs*P" + swwww + ".snx";
            snxxfile = snxfile + ".*";
            cmd = wgetfull + " " + qr + " -nH -A " + snxxfile + cutdirs + url;
            std::system(cmd.c_str());

            std::string substr = "P" + swwww + ".snx";
            CString::GetFile(dir, substr, snxxfile);
            if (access(snxxfile.c_str(), 0) == 0)
            {
                snxfile = snxfileshort;
                isfound = 2;
            }
        }
        if (isfound == 0)
        {
            /* If the weekly SINEX file does not exist,  we use the daily SINEX file instead */
            snxfileshort = "igs" + swwww + sdow + ".snx";
            snx0filelong = "IGS0OPSSNX_" + syyyy + sdoy + "0000_01D_01D_CRD.SNX";
            snx1filelong = "IGS0OPSSNX_" + syyyy + sdoy + "0000_01D_01D_SOL.SNX";

            /* it is OK for '*.Z' or '*.gz' format */
            snxxfile = snx0filelong + ".*";
            cmd = wgetfull + " " + qr + " -nH -A " + snxxfile + cutdirs + url;
            std::system(cmd.c_str());

            CString::GetFile(dir, snx0filelong, snxxfile);
            if (access(snxxfile.c_str(), 0) == 0)
            {
                snxfile = snx0filelong;
                isfound = 1;
            }
            else
            {
                snxfile = "igs*P" + swwww + sdow + ".snx";
                snxxfile = snxfile + ".*";
                cmd = wgetfull + " " + qr + " -nH -A " + snxxfile + cutdirs + url;
                std::system(cmd.c_str());

                std::string substr = "P" + swwww + ".snx";
                CString::GetFile(dir, substr, snxxfile);
                if (access(snxxfile.c_str(), 0) == 0)
                {
                    snxfile = snxfileshort;
                    isfound = 2;
                }
            }
        }
        char tmpfile[MAXSTRPATH] = { '\0' };
        char sep = (char)FILEPATHSEP;
        std::string localfile, url0;
        sprintf(tmpfile, "%s%c%s", dir.c_str(), sep, snxfile.c_str());
        localfile = tmpfile;
        if (isfound == 0)
        {
            Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetSnx): failed to download IGS weekly SINEX file " + snxfile);

            url0 = url + '/' + snxxfile;
            if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetSnx): " << url0 << "  ->  " <<
                localfile << "  failed" << std::endl;
        }
        else if (isfound > 0)
        {
            size_t pos = snxxfile.find_last_of(".");
            if (pos != std::string::npos) snxfile = snxxfile.substr(0, pos);
            /* extract '*.gz' or '*.Z' */
            cmd = gzipfull + " -d -f " + snxxfile;
            std::system(cmd.c_str());
            if (access(snxfile.c_str(), 0) == 0)
            {
                Logger::Trace(TINFO, "*** INFO(FtpUtil::GetSnx): successfully download IGS weekly SINEX file " + snxfile);

                url0 = url + '/' + snxxfile;
                if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetSnx): " << url0 << "  ->  " <<
                    localfile << "  OK" << std::endl;

                if (isfound == 2)
                {
                    std::string change_filename;
#ifdef _WIN32  /* for Windows */
                    change_filename = "rename";
#else          /* for Linux or Mac */
                    change_filename = "mv";
#endif
                    cmd = change_filename + " " + snxfile + " " + snxfileshort;
                    std::system(cmd.c_str());
                }

                /* to check if it needs to convert SINEX file with long name to those with short name */
                if (fopt->l2s4eop > 0 && isfound == 1)
                {
                    std::string change_filename;
#ifdef _WIN32  /* for Windows */
                    if (fopt->l2s4oc == 1) change_filename = "rename";
                    else if (fopt->l2s4oc == 2) change_filename = "copy";
#else          /* for Linux or Mac */
                    if (fopt->l2s4oc == 1) change_filename = "mv";
                    else if (fopt->l2s4oc == 2) change_filename = "cp";
#endif
                    cmd = change_filename + " " + snx0filelong + " " + snxfileshort;
                    std::system(cmd.c_str());
                }
            }
        }

        /* delete some temporary directories */
        std::string tmpdir = "repro3";
        if (access(tmpdir.c_str(), 0) == 0)
        {
#ifdef _WIN32  /* for Windows */
            cmd = "rd /s /q " + tmpdir;
#else          /* for Linux or Mac */
            cmd = "rm -rf " + tmpdir;
#endif
            std::system(cmd.c_str());
        }
    }
    else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetSnx): IGS weekly SINEX file " + snxfileshort + " or " +
        snx0filelong + " has existed!");
} /* end of GetSnx */

/**
* @brief   : GetIono - to download daily GIM file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetIono(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(dir.c_str());
#else           /* for Linux or Mac */
    chdir(dir.c_str());
#endif

    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);

    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    std::string ftpname = fopt->ftpfrom;
    CString::trim(ftpname);
    CString::ToUpper(ftpname);
    std::string ac(fopt->ionfrom);
    CString::trim(ac);
    CString::ToLower(ac);
    std::vector<std::string> acfinal = { "cas", "cod", "emr", "esa", "igs", "jpl", "upc" };
    std::vector<std::string> acs;
    if (ac == "all") acs = acfinal;
    else if ((int)ac.find_first_of('+') > 0) acs = CString::split(ac, "+");
    else acs.push_back(ac);
    for (size_t i = 0; i < acs.size(); i++)
    {
        std::string ionfileshort = acs[i] + "g" + sdoy + "0." + syy + "i";
        CString::ToUpper(acs[i]);
        std::string ionfilelongkey = acs[i] + "0OPSFIN_" + syyyy + sdoy + "0000_01D_";
        std::string ionfilelong, ionfile;
        CString::GetFile(dir, ionfilelongkey, ionfilelong);
        if (access(ionfileshort.c_str(), 0) == -1 && access(ionfilelong.c_str(), 0) == -1)
        {
            std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, qr = fopt->qr;
            std::string url, cutdirs = " --cut-dirs=6 ";
            if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_ION] + "/" +
                syyyy + "/" + sdoy;
            else if (ftpname == "IGN") url = ftparchive_.IGN[IDX_ION] + "/" +
                syyyy + "/" + sdoy;
            else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_ION] + "/" +
                syyyy + "/" + sdoy;
            else url = ftparchive_.CDDIS[IDX_ION] + "/" + syyyy + "/" + sdoy;

            /* it is OK for '*.Z' or '*.gz' format */
            std::string ionxfile = ionfilelongkey + "*";
            std::string cmd = wgetfull + " " + qr + " -nH -A " + ionxfile + cutdirs + url;
            std::system(cmd.c_str());

            int isfound = 0;  /* 1: long file name; 2: short filename */
            CString::GetFile(dir, ionfilelongkey, ionxfile);
            if (access(ionxfile.c_str(), 0) == 0)
            {
                ionfile = ionxfile.substr(0, (int)ionxfile.find_last_of('.'));
                isfound = 1;
            }
            else
            {
                /* it is OK for '*.Z' or '*.gz' format */
                ionxfile = ionfileshort + ".*";
                cmd = wgetfull + " " + qr + " -nH -A " + ionxfile + cutdirs + url;
                std::system(cmd.c_str());

                CString::GetFile(dir, ionfileshort, ionxfile);
                if (access(ionxfile.c_str(), 0) == 0)
                {
                    ionfile = ionfileshort;
                    isfound = 2;
                }
            }

            std::string localfile = dir + sep + ionfile, url0;
            if (isfound == 0)
            {
                Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetIono): failed to download ionopospheric product file " + ionfile);

                url0 = url + '/' + ionxfile;
                if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetIono): " << url0 << "  ->  " <<
                    localfile << "  failed" << std::endl;
            }
            else
            {
                /* extract '*.gz' or '*.Z' */
                cmd = gzipfull + " -d -f " + ionxfile;
                std::system(cmd.c_str());
                if (access(ionfile.c_str(), 0) == 0)
                {
                    Logger::Trace(TINFO, "*** INFO(FtpUtil::GetIono): successfully download ionopospheric product file " + ionfile);

                    url0 = url + '/' + ionxfile;
                    if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetIono): " << url0 << "  ->  " <<
                        localfile << "  OK" << std::endl;

                    /* to check if it needs to convert ionoposphere files with long name to those with short name */
                    if (isfound == 1 && fopt->l2s4ion > 0)
                    {
                        std::string change_filename;
#ifdef _WIN32  /* for Windows */
                        if (fopt->l2s4ion == 1) change_filename = "rename";
                        else if (fopt->l2s4ion == 2) change_filename = "copy";
#else          /* for Linux or Mac */
                        if (fopt->l2s4ion == 1) change_filename = "mv";
                        else if (fopt->l2s4ion == 2) change_filename = "cp";
#endif
                        cmd = change_filename + " " + ionfile + " " + ionfileshort;
                        std::system(cmd.c_str());
                    }
                }
            }

            /* delete some temporary directories */
            std::string tmpdir = "topex";
            if (access(tmpdir.c_str(), 0) == 0)
            {
#ifdef _WIN32  /* for Windows */
                cmd = "rd /s /q " + tmpdir;
#else          /* for Linux or Mac */
                cmd = "rm -rf " + tmpdir;
#endif
                std::system(cmd.c_str());
            }
        }
        else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetIono): ionopospheric product file " +
            ionfileshort + " or " + ionfilelong + " has existed!");
    }
} /* end of GetIono */

/**
* @brief   : GetRoti - to download daily Rate of TEC index (ROTI) file
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetRoti(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(dir.c_str());
#else           /* for Linux or Mac */
    chdir(dir.c_str());
#endif

    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);

    std::string ftpname = fopt->ftpfrom;
    CString::trim(ftpname);
    CString::ToUpper(ftpname);
    std::string rotfile = "roti" + sdoy + "0." + syy + "f";
    if (access(rotfile.c_str(), 0) == -1)
    {
        std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, qr = fopt->qr;
        std::string url, cutdirs = " --cut-dirs=6 ";
        if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_ROTI] + "/" +
            syyyy + "/" + sdoy;
        else if (ftpname == "IGN") url = ftparchive_.IGN[IDX_ROTI] + "/" +
            syyyy + "/" + sdoy;
        else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_ROTI] + "/" +
            syyyy + "/" + sdoy;
        else url = ftparchive_.CDDIS[IDX_ROTI] + "/" + syyyy + "/" + sdoy;

        /* it is OK for '*.Z' or '*.gz' format */
        std::string rotxfile = rotfile + ".*";
        std::string cmd = wgetfull + " " + qr + " -nH -A " + rotxfile + cutdirs + url;
        std::system(cmd.c_str());

        std::string sep;
        sep.push_back((char)FILEPATHSEP);
        std::string localfile = dir + sep + rotfile, url0;
        CString::GetFile(dir, rotfile, rotxfile);
        if (access(rotxfile.c_str(), 0) == -1)
        {
            Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetRoti): failed to download ROTI file " + rotfile);

            url0 = url + '/' + rotxfile;
            if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetRoti): " << url0 << "  ->  " <<
                localfile << "  failed" << std::endl;
        }
        else
        {
            /* extract '*.gz' or '*.Z' */
            cmd = gzipfull + " -d -f " + rotxfile;
            std::system(cmd.c_str());

            if (access(rotfile.c_str(), 0) == 0)
            {
                Logger::Trace(TINFO, "*** INFO(FtpUtil::GetRoti): successfully download ROTI file " + rotfile);

                url0 = url + '/' + rotxfile;
                if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetRoti): " << url0 << "  ->  " <<
                    localfile << "  OK" << std::endl;
            }
        }

        /* delete some temporary directories */
        std::string tmpdir = "topex";
        if (access(tmpdir.c_str(), 0) == 0)
        {
#ifdef _WIN32  /* for Windows */
            cmd = "rd /s /q " + tmpdir;
#else          /* for Linux or Mac */
            cmd = "rm -rf " + tmpdir;
#endif
            std::system(cmd.c_str());
        }
    }
    else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetRoti): ROTI file " + rotfile + " has existed!");
} /* end of GetRoti */

/**
* @brief   : GetTrop - to download daily tropospheric product file from CDDIS and/or CODE
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetTrop(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(dir.c_str());
#else           /* for Linux or Mac */
    chdir(dir.c_str());
#endif

    /* compute day of year */
    int yyyy, doy;
    GTime::time2yrdoy(ts, yyyy, doy);
    int yy = GTime::yyyy2yy(yyyy);
    std::string syyyy = CString::int2str(yyyy, 4);
    std::string syy = CString::int2str(yy, 2);
    std::string sdoy = CString::int2str(doy, 3);
    int wwww = 0;
    double sow = 0.0;
    int dow = GTime::time2gpst(ts, wwww, sow);
    std::string swwww = CString::int2str(wwww, 4);
    std::string sdow(std::to_string(dow));

    std::string ac(fopt->trpfrom);
    CString::trim(ac);
    CString::ToLower(ac);
    std::string wgetfull = fopt->wgetfull, gzipfull = fopt->gzipfull, qr = fopt->qr;
    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    if (ac == "igs")
    {
        /* creation of sub-directory ('daily') */
        std::string subdir = dir + sep + "IGS";
        if (access(subdir.c_str(), 0) == -1)
        {
            /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
            std::string cmd = "mkdir " + subdir;
#else           /* for Linux or Mac */
            std::string cmd = "mkdir -p " + subdir;
#endif
            std::system(cmd.c_str());
        }

        /* change directory */
#ifdef _WIN32   /* for Windows */
        _chdir(subdir.c_str());
#else           /* for Linux or Mac */
        chdir(subdir.c_str());
#endif

        std::string ftpname = fopt->ftpfrom;
        CString::trim(ftpname);
        CString::ToUpper(ftpname);
        std::string sitfile = fopt->trplist;
        /* download the IGS ZPD file site-by-site */
        std::ifstream sitlst(sitfile.c_str());
        if (!sitlst.is_open())
        {
            Logger::Trace(TERROR, "*** ERROR(FtpUtil::GetTrop): open site.list = " + sitfile +
                " file failed, please check it");
            return;
        }

        std::string sitname;
        while (getline(sitlst, sitname))
        {
            if (sitname[0] == '#') continue;
            CString::trim(sitname);
            if (sitname.size() != 4) continue;
            CString::ToLower(sitname);
            std::string zpdfileshort = sitname + sdoy + "0." + syy + "zpd";
            CString::ToUpper(sitname);
            std::string zpdfilelongkey = "IGS0OPSFIN_" + syyyy + sdoy + "0000_01D_05M_" + sitname;
            std::string zpdfilelong, zpdfile;
            CString::GetFile(subdir, zpdfilelongkey, zpdfilelong);
            if (access(zpdfileshort.c_str(), 0) == -1 && access(zpdfilelong.c_str(), 0) == -1)
            {
                std::string url, cutdirs = " --cut-dirs=7 ";
                if (ftpname == "CDDIS") url = ftparchive_.CDDIS[IDX_ZTD] + "/" +
                    syyyy + "/" + sdoy;
                else if (ftpname == "IGN")
                {
                    url = ftparchive_.IGN[IDX_ZTD] + "/" + syyyy + "/" + sdoy;
                    cutdirs = " --cut-dirs=6 ";
                }
                else if (ftpname == "WHU") url = ftparchive_.WHU[IDX_ZTD] + "/" +
                    syyyy + "/" + sdoy;

                /* it is OK for '*.Z' or '*.gz' format */
                std::string zpdxfile = zpdfilelongkey + "*";
                std::string cmd = wgetfull + " " + qr + " -nH -A " + zpdxfile + cutdirs + url;
                std::system(cmd.c_str());

                int isfound = 0;  /* 1: long file name; 2: short filename */
                CString::GetFile(subdir, zpdfilelongkey, zpdxfile);
                if (access(zpdxfile.c_str(), 0) == 0)
                {
                    zpdfile = zpdxfile.substr(0, (int)zpdxfile.find_last_of('.'));
                    isfound = 1;
                }
                else
                {
                    /* it is OK for '*.Z' or '*.gz' format */
                    zpdxfile = zpdfileshort + ".*";
                    cmd = wgetfull + " " + qr + " -nH -A " + zpdxfile + cutdirs + url;
                    std::system(cmd.c_str());

                    CString::GetFile(subdir, zpdfileshort, zpdxfile);
                    if (access(zpdxfile.c_str(), 0) == 0)
                    {
                        zpdfile = zpdfileshort;
                        isfound = 2;
                    }
                }

                std::string localfile = subdir + sep + zpdfile, url0;
                if (isfound == 0)
                {
                    Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetTrop): failed to download IGS tropospheric product file " + zpdfile);

                    url0 = url + '/' + zpdxfile;
                    if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetTrop): " << url0 << "  ->  " <<
                        localfile << "  failed" << std::endl;
                }
                else
                {
                    /* extract '*.gz' or '*.Z' */
                    cmd = gzipfull + " -d -f " + zpdxfile;
                    std::system(cmd.c_str());
                    if (access(zpdfile.c_str(), 0) == 0)
                    {
                        Logger::Trace(TINFO, "*** INFO(FtpUtil::GetTrop): successfully download IGS tropospheric product file " + zpdfile);

                        url0 = url + '/' + zpdxfile;
                        if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetTrop): " << url0 << "  ->  " <<
                            localfile << "  OK" << std::endl;

                        /* to check if it needs to convert troposphere files with long name to those with short name */
                        if (isfound == 1 && fopt->l2s4trp > 0)
                        {
                            std::string change_filename;
    #ifdef _WIN32  /* for Windows */
                            if (fopt->l2s4trp == 1) change_filename = "rename";
                            else if (fopt->l2s4trp == 2) change_filename = "copy";
    #else          /* for Linux or Mac */
                            if (fopt->l2s4trp == 1) change_filename = "mv";
                            else if (fopt->l2s4trp == 2) change_filename = "cp";
    #endif
                            cmd = change_filename + " " + zpdfile + " " + zpdfileshort;
                            std::system(cmd.c_str());
                        }
                    }
                }
            }
            else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetTrop): IGS tropospheric product file " +
                zpdfileshort + " or " + zpdfilelong + " has existed!");
        }
        /* close 'site.list' */
        sitlst.close();
    }
    else if (ac == "cod")
    {
        /* creation of sub-directory ('daily') */
        std::string subdir = dir + sep + "CODE";
        if (access(subdir.c_str(), 0) == -1)
        {
            /* If the directory does not exist, creat it */
    #ifdef _WIN32   /* for Windows */
            std::string cmd = "mkdir " + subdir;
    #else           /* for Linux or Mac */
            std::string cmd = "mkdir -p " + subdir;
    #endif
            std::system(cmd.c_str());
        }

        /* change directory */
    #ifdef _WIN32   /* for Windows */
        _chdir(subdir.c_str());
    #else           /* for Linux or Mac */
        chdir(subdir.c_str());
    #endif

        std::string trpfileshort = "COD" + swwww + sdow + ".TRO";
        std::string trpfilelong = "COD0OPSFIN_" + syyyy + sdoy + "0000_01D_01H_TRO.TRO";
        std::string trpfile;
        if (access(trpfileshort.c_str(), 0) == -1 && access(trpfilelong.c_str(), 0) == -1)
        {
            std::string url = "ftp://ftp.aiub.unibe.ch/CODE/" + syyyy;
            std::string cutdirs = " --cut-dirs=2 ";

            /* it is OK for '*.Z' or '*.gz' format */
            std::string trpxfile = trpfilelong + ".*";
            std::string cmd = wgetfull + " " + qr + " -nH -A " + trpxfile + cutdirs + url;
            std::system(cmd.c_str());

            int isfound = 0;  /* 1: long file name; 2: short filename */
            CString::GetFile(subdir, trpfilelong, trpxfile);
            if (access(trpxfile.c_str(), 0) == 0)
            {
                trpfile = trpfilelong;
                isfound = 1;
            }
            else
            {
                trpxfile = trpfileshort + ".*";
                cmd = wgetfull + " " + qr + " -nH -A " + trpxfile + cutdirs + url;
                std::system(cmd.c_str());

                CString::GetFile(subdir, trpfileshort, trpxfile);
                if (access(trpxfile.c_str(), 0) == 0)
                {
                    trpfile = trpfileshort;
                    isfound = 2;
                }
            }

            std::string localfile = subdir + sep + trpfile, url0;
            if (isfound == 0)
            {
                Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetTrop): failed to download CODE tropospheric product file " + trpfile);

                url0 = url + '/' + trpxfile;
                if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetTrop): " << url0 << "  ->  " <<
                    localfile << "  failed" << std::endl;
            }
            else
            {
                /* extract '*.gz' or '*.Z' */
                cmd = gzipfull + " -d -f " + trpxfile;
                std::system(cmd.c_str());
                if (access(trpfile.c_str(), 0) == 0)
                {
                    Logger::Trace(TINFO, "*** INFO(FtpUtil::GetTrop): successfully download CODE tropospheric product file " + trpfile);

                    url0 = url + '/' + trpxfile;
                    if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetTrop): " << url0 << "  ->  " <<
                        localfile << "  OK" << std::endl;

                    /* to check if it needs to convert troposphere files with long name to those with short name */
                    if (isfound == 1 && fopt->l2s4trp > 0)
                    {
                        std::string change_filename;
#ifdef _WIN32  /* for Windows */
                        if (fopt->l2s4trp == 1) change_filename = "rename";
                        else if (fopt->l2s4trp == 2) change_filename = "copy";
#else          /* for Linux or Mac */
                        if (fopt->l2s4trp == 1) change_filename = "mv";
                        else if (fopt->l2s4trp == 2) change_filename = "cp";
#endif
                        cmd = change_filename + " " + trpfilelong + " " + trpfileshort;
                        std::system(cmd.c_str());
                    }
                }
            }
        }
        else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetTrop): CODE tropospheric product file " +
            trpfileshort + " or " + trpfilelong + " has existed!");
    }
} /* end of GetTrop */

/**
* @brief   : GetAntexIGS - to download ANTEX file from IGS
* @param[I]: ts (start time)
* @param[I]: dir (data directory)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::GetAntexIGS(gtime_t ts, std::string dir, const ftpopt_t* fopt)
{
    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(dir.c_str());
#else           /* for Linux or Mac */
    chdir(dir.c_str());
#endif

    std::vector<std::string> atxfiles = {"igs14.atx", "igs20.atx"};
    std::string atxfile;
    for (size_t i = 0; i < atxfiles.size(); i++)
    {
        atxfile = atxfiles[i];
        if (access(atxfile.c_str(), 0) == -1)
        {
            std::string wgetfull = fopt->wgetfull, qr = fopt->qr;
            std::string url = "https://files.igs.org/pub/station/general/" + atxfile;
            std::string cmd = wgetfull + " " + qr + " -nH --cut-dirs=3 " + url;
            std::system(cmd.c_str());

            std::string sep;
            sep.push_back((char)FILEPATHSEP);
            std::string localfile = dir + sep + atxfile;
            if (access(atxfile.c_str(), 0) == 0)
            {
                Logger::Trace(TINFO, "*** INFO(FtpUtil::GetAntexIGS): successfully download IGS ANTEX file " + atxfile);
                if (fplog_.is_open()) fplog_ << "* INFO(FtpUtil::GetAntexIGS): " << url << "  ->  " <<
                    localfile << "  OK" << std::endl;
            }
            else
            {
                Logger::Trace(TWARNING, "*** WARNING(FtpUtil::GetAntexIGS): failed to download IGS ANTEX file " + atxfile);
                if (fplog_.is_open()) fplog_ << "* WARNING(FtpUtil::GetAntexIGS): " << url << "  ->  " <<
                    localfile << "  failed" << std::endl;
            }
        }
        else Logger::Trace(TINFO, "*** INFO(FtpUtil::GetAntexIGS): IGS ANTEX file " + atxfile + " has existed!");
    }
} /* end of GetAntexIGS */

/**
* @brief   :  -
* @param[I]:
* @param[O]:
* @return  :
* @note    :
**/
std::string FtpUtil::LoadSp3File(gtime_t ts, std::string dir, std::string ac_s, bool longname)
{
    std::string sep;
    sep.push_back((char)FILEPATHSEP);
    int yyyy, doy, wwww, dow;
    double sow;
    std::string yyyy_s, doy_s, wwww_s, dow_s;
    GTime::time2yrdoy(ts, yyyy, doy);
    yyyy_s = CString::int2str(yyyy, 4);
    doy_s = CString::int2str(doy, 3);
    dow = GTime::time2gpst(ts, wwww, sow);
    wwww_s = CString::int2str(wwww, 4);
    dow_s = std::to_string(dow);
    CString::ToLower(ac_s);
    std::string ac = ac_s;
    std::string sp3file, filename;
    bool isfound = false;
    if (longname) /* long file name */
    {
        bool isMGEX = false;
        if (ac_s == "cod_m") ac = "cod";
        else if (ac_s == "gfz_m") ac = "gfz";
        else if (ac_s == "grg_m") ac = "grg";
        else if (ac_s == "iac_m") ac = "iac";
        else if (ac_s == "jax_m") ac = "jax";
        else if (ac_s == "sha_m") ac = "sha";
        else if (ac_s == "whu_m") ac = "whu";
        if (ac_s.find("_m") != std::string::npos) isMGEX = true;
        CString::ToUpper(ac);
        std::vector<std::string> str;
        if (isMGEX)
        {
            str.push_back(ac + "0MGXFIN_" + yyyy_s + doy_s + "0000_01D_");
            str.push_back(ac + "0MGXRAP_" + yyyy_s + doy_s + "0000_01D_");
        }
        else
        {
            str.push_back(ac + "0OPSFIN_" + yyyy_s + doy_s + "0000_01D_");
            str.push_back(ac + "0OPSRAP_" + yyyy_s + doy_s + "0000_01D_");
        }
        std::vector<std::string> sp3files;
        for (size_t i = 0; i < str.size(); i++)
        {
            CString::GetFilesAll(dir, str[i], sp3files);
            if ((int)sp3files.size() > 0) sp3file = sp3files[0];
            if (access(sp3file.c_str(), 0) == 0)
            {
                isfound = true;
                break;
            }
        }
        if (isfound) filename = sp3file;
        else filename = "";
    }
    else  /* short file name */
    {
        if (ac_s == "cod_m") ac = "com";
        else if (ac_s == "gfz_m") ac = "gbm";
        else if (ac_s == "grg_m") ac = "grm";
        else if (ac_s == "iac_m") ac = "iac";
        else if (ac_s == "jax_m") ac = "jax";
        else if (ac_s == "sha_m") ac = "sha";
        else if (ac_s == "whu_m") ac = "whu";
        sp3file = dir + sep + ac + wwww_s + dow_s + ".sp3";
        if (ac == "cod") sp3file = dir + sep + ac + wwww_s + dow_s + ".eph";
        if (access(sp3file.c_str(), 0) == 0) filename = sp3file;
        else filename = "";
    }

    return filename;
} /* end of loadSp3File */

/**
* @brief   :  -
* @param[I]:
* @param[O]:
* @return  :
* @note    :
**/
bool FtpUtil::Sp3FilesIntoOneFile(std::vector<std::string> sp3files)
{
    if ((int)sp3files.size() != 3) return false;
    if ((int)sp3files[1].length() == 0) return false;
    std::string origin_file = sp3files[1] + ".orig";
    if (access(origin_file.c_str(), 0) == -1)
    {
        std::string change_filename;
#ifdef _WIN32  /* for Windows */
        change_filename = "rename";
#else          /* for Linux or Mac */
        change_filename = "mv";
#endif
        std::string cmd = change_filename + " " + sp3files[1] + " " + origin_file;
        std::system(cmd.c_str());
    }
    std::fstream filefp_out, filefp0_in, filefp1_in, filefp2_in;
    std::string sline0, sline1, sline2;
    filefp_out.open(sp3files[1], std::ios_base::out);
    filefp1_in.open(origin_file, std::ios_base::in);
    while (std::getline(filefp1_in, sline1))  /* to read and store the header components of the second SP3 file */
    {
        if (sline1.substr(0, 2) == "* ")  /* the first record of data block */
        {
            break;
        }
        filefp_out << sline1 << std::endl;
    }

    filefp0_in.open(sp3files[0], std::ios_base::in);
    while (std::getline(filefp0_in, sline0))  /* to skip the header components of the first SP3 file */
    {
        if (sline0.substr(0, 2) == "* ")  /* the first record of data block */
        {
            break;
        }
    }
    filefp_out << sline0 << std::endl;
    while (std::getline(filefp0_in, sline0))  /* to read the data block of the first SP3 file */
    {
        if (filefp0_in.eof()) break;
        if (sline0.find("EOF") != std::string::npos) break;
        filefp_out << sline0 << std::endl;
    }

    filefp_out << sline1 << std::endl;
    while (std::getline(filefp1_in, sline1))  /* to read the data block of the second SP3 file */
    {
        if (filefp1_in.eof()) break;
        if (sline1.find("EOF") != std::string::npos) break;
        filefp_out << sline1 << std::endl;
    }

    filefp2_in.open(sp3files[2], std::ios_base::in);
    while (std::getline(filefp2_in, sline2))  /* to skip the header components of the third SP3 file */
    {
        if (sline2.substr(0, 2) == "* ")  /* the first record of data block */
        {
            break;
        }
    }
    filefp_out << sline2 << std::endl;
    while (std::getline(filefp2_in, sline2))  /* to read the data block of the third SP3 file */
    {
        if (filefp2_in.eof()) break;
        filefp_out << sline2 << std::endl;
    }

    filefp_out.close();
    filefp0_in.close();
    filefp1_in.close();
    filefp2_in.close();

    return true;
} /* end of Sp3FilesIntoOneFile */

/**
* @brief   : MergeSp3Files - to merge three consecutive sp3 files into one file
* @param[I]: ts (start time)
* @param[I]: dir (orbit directory)
* @param[I]: ac (analysis center, i.e., 'igs', 'cod', et al.)
* @param[I]: fopt (FTP options)
* @param[O]: none
* @return  : none
* @note    :
**/
void FtpUtil::MergeSp3Files(gtime_t ts, std::string dir, std::string ac)
{
    /* change directory */
#ifdef _WIN32   /* for Windows */
    _chdir(dir.c_str());
#else           /* for Linux or Mac */
    chdir(dir.c_str());
#endif

    std::vector<std::string> sp3fileslong;
    gtime_t tt = GTime::TimeAdd(ts, -86400.0);
    for (int i = 0; i < 3; i++)
    {
        sp3fileslong.push_back(this->LoadSp3File(tt, dir, ac, true));  /* SP3 files with long file name */
        tt = GTime::TimeAdd(tt, 86400.0);
    }
    this->Sp3FilesIntoOneFile(sp3fileslong);

    std::vector<std::string> sp3filesshort;
    tt = GTime::TimeAdd(ts, -86400.0);
    for (int i = 0; i < 3; i++)
    {
        sp3filesshort.push_back(this->LoadSp3File(tt, dir, ac, false));  /* SP3 files with short file name */
        tt = GTime::TimeAdd(tt, 86400.0);
    }
    this->Sp3FilesIntoOneFile(sp3filesshort);
} /* end of MergeSp3Files */

/**
* @brief     : FtpDownload - GNSS data downloading via FTP
* @param[I]  : popt (processing options)
* @param[I/O]: fopt (FTP options)
* @return    : none
* @note      :
**/
void FtpUtil::FtpDownload(const prcopt_t *popt, ftpopt_t* fopt)
{
    /* Get FTP archive for CDDIS, IGN, or WHU */
    init();

    /* setting of the third-party softwares (.i.e, wget, gzip, crx2rnx etc.) */
    std::string sep;
    sep.push_back((char)FILEPATHSEP);

#ifdef _WIN32   /* for Windows */
    /* for wget */
    fopt->wgetfull = popt->dir3party + sep + "wget";
    CString::trim(fopt->wgetfull);
    /* for gzip */
    fopt->gzipfull = popt->dir3party + sep + "gzip";
    CString::trim(fopt->gzipfull);
    /* for crx2rnx */
    fopt->crx2rnxfull = popt->dir3party + sep + "crx2rnx";
    CString::trim(fopt->crx2rnxfull);
#else           /* for Linux or Mac */
    fopt->wgetfull = "wget";
    fopt->gzipfull = "gzip";
    /* for crx2rnx */
    fopt->crx2rnxfull = popt->dir3party + sep + "crx2rnx";
    CString::trim(fopt->crx2rnxfull);
#endif

    /* "-qr" (printInfoWget == false, not print information of 'wget') or "r" (printInfoWget == true, print information of 'wget') in 'wget' command line */
    if (fopt->printinfo4wget) fopt->qr = "-r";
    else fopt->qr = "-qr";

    /* If the directory does not exist, creat it */
    if (access(popt->logdir.c_str(), 0) == -1)
    {
#ifdef _WIN32   /* for Windows */
        std::string cmd = "mkdir " + popt->logdir;
#else           /* for Linux or Mac */
        std::string cmd = "mkdir -p " + popt->logdir;
#endif
        std::system(cmd.c_str());
    }
    fplog_.open(popt->logfil, std::ios::out);

    /* IGS observation (short name 'd') downloaded */
    if (fopt->getobs)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->obsdir.c_str(), 0) == -1)
        {
#ifdef _WIN32   /* for Windows */
            std::string cmd = "mkdir " + popt->obsdir;
#else           /* for Linux or Mac */
            std::string cmd = "mkdir -p " + popt->obsdir;
#endif
            std::system(cmd.c_str());
        }

        std::string obstyp = fopt->obstype;
        CString::trim(obstyp);
        CString::ToLower(obstyp);
        std::vector<std::string> obsfroms = { "igs", "mgex", "igm", "cut", "hk", "ngs", "epn", "pbo", "chi" };
        std::string obsfrom = fopt->obsfrom;
        CString::trim(obsfrom);
        CString::ToLower(obsfrom);
        for (int i = 0; i < (int)obsfroms.size(); i++)
        {
            if (obsfrom == obsfroms[i])
            {
                std::string obsufrom = obsfrom;
                CString::ToUpper(obsufrom);
                /* creation of sub-directory */
                std::string subobsdir = popt->obsdir + sep + obsufrom;
                if (access(subobsdir.c_str(), 0) == -1)
                {
                    /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                    std::string cmd = "mkdir " + subobsdir;
#else           /* for Linux or Mac */
                    std::string cmd = "mkdir -p " + subobsdir;
#endif
                    std::system(cmd.c_str());
                }

                if (obsfrom == "igs")       /* IGS observation (RINEX version 2.xx, short name "d") */
                {
                    if (obstyp == "daily") GetDailyObsIgs(popt->ts, subobsdir, fopt);
                    else if (obstyp == "hourly") GetHourlyObsIgs(popt->ts, subobsdir, fopt);
                    else if (obstyp == "highrate") GetHrObsIgs(popt->ts, subobsdir, fopt);
                }
                else if (obsfrom == "mgex") /* MGEX observation (RINEX version 3.xx, long name "crx") */
                {
                    if (obstyp == "daily") GetDailyObsMgex(popt->ts, subobsdir, fopt);
                    else if (obstyp == "hourly") GetHourlyObsMgex(popt->ts, subobsdir, fopt);
                    else if (obstyp == "highrate") GetHrObsMgex(popt->ts, subobsdir, fopt);
                }
                else if (obsfrom == "igm")  /* the union of IGS and MGEX observation with respect to the site name */
                {
                    if (obstyp == "daily") GetDailyObsIgm(popt->ts, subobsdir, fopt);
                    else if (obstyp == "hourly") GetHourlyObsIgm(popt->ts, subobsdir, fopt);
                    else if (obstyp == "highrate") GetHrObsIgm(popt->ts, subobsdir, fopt);
                }
                else if (obsfrom == "cut")  /* Curtin University of Technology (CUT) observation (RINEX version 3.xx, long name "crx") */
                {
                    if (obstyp == "daily") GetDailyObsCut(popt->ts, subobsdir, fopt);
                }
                else if (obsfrom == "hk")   /* Hong Kong CORS observation (RINEX version 3.xx, long name "crx") */
                {
                    if (obstyp == "30s") Get30sObsHk(popt->ts, subobsdir, fopt);
                    else if (obstyp == "5s" || obstyp == "05s") Get5sObsHk(popt->ts, subobsdir, fopt);
                    else if (obstyp == "1s" || obstyp == "01s") Get1sObsHk(popt->ts, subobsdir, fopt);
                }
                else if (obsfrom == "ngs")  /* NGS/NOAA CORS observation (RINEX version 2.xx, short name "d") */
                {
                    if (obstyp == "daily") GetDailyObsNgs(popt->ts, subobsdir, fopt);
                }
                else if (obsfrom == "epn")  /* EUREF Permanent Network (EPN) observation (RINEX version 3.xx, long name "crx" and RINEX version 2.xx, short name "d") */
                {
                    if (obstyp == "daily") GetDailyObsEpn(popt->ts, subobsdir, fopt);
                }
                else if (obsfrom == "pbo") /* Plate Boundary Observatory (PBO) observation (RINEX version 3.xx, long name "crx") */
                {
                    if (obstyp == "daily") GetDailyObsPbo(popt->ts, subobsdir, fopt);
                }
                else if (obsfrom == "chi") /*  Observations (RINEX version 2.xx, short name "d") from Centro Sismologico Nacional of Universidad de Chile */
                {
                    if (obstyp == "daily") GetDailyObsChi(popt->ts, subobsdir, fopt);
                }
            }
        }
    }

    /* broadcast ephemeris downloaded */
    if (fopt->getnav)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->navdir.c_str(), 0) == -1)
        {
#ifdef _WIN32   /* for Windows */
            std::string cmd = "mkdir " + popt->navdir;
#else           /* for Linux or Mac */
            std::string cmd = "mkdir -p " + popt->navdir;
#endif
            std::system(cmd.c_str());
        }

        GetNav(popt->ts, popt->navdir.c_str(), fopt);
    }

    /* precise orbit and clock product downloaded */
    if (fopt->getorbclk)
    {
        std::string ac = fopt->orbclkfrom;
        CString::trim(ac);
        CString::ToLower(ac);
        std::vector<std::string> acs;
        int ipos = (int)ac.find_first_of('+');
        if (ipos > 0) acs = CString::split(ac, "+");
        else
        {
            if (ac == "all" || ac == "all_m")
            {
                if (ac == "all") acs = { "cod", "emr", "esa", "gfz", "grg", "igs", "jgx", "jpl", "mit" };
                else if (ac == "all_m") acs = { "cod_m", "gfz_m", "grg_m", "iac_m", "jax_m", "sha_m", "whu_m" };
            }
            else acs.push_back(ac);
        }
        for (int i = 0; i < (int)acs.size(); i++)
        {
            std::string ac_i = acs[i];
            int prodtype = 0;

            /* cnt: CNES real-time orbit and clock products from CNES offline files */
            if (ac_i == "cnt") prodtype = PROD_REALTIME;

            /* esa_u: ESA ultra-rapid orbit and clock products
               gfz_u: GFZ ultra-rapid orbit and clock products
               igs_u: IGS ultra-rapid orbit and clock products
               whu_u: WHU ultra-rapid orbit and clock products
            */
            std::vector<std::string> ultra_ac = { "esa_u", "gfz_u", "igs_u", "whu_u" };
            auto isfound = std::find(ultra_ac.begin(), ultra_ac.end(), ac_i);
            if (isfound != ultra_ac.end()) prodtype = PROD_ULTRA_RAPID;

            /* cod_r: CODE rapid orbit and clock products
               emr_r: NRCan rapid orbit and clock products
               esa_r: ESA rapid orbit and clock products
               gfz_r: GFZ rapid orbit and clock products
               igs_r: IGS rapid orbit and clock products
            */
            std::vector<std::string> rapid_ac = { "cod_r", "emr_r", "esa_r", "gfz_r", "igs_r" };
            isfound = std::find(rapid_ac.begin(), rapid_ac.end(), ac_i);
            if (isfound != rapid_ac.end()) prodtype = PROD_RAPID;

            /* cod: CODE final orbit and clock products
               emr: NRCan final orbit and clock products
               esa: ESA final orbit and clock products
               gfz: GFZ final orbit and clock products
               grg: CNES final orbit and clock products
               igs: IGS final orbit and clock products
               jpl: JPL final orbit and clock products
               mit: MIT final orbit and clock products
            */
            std::vector<std::string> final_ac_IGS = { "cod", "emr", "esa", "gfz", "grg", "igs", "jgx", "jpl", "mit" };
            isfound = std::find(final_ac_IGS.begin(), final_ac_IGS.end(), ac_i);
            if (isfound != final_ac_IGS.end()) prodtype = PROD_FINAL_IGS;

            /* cod_m: CODE multi-GNSS final orbit and clock products
               gfz_m: GFZ multi-GNSS final orbit and clock products
               grg_m: CNES multi-GNSS final orbit and clock products
               iac_m: Russia Information and Analysis Center (IAC) multi-GNSS final orbit and clock products
               jax_m: Japan Aerospace Exploration Agency (JAXA) multi-GNSS final orbit and clock products
               sha_m: Shanghai Observatory (SHAO) multi-GNSS final orbit and clock products
               whu_m: Wuhan University (WHU) multi-GNSS final orbit and clock products
            */
            std::vector<std::string> final_ac_MGEX = { "cod_m", "gfz_m", "grg_m", "iac_m", "jax_m", "sha_m", "whu_m" };
            isfound = std::find(final_ac_MGEX.begin(), final_ac_MGEX.end(), ac_i);
            if (isfound != final_ac_MGEX.end()) prodtype = PROD_FINAL_MGEX;

            /* creation of sub-directory */
            std::string suborbdir = popt->orbdir;
            std::string subclkdir = popt->clkdir;
            /* for orb at a specified day */
            if (access(suborbdir.c_str(), 0) == -1)
            {
                /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                std::string cmd = "mkdir " + suborbdir;
#else           /* for Linux or Mac */
                std::string cmd = "mkdir -p " + suborbdir;
#endif
                std::system(cmd.c_str());
            }
            /* for clk at a specified day */
            if (prodtype > 0 && prodtype != PROD_ULTRA_RAPID)
            {
                if (access(subclkdir.c_str(), 0) == -1)
                {
                    /* If the directory does not exist, creat it */
#ifdef _WIN32   /* for Windows */
                    std::string cmd = "mkdir " + subclkdir;
#else           /* for Linux or Mac */
                    std::string cmd = "mkdir -p " + subclkdir;
#endif
                    std::system(cmd.c_str());
                }
            }
            std::vector<std::string> subdirs = { suborbdir, subclkdir };
            GetOrbClk(popt->ts, subdirs, prodtype, ac_i, fopt);

            if (fopt->minus_add_1day && (prodtype == PROD_FINAL_IGS || prodtype == PROD_FINAL_MGEX))
            {

                gtime_t tt;
                /* precise orbit and clock product downloaded for the day before the specified day */
                tt = GTime::TimeAdd(popt->ts, -86400.0);
                GetOrbClk(tt, subdirs, prodtype, ac_i, fopt);

                /* precise orbit and clock product downloaded for the day after the specified day */
                tt = GTime::TimeAdd(popt->ts, 86400.0);
                GetOrbClk(tt, subdirs, prodtype, ac_i, fopt);
            }

            if (fopt->merge_sp3files)
            {
                this->MergeSp3Files(popt->ts, suborbdir, ac_i);
            }
        }
    }

    /* EOP file downloaded */
    if (fopt->geteop)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->eopdir.c_str(), 0) == -1)
        {
#ifdef _WIN32   /* for Windows */
            std::string cmd = "mkdir " + popt->eopdir;
#else           /* for Linux or Mac */
            std::string cmd = "mkdir -p " + popt->eopdir;
#endif
            std::system(cmd.c_str());
        }

        GetEop(popt->ts, popt->eopdir.c_str(), fopt);
    }

    /* ORBEX (ORBit EXchange format) downloaded */
    if (fopt->getobx)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->obxdir.c_str(), 0) == -1)
        {
#ifdef _WIN32   /* for Windows */
            std::string cmd = "mkdir " + popt->obxdir;
#else           /* for Linux or Mac */
            std::string cmd = "mkdir -p " + popt->obxdir;
#endif
            std::system(cmd.c_str());
        }

        GetObx(popt->ts, popt->obxdir.c_str(), fopt);
    }


    /* differential code/signal bias (DCB/DSB) files downloaded */
    if (fopt->getdsb)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->biadir.c_str(), 0) == -1)
        {
#ifdef _WIN32   /* for Windows */
            std::string cmd = "mkdir " + popt->biadir;
#else           /* for Linux or Mac */
            std::string cmd = "mkdir -p " + popt->biadir;
#endif
            std::system(cmd.c_str());
        }

        GetDsb(popt->ts, popt->biadir.c_str(), fopt);
    }


    /* obsevable-specific code/phase signal bias (OSB) files downloaded */
    if (fopt->getosb)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->biadir.c_str(), 0) == -1)
        {
#ifdef _WIN32   /* for Windows */
            std::string cmd = "mkdir " + popt->biadir;
#else           /* for Linux or Mac */
            std::string cmd = "mkdir -p " + popt->biadir;
#endif
            std::system(cmd.c_str());
        }

        GetOsb(popt->ts, popt->biadir.c_str(), fopt);
    }

    /* IGS SINEX file downloaded */
    if (fopt->getsnx)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->snxdir.c_str(), 0) == -1)
        {
#ifdef _WIN32   /* for Windows */
            std::string cmd = "mkdir " + popt->snxdir;
#else           /* for Linux or Mac */
            std::string cmd = "mkdir -p " + popt->snxdir;
#endif
            std::system(cmd.c_str());
        }

        GetSnx(popt->ts, popt->snxdir.c_str(), fopt);
    }

    /* global ionosphere map (GIM) downloaded */
    if (fopt->getion)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->iondir.c_str(), 0) == -1)
        {
#ifdef _WIN32   /* for Windows */
            std::string cmd = "mkdir " + popt->iondir;
#else           /* for Linux or Mac */
            std::string cmd = "mkdir -p " + popt->iondir;
#endif
            std::system(cmd.c_str());
        }

        GetIono(popt->ts, popt->iondir.c_str(), fopt);
    }

    /* Rate of TEC index (ROTI) file downloaded */
    if (fopt->getroti)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->iondir.c_str(), 0) == -1)
        {
#ifdef _WIN32   /* for Windows */
            std::string cmd = "mkdir " + popt->iondir;
#else           /* for Linux or Mac */
            std::string cmd = "mkdir -p " + popt->iondir;
#endif
            std::system(cmd.c_str());
        }

        GetRoti(popt->ts, popt->iondir.c_str(), fopt);
    }

    /* final tropospheric product downloaded */
    if (fopt->gettrp)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->ztddir.c_str(), 0) == -1)
        {
#ifdef _WIN32   /* for Windows */
            std::string cmd = "mkdir " + popt->ztddir;
#else           /* for Linux or Mac */
            std::string cmd = "mkdir -p " + popt->ztddir;
#endif
            std::system(cmd.c_str());
        }

        GetTrop(popt->ts, popt->ztddir.c_str(), fopt);
    }

    /* IGS ANTEX file downloaded */
    if (fopt->getatx)
    {
        /* If the directory does not exist, creat it */
        if (access(popt->tbldir.c_str(), 0) == -1)
        {
#ifdef _WIN32   /* for Windows */
            std::string cmd = "mkdir " + popt->tbldir;
#else           /* for Linux or Mac */
            std::string cmd = "mkdir -p " + popt->tbldir;
#endif
            std::system(cmd.c_str());
        }

        GetAntexIGS(popt->ts, popt->tbldir.c_str(), fopt);
    }

    if (fplog_.is_open()) fplog_.close();  /* to close the log file */
} /* end of FtpDownload */