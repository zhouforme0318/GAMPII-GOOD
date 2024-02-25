/*------------------------------------------------------------------------------
* types.h : types definition
*
* Copyright (C) 2020-2099 by Ubiquitous Navigation & Integrated positioning lab in Quest (UNIQ), all rights reserved.
*    This file is part of GAMP II - GOOD (Gnss Observations and prOducts Downloader)
*
* history : 2020/08/16 1.0  new (by Feng Zhou)
*           2020/10/12 3.0  mojor modifications (by Feng Zhou)
*-----------------------------------------------------------------------------*/
#ifndef TYPES_H
#define TYPES_H

#include "common.h"

/* type definitions ----------------------------------------------------------*/
struct gtime_t
{                                 /* time struct */
    int mjd;                      /* modified Julian date (MJD) */
    double sod;                   /* seconds of the day */
};

struct ftpopt_t
{                                 /* the type of GNSS data downloading */
    bool ftpdownloading;          /* the master switch for data downloading, 0:off  1:on, only for data downloading */
    std::string ftpfrom;          /* FTP archive: CDDIS, IGN, or WHU */
    bool getobs;                  /* (0:off  1:on) GNSS observation data downloading option */
    std::string obstype;          /* "daily", "hourly", "highrate", "30s", "5s", or "1s" */
    std::string obsfrom;          /* where to download the observations (i.e., "igs", "mgex", "igm", "cut", "ga", "hk", "ngs", or "epn") */
    std::string obslist;          /* 'all'; the full path of 'site.list' */
    std::vector<int> hhobs;       /* hours array (0: 00:00, 1: 1:00, 2: 2:00, 3: 3:00, ...) */
    int l2s4obs;                  /* valid only for the observation files with long name, 0: long name, 1: short name, 2: long and short name */
    bool getnav;                  /* (0:off  1:on) various broadcast ephemeris */
    std::string navtype;          /* 'daily' or 'hourly' */
    std::string navsys;           /* 'gps', 'glo', 'bds', 'gal', 'qzs', 'irn', 'mixed' or 'all' */
    std::string navfrom;          /* analysis center (i.e., "igs", "dlr", "ign", "gop", or "wrd") that carries out the combination of broadcast
                                     ephemeris for mixed navigation data */
    std::string navlist;          /* the full path of 'site.list' */
    std::vector<int> hhnav;       /* hours array (0: 00:00, 1: 1:00, 2: 2:00, 3: 3:00, ...) */
    int l2s4nav;                  /* valid only for the navigation files with long name, 0: long name, 1: short name, 2: long and short name */
    bool getorbclk;               /* (0:off  1:on) precise orbit 'sp3' and precise clock 'clk' */
    std::string orbclkfrom;       /* analysis center (i.e., final: "cod", "emr", "esa", "gfz", "grg", "igs", "jpl", "mit", "cod_m", "gfz_m",
                                     "grg_m", "whu_m"; rapid: "cod_r", "emr_r", "esa_r", "gfz_r", "igs_r"; ultra-rapid: "esa_u", "gfz_u",
                                     "igs_u", "whu_u"; real-time: "cnt"). NOTE: the option of "cnt" is for real-time precise orbit and
                                     clock products from CNES offline files */
    std::vector<std::vector<int>> hhorbclk;  /* hours array (0: 00:00, 6: 6:00, 12: 12:00, 18: 18:00 for esa_u and/or igs_u; 0: 00:00, 3: 3:00,
                                                 6: 6:00, ... for gfz_u; 0: 00:00, 1: 1:00, 2: 2:00, ... for whu_u) */
    int l2s4oc;                   /* valid only for the precise orbit and clock files with long name, 0: long name, 1: short name, 2: long and short name */
    bool getobx;                  /* (0:off  1:on) ORBEX (ORBit EXchange format) for satellite attitude information */
    std::string obxfrom;          /* analysis center (i.e., final/rapid: "cod_m", "gfz_m", "grg_m", "whu_m"; real-time: cnt). NOTE: the
                                     option of "cnt" is for real-time ORBEX from CNES offline files */
    bool geteop;                  /* (0:off  1:on) earth rotation parameter */
    std::string eopfrom;          /* analysis center (i.e., final: "cod", "emr", "esa", "gfz", "grg", "igs", "jpl", "mit";
                                     ultra: "esa_u", "gfz_u", "igs_u") */
    std::vector<int> hheop;       /* hours array (0: 00:00, 6: 6:00, 12: 12:00, 18: 18:00 for esa_u and/or igs_u; 0: 00:00, 3: 3:00,
                                     6: 6:00, ... for gfz_u) */
    int l2s4eop;                  /* valid only for EOP files with long name, 0: long name, 1: short name, 2: long and short name */
    bool getsnx;                  /* (0:off  1:on) IGS weekly SINEX */
    int l2s4snx;                  /* valid only for IGS weekly SINEX files with long name, 0: long name, 1: short name, 2: long and short name */
    bool getdsb;                  /* (0:off  1:on) differential code/signal bias (DCB/DSB) */
    std::string dsbfrom;          /* analysis center (i.e., "cod", "cas") */
    bool getosb;                  /* (0:off  1:on) observable-specific signal bias (OSB) */
    std::string osbfrom;          /* analysis center (i.e., final/rapid: "cod_m", "gfz_m", "grg_m", "whu_m"; real-time: cnt). NOTE: the
                                     option of "cnt" is for real-time OSBs from CNES offline files */
    bool getion;                  /* (0:off  1:on) global ionosphere map (GIM) */
    std::string ionfrom;          /* analysis center (i.e., "igs", "cod", "cas", ...) */
    int l2s4ion;                  /* valid only for the ionosphere files with long name, 0: long name, 1: short name, 2: long and short name */
    bool getroti;                 /* (0:off  1:on) rate of TEC index (ROTI) */
    bool gettrp;                  /* (0:off  1:on) tropospheric product */
    std::string trpfrom;          /* analysis center (i.e., "igs" or "cod") */
    std::string trplist;          /* 'all'; the full path of 'site.list' */
    int l2s4trp;                  /* valid only for the troposphere files with long name, 0: long name, 1: short name, 2: long and short name */
    bool getatx;                  /* (0:off  1:on) ANTEX format antenna phase center correction */
    std::string wgetfull;         /* if isPath3party == true, set the full path where 'wget' is */
    std::string gzipfull;         /* if isPath3party == true, set the full path where 'gzip' is */
    std::string crx2rnxfull;      /* if isPath3party == true, set the full path where 'crx2rnx' is */
    bool minus_add_1day;          /* (0:off  1:on) the day before and after the current day for precise satellite orbit and
                                     clock products downloading */
    bool merge_sp3files;          /* (0: off  1: on) to merge three consecutive sp3 files into one file */
    bool printinfo4wget;          /* (0:off  1 : on) print the information generated by 'wget' */
    std::string qr;               /* "-qr" (printInfoWget == false) or "r" (printInfoWget == true) in 'wget' command line */
};

struct prcopt_t
{                                 /* processing options type */
    /* processing directory */
    std::string maindir;          /* the root/main directory of GNSS observations and products */
    std::string obsdir;           /* the sub-directory of RINEX format observation files */
    std::string navdir;           /* the sub-directory of RINEX format broadcast ephemeris files */
    std::string orbdir;           /* the sub-directory of SP3 format precise ephemeris files */
    std::string clkdir;           /* the sub-directory of RINEX format precise clock files */
    std::string eopdir;           /* the sub-directory of earth rotation/orientation parameter (EOP) files */
    std::string obxdir;           /* the sub-directory of MGEX final/rapid and/or CNES real-time ORBEX (ORBit EXchange format) files */
    std::string snxdir;           /* the sub-directory of SINEX format IGS weekly solution files */
    std::string biadir;           /* the sub-directory of CODE/MGEX differential code/signal bias (DCB/DSB), MGEX observable-specific signal bias (OSB), and/or CNES real-time OSB files */
    std::string iondir;           /* the sub-directory of CODE/IGS global ionosphere map (GIM) files */
    std::string ztddir;           /* the sub-directory of CODE/IGS tropospheric product files */
    std::string tbldir;           /* the sub-directory of table files (i.e., ANTEX, ocean tide loading files, etc.) for processing */
    std::string logdir;           /* the sub-directory of log file */
    std::string dir3party;        /* the sub-directory where third-party softwares (i.e., 'wget', 'gzip', 'crx2rnx' etc) are stored */
    std::string logfil;           /* The log file with full path that gives the indications of whether the data downloading is successful or not */

    /* time settings */
    gtime_t ts;                   /* start time for processing */
    int ndays;                    /* number of consecutive days */
};

#endif  // TYPES_H