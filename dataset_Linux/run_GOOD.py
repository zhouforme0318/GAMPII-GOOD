#!/usr/bin/env python
# coding:utf-8


################################################################################
# PROGRAM:
################################################################################
"""

 To run GOOD software for GNSS observations and products downloading

 UsagD: python run_GOOD.py -exe <GOODpath> -datadir <mainDir> -time <yyyy> <doy> <ndays> -ftp <ftpArch> -obs <obsType> <obsFrom> <obsList>
           -nav <navType> <navSys> <navFrom> -orbclk <ocFrom> -eop <eopFrom> -obx <obxFrom> -dsb <dsbFrom> -osb <osbFrom> -snx -ion <ionFrom>
           -roti -trop <trpFrom> <trpList> -atx

 OPTIONS:
   <-exe>          - The full path of executable GOOD program
   <-datadir>      - The root/main directory of GNSS observations and products storage, i.e., '-datadir D:\data' under Windows or '-datadir /home/zhouforme/data' under Linux/Mac
   <-time>         - time setting, 1st: 4-digit year, 2nd: day of year, 3rd: number of consecutive days, i.e., '-time 2024 10 3'
   <-ftp>          - FTP archieve, i.e., '-ftp cddis', '-ftp whu', or '-ftp ign'
   <-obs>          - [optional] GNSS observation data downloading option:
                      1st <obsType>: 'daily', 'hourly', 'highrate', '30s', '5s', or '1s';
                      2nd <obsFrom>: 'igs', 'mgex', 'igm', 'cut', 'hk', 'ngs', 'epn', 'pbo', or 'chi';
                      3rd <obsList>: 'all' (observation files downloaded in the whole remote directory) or the file name of site list (observation files downloaded site-by-site according to the site list file);
                      i.e., '-obs daily mgex site_mgex.list' or '-obs daily igs all'
   <-nav>          - [optional] various broadcast ephemeris downloading option:
                      1st <navType>: 'daily' or 'hourly';
                      2nd <navSys> : 'gps', 'glo', 'bds', 'gal', 'qzs', 'irn', 'mixed3', 'mixed4', or 'all';
                      3rd <navFrom>: 'igs', 'dlr', 'ign', 'gop', or 'wrd';
                      i.e., '-nav daily mixed3 igs' or '-nav daily mixed4 dlr'
   <-orbclk>       - [optional] satellite final/rapid/ultra-rapid precise orbit and clock downloading option:
                      1st <ocFrom> : IGS final: 'cod', 'emr', 'esa', 'gfz', 'grg', 'igs', 'jgx', 'jpl', 'mit', 'all', 'cod+igs', 'cod+gfz+igs', ...;
                         MGEX final: 'cod_m', 'gfz_m', 'grg_m', 'iac_m', 'jax_m', 'sha_m', 'whu_m', 'all_m', 'cod_m+gfz_m', 'grg_m+whu_m', ...;
                         rapid: 'cod_r', 'emr_r', 'esa_r', 'gfz_r', 'igs_r';
                         ultra-rapid: 'esa_u', 'gfz_u', 'igs_u', 'whu_u';
                         real-timD: 'cnt';
                      i.e., '-orbclk igs' or '-orbclk igs+cod+cod_m'
   <-eop>          - [optional] Earth rotation/orientation parameter (ERP/EOP) downloading option:
                      1st <eopFrom>: final: 'cod', 'emr', 'esa', 'gfz', 'grg', 'igs', 'jpl', 'mit';
                         ultra-rapid: 'esa_u', 'gfz_u', 'igs_u';
                      i.e., '-eop igs' or '-eop igs_u'
   <-obx>          - [optional] ORBEX (ORBit EXchange format) for satellite attitude information downloading option:
                      1st <obxFrom>: final/rapid: 'cod', 'gfz', 'grg', 'whu', 'all'; real-time: 'cnt'
                      i.e., '-obx cod' or '-obx all'
   <-dsb>          - [optional] Differential code/signal bias (DCB/DSB) downloading option:
                      1st <dsbFrom>: 'cod', 'cas', 'all'
                      i.e., '-dsb cod' or '-dsb all'
   <-osb>          - [optional] Observable-specific signal bias (OSB) downloading option:
                      1st <osbFrom>: 'cas', 'cod', 'gfz', 'grg', 'whu', 'all'; real-time: 'cnt'
                      i.e., '-osb cod' or '-osb all'
   <-snx>          - [optional] IGS weekly SINEX downloading option
                      i.e., '-snx'
   <-ion>          - [optional] Global ionosphere map (GIM) downloading option:
                      1st <ionFrom>: final: 'cas', 'cod', 'emr', 'esa', 'igs', 'jpl', 'upc', 'all', 'cas+cod', 'cas+cod+igs', ...;
                      i.e., '-ion cod' or '-ion cas+cod' or '-ion all'
   <-roti>         - [optional] Rate of TEC index (ROTI) downloading option
                      i.e., '-roti'
   <-trop>         - [optional] CODE/IGS tropospheric product downloading option:
                      1st <trpFrom>: 'igs' or 'cod';
                      2nd <trpList>: 'all' (observation files downloaded in the whole remote directory) or the file name of site list (observation files downloaded site-by-site according to the site list file).
                      i.e., '-trop cod' or '-trop igs site_trp.list' or '-trop all'
   <-atx>          - [optional] ANTEX format antenna phase center correction downloading option
                      i.e., '-atx'

EXAMPLES: python run_GOOD.py -exe D:\data\run_GOOD.exe -datadir D:\data -time 2024 10 3 -ftp cddis -obs daily igs site_igs.list
          python run_GOOD.py -exe D:\data\run_GOOD.exe -datadir D:\data -time 2024 10 3 -ftp cddis -nav daily mixed3 igs
          python run_GOOD.py -exe D:\data\run_GOOD.exe -datadir D:\data -time 2024 10 3 -ftp cddis -orbclk igs
          python run_GOOD.py -exe D:\data\run_GOOD.exe -datadir D:\data -time 2024 10 3 -ftp cddis -orbclk igs -snx -atx
          python run_GOOD.py -exe D:\data\run_GOOD.exe -datadir D:\data -time 2024 10 3 -ftp cddis -nav daily mixed3 igs -orbclk gfz_m

Changes: 06-Apr-2022   fzhou: create the prototype of the scripts for version 1.0

 to get help, typD:
           python run_GOOD.py
 or        python run_GOOD.py -h

"""
################################################################################
# Import Python modules
import os, sys, pydoc
import platform


__author__ = 'Feng Zhou @ SDUST'
__date__ = '$DatD: 2022-04-06 07:05:20 (Mon, 06 Apr 2022) $'[7:-21]
__version__ = '$Version: run_GOOD.py V1.0 $'[10:-2]


################################################################################
# FUNCTION: batch processing in single point positioning (SPP) mode using GAMPII software
################################################################################
def main_run_GOOD():
    nargv = len(sys.argv)
    if '-h' in sys.argv or nargv < 2:
        pydoc.help(os.path.basename(sys.argv[0]).split('.')[0])
        return 0
    elif nargv >= 12:
        sep = ''
        if 'Windows' in platform.system():
            sep = '\\'
            thirdpartydir = 'thirdparty_Win'
        elif 'Linux' or 'Mac' in platform.system():
            sep = '/'
            thirdpartydir = 'thirdparty_Linux'
        datadir = ''
        GOOD_bin = ''
        index = 0
        if '-exe' in sys.argv:
            index = sys.argv.index('-exe')
            if sys.argv[index + 1][0] == '-':
                print('*** ERROR: The full path of executable GOOD program is NOT given! Please check it.\n')
                return 0
            else:
                GOOD_bin = sys.argv[index + 1]

        if '-datadir' in sys.argv:
            index = sys.argv.index('-datadir')
            if sys.argv[index + 1][0] == '-':
                print('*** ERROR: The full path of data directory is NOT given! Please check it.\n')
                return 0
            else:
                datadir = sys.argv[index + 1]
        if datadir == '':
            return 0

        cfgfile = datadir + sep + 'GOOD_temporary.yaml'
        with open(cfgfile,"w") as f_w:
            line = ''
            line += '# #> GAMP II: intelliGent Analysis system for Multi-sensor integrated navigation and Positioning v2.0\n'
            line += '#>   This configuration is for GAMP II - GOOD (GNSS Observations and prOducts Downloader)\n'
            line += '\n'

            line += '# The root/main directory of GNSS observations and products  -------------------\n'
            # to write the setting of main directory
            line += 'mainDir       : ' + datadir + '\n'

            # to write the setting of sub-directories
            line += '# The sub-directories of GNSS observations and products, which needs to inherit the path of \'mainDir\'\n'
            line += '#   i.e., \'orbDir\' = \'mainDir\' + \'orbDir\', which is \'D:\\Projects\\proj_VScode\\UNIQ\\GOOD\\dataset\\orb\'\n'
            line += 'obsDir        : obs               # The sub-directory of RINEX format observation files\n'
            line += 'navDir        : nav               # The sub-directory of RINEX format broadcast ephemeris files\n'
            line += 'orbDir        : orb               # The sub-directory of SP3 format precise ephemeris files\n'
            line += 'clkDir        : clk               # The sub-directory of RINEX format precise clock files\n'
            line += 'eopDir        : eop               # The sub-directory of earth rotation/orientation parameter (EOP) files\n'
            line += 'obxDir        : obx               # The sub-directory of MGEX final/rapid and/or CNES real-time ORBEX (ORBit EXchange format) files\n'
            line += 'biaDir        : bia               # The sub-directory of CODE/MGEX differential code/signal bias (DCB/DSB), MGEX observable-specific signal bias (OSB), and/or CNES real-time OSB files\n'
            line += 'snxDir        : snx               # The sub-directory of SINEX format IGS weekly solution files\n'
            line += 'ionDir        : ion               # The sub-directory of CODE/IGS global ionosphere map (GIM) files\n'
            line += 'ztdDir        : ztd               # The sub-directory of CODE/IGS tropospheric product files\n'
            line += 'tblDir        : tbl               # The sub-directory of table files (i.e., ANTEX, ocean tide loading files, etc.) for processing\n'
            line += 'logDir        : log               # The sub-directory of log file\n'
            line += '3partyDir     : ' + thirdpartydir + '  # The sub-directory where third-party softwares (i.e., \'wget\', \'gzip\', \'crx2rnx\' etc) are stored\n'

            if '-time' in sys.argv:
                index = sys.argv.index('-time')
                if sys.argv[index + 1][0] == '-' or sys.argv[index + 2][0] == '-' or sys.argv[index + 3][0] == '-':
                    print('*** ERROR: Three items are needed for time setting! Please check it.\n')
                    return 0
                else:
                    yyyy = sys.argv[index + 1]
                    doy = sys.argv[index + 2]
                    ndays = sys.argv[index + 3]
                    ctime = yyyy + '  ' + doy + '  ' + ndays

                    line += '\n'
                    line += '# Time settings ----------------------------------------------------------------\n'
                    line += 'procTime      : 2  ' + ctime + '    # The setting of start time for processing, which should be set to \'1 year month day ndays\' or \'2 year doy ndays\' or \'3 week dow ndays\'. NOTE: doy = day of year; week = GPS week; dow = day within week\n'

                    line += '\n'
                    # Settings of FTP downloading --------------------------------------------------
                    line += 'minusAdd1day  : 0                 # (0: off  1: on) The setting of the day before and after the current day for precise satellite orbit and clock products downloading\n'
                    line += 'merge_sp3files: 0                 # (0: off  1: on) to merge three consecutive sp3 files into one file\n'
                    line += 'printInfoWget : 1                 # (0: off  1: on) Printing the information generated by \'wget\'\n'

            if '-ftp' in sys.argv:
                index = sys.argv.index('-ftp')
                if sys.argv[index + 1][0] == '-':
                    print('*** ERROR: The FTP archieve is NOT given! Please check it.\n')
                    return 0
                else:
                    line += '\n'
                    line += '# Handling of FTP downloading --------------------------------------------------\n'
                    line += 'ftpDownloading:                   # The setting of the master switch for data downloading\n'
                    line += '  opt4ftp: 1                      #   1st: (0:off  1:on);\n'
                    line += '  ftpArch: ' + sys.argv[index + 1] + '                    #   2nd: the FTP archive, i.e., cddis, ign, or whu.\n'

            if '-obs' in sys.argv:
                index = sys.argv.index('-obs')
                if sys.argv[index + 1][0] == '-' or sys.argv[index + 2][0] == '-' or sys.argv[index + 3][0] == '-':
                    print('*** ERROR: Three items are needed for observation downloading! Please check it.\n')
                    return 0
                else:
                    line += 'getObs:                           # GNSS observation data downloading option\n'
                    line += '  opt4obs: 1                      #   1st(opt4obs): (0: off  1: on);\n'
                    line += '  obsType: ' + sys.argv[index + 1] + '                  #   2nd(obsType): \'daily\', \'hourly\', \'highrate\', \'30s\', \'5s\', or \'1s\';\n'
                    line += '  obsFrom: ' + sys.argv[index + 2] + '                   #   3rd: \'igs\', \'mgex\', \'igm\', \'cut\', \'hk\', \'ngs\', \'epn\', \'pbo\', or \'chi\';\n'
                    line += '  obsList: ' + sys.argv[index + 3] + '\n'
                    line += '  sHH4obs: 01                     #   5th: Start hour (00, 01, 02, ...);\n'
                    line += '  nHH4obs: 1                      #   6th: The consecutive hours, i.e., \'01  3\' denotes 01, 02, and 03;\n'
                    line += '  l2s4obs: 1                      #   7th: Valid only for the observation files with long name, 0: long name, 1: short name, 2: long and short name\n'
            else:
                line += 'getObs:                           # GNSS observation data downloading option\n'
                line += '  opt4obs: 0                      #   1st(opt4obs): (0: off  1: on);\n'
                line += '  obsType: daily                  #   2nd(obsType): \'daily\', \'hourly\', \'highrate\', \'30s\', \'5s\', or \'1s\';\n'
                line += '  obsFrom: mgex                   #   3rd: \'igs\', \'mgex\', \'igm\', \'cut\', \'hk\', \'ngs\', \'epn\', \'pbo\', or \'chi\';\n'
                line += '  obsList: site_mgex.list\n'
                line += '  sHH4obs: 01                     #   5th: Start hour (00, 01, 02, ...);\n'
                line += '  nHH4obs: 1                      #   6th: The consecutive hours, i.e., \'01  3\' denotes 01, 02, and 03;\n'
                line += '  l2s4obs: 1                      #   7th: Valid only for the observation files with long name, 0: long name, 1: short name, 2: long and short name\n'

            if '-nav' in sys.argv:
                index = sys.argv.index('-nav')
                if sys.argv[index + 1][0] == '-' or sys.argv[index + 2][0] == '-' or sys.argv[index + 3][0] == '-':
                    print('*** ERROR: Three items are needed for broadcast ephemeris downloading! Please check it.\n')
                    return 0
                else:
                    line += 'getNav:                           # Various broadcast ephemeris downloading option\n'
                    line += '  opt4nav: 1                      #   1st: (0: off  1: on);\n'
                    line += '  navType: ' + sys.argv[index + 1] + '                  #   2nd: \'daily\' or \'hourly\';\n'
                    line += '  navSys : ' + sys.argv[index + 2] + '                  #   3rd: \'gps\', \'glo\', \'bds\', \'gal\', \'qzs\', \'irn\', \'mixed3\', \'mixed4\', or \'all\';\n'
                    line += '  navFrom: ' + sys.argv[index + 3] + '                  #   4th: Analysis center (i.e., \'igs\', \'dlr\', \'ign\', \'gop\', or \'wrd\') that carries out the combination of broadcast ephemeris for mixed navigation data. From CDDIS or WHU FTP, \'igs\' and \'dlr\' can be downloaded, and from IGN, \'igs\' and \'ign\' can be downloaded. The downloading of \'gop\' and/or \'wrd\' is via the other FTP addresses;\n'
                    line += '  navList: site_mgex.list         #   5th: ONLY the site list file name is valid if the 2nd item is \'hourly\';\n'
                    line += '  sHH4nav: 01                     #   6th: Start hour (00, 01, 02, ...);\n'
                    line += '  nHH4nav: 2                      #   7th: The consecutive hours, i.e., \'01  3\' denotes 01, 02, and 03;\n'
                    line += '  l2s4nav: 2                      #   8th: Valid only for the navigation files with long name, 0: long name, 1: short name, 2: long and short name\n'
            else:
                line += 'getNav:                           # Various broadcast ephemeris downloading option\n'
                line += '  opt4nav: 0                      #   1st: (0: off  1: on);\n'
                line += '  navType: daily                  #   2nd: \'daily\' or \'hourly\';\n'
                line += '  navSys : mixed3                 #   3rd: \'gps\', \'glo\', \'bds\', \'gal\', \'qzs\', \'irn\', \'mixed3\', \'mixed4\', or \'all\';\n'
                line += '  navFrom: igs                    #   4th: Analysis center (i.e., \'igs\', \'dlr\', \'ign\', \'gop\', or \'wrd\') that carries out the combination of broadcast ephemeris for mixed navigation data. From CDDIS or WHU FTP, \'igs\' and \'dlr\' can be downloaded, and from IGN, \'igs\' and \'ign\' can be downloaded. The downloading of \'gop\' and/or \'wrd\' is via the other FTP addresses;\n'
                line += '  navList: site_mgex.list         #   5th: ONLY the site list file name is valid if the 2nd item is \'hourly\';\n'
                line += '  sHH4nav: 01                     #   6th: Start hour (00, 01, 02, ...);\n'
                line += '  nHH4nav: 2                      #   7th: The consecutive hours, i.e., \'01  3\' denotes 01, 02, and 03;\n'
                line += '  l2s4nav: 2                      #   8th: Valid only for the navigation files with long name, 0: long name, 1: short name, 2: long and short name\n'

            if '-orbclk' in sys.argv:
                index = sys.argv.index('-orbclk')
                if sys.argv[index + 1][0] == '-':
                    print('*** ERROR: One item are needed for final/rapid/ultra-rapid precise and clock downloading! Please check it.\n')
                    return 0
                else:
                    line += 'getOrbClk:                        # Satellite final/rapid/ultra-rapid precise orbit and clock downloading option\n'
                    line += '  opt4oc : 1                      #   1st: (0: off  1: on);\n'
                    line += '  ocFrom : ' + sys.argv[index + 1] + '                    #   2nd: Analysis center\n'
                    line += '  sHH4oc : 01                     #   6th: Start hour (00, 01, 02, ...);\n'
                    line += '  nHH4oc : 2                      #   7th: The consecutive hours, i.e., \'01  3\' denotes 01, 02, and 03;\n'
                    line += '  l2s4oc : 2                      #   8th: Valid only for the precise orbit and clock files with long name, 0: long name, 1: short name, 2: long and short name\n'
            else:
                line += 'getOrbClk:                        # Satellite final/rapid/ultra-rapid precise orbit and clock downloading option\n'
                line += '  opt4oc : 0                      #   1st: (0: off  1: on);\n'
                line += '  ocFrom : igs                    #   2nd: Analysis center\n'
                line += '  sHH4oc : 01                     #   6th: Start hour (00, 01, 02, ...);\n'
                line += '  nHH4oc : 2                      #   7th: The consecutive hours, i.e., \'01  3\' denotes 01, 02, and 03;\n'
                line += '  l2s4oc : 2                      #   8th: Valid only for the precise orbit and clock files with long name, 0: long name, 1: short name, 2: long and short name\n'

            if '-eop' in sys.argv:
                index = sys.argv.index('-eop')
                if sys.argv[index + 1][0] == '-':
                    print('*** ERROR: Three items are needed for final/ultra-rapid earth rotation/orientation parameter (ERP/EOP) downloading! Please check it.\n')
                    return 0
                else:
                    line += 'getEop:                           # Earth rotation/orientation parameter (ERP/EOP) downloading option\n'
                    line += '  opt4eop: 1                      #   1st: (0: off  1: on);\n'
                    line += '  eopFrom: ' + sys.argv[index + 1] + '                    #   2nd: Analysis center\n'
                    line += '  sHH4eop: 01                     #   6th: Start hour (00, 01, 02, ...);\n'
                    line += '  nHH4eop: 2                      #   7th: The consecutive hours, i.e., \'01  3\' denotes 01, 02, and 03;\n'
                    line += '  l2s4eop: 2                      #   8th: Valid only for the EOP files with long name, 0: long name, 1: short name, 2: long and short name\n'
            else:
                line += 'getEop:                           # Earth rotation/orientation parameter (ERP/EOP) downloading option\n'
                line += '  opt4eop: 0                      #   1st: (0: off  1: on);\n'
                line += '  eopFrom: igs                    #   2nd: Analysis center\n'
                line += '  sHH4eop: 01                     #   6th: Start hour (00, 01, 02, ...);\n'
                line += '  nHH4eop: 2                      #   7th: The consecutive hours, i.e., \'01  3\' denotes 01, 02, and 03;\n'
                line += '  l2s4eop: 2                      #   8th: Valid only for the EOP files with long name, 0: long name, 1: short name, 2: long and short name\n'

            if '-obx' in sys.argv:
                index = sys.argv.index('-obx')
                if sys.argv[index + 1][0] == '-':
                    print('*** ERROR: One item is needed for final/rapid/real-time ORBEX (ORBit EXchange format) downloading! Please check it.\n')
                    return 0
                else:
                    line += 'getObx:                           # ORBEX (ORBit EXchange format) for satellite attitude information downloading option\n'
                    line += '  opt4obx: 1                      #   1st: (0: off  1: on);\n'
                    line += '  obxFrom: ' + sys.argv[index + 1] + '                    #   2nd: Analysis center\n'
            else:
                line += 'getObx:                           # ORBEX (ORBit EXchange format) for satellite attitude information downloading option\n'
                line += '  opt4obx: 0                      #   1st: (0: off  1: on);\n'
                line += '  obxFrom: cod                    #   2nd: Analysis center\n'

            if '-dsb' in sys.argv:
                index = sys.argv.index('-dsb')
                if sys.argv[index + 1][0] == '-':
                    print('*** ERROR: One item is needed for differential code/signal bias (DCB/DSB) downloading! Please check it.\n')
                    return 0
                else:
                    line += 'getDsb:                           # Differential code/signal bias (DCB/DSB) downloading option\n'
                    line += '  opt4dsb: 1                      #   1st: (0: off  1: on);\n'
                    line += '  dsbFrom: ' + sys.argv[index + 1] + '                    #   2nd: Analysis center\n'
            else:
                line += 'getDsb:                           # Differential code/signal bias (DCB/DSB) downloading option\n'
                line += '  opt4dsb: 0                      #   1st: (0: off  1: on);\n'
                line += '  dsbFrom: cod                    #   2nd: Analysis center\n'

            if '-osb' in sys.argv:
                index = sys.argv.index('-osb')
                if sys.argv[index + 1][0] == '-':
                    print('*** ERROR: One item is needed for bservable-specific signal bias (OSB) downloading! Please check it.\n')
                    return 0
                else:
                    line += 'getOsb:                           # Observable-specific signal bias (OSB) downloading option\n'
                    line += '  opt4osb: 1                      #   1st: (0: off  1: on);\n'
                    line += '  osbFrom: ' + sys.argv[index + 1] + '                    #   2nd: Analysis center\n'
            else:
                line += 'getOsb:                           # Observable-specific signal bias (OSB) downloading option\n'
                line += '  opt4osb: 0                      #   1st: (0: off  1: on);\n'
                line += '  osbFrom: cod                    #   2nd: Analysis center\n'

            if '-snx' in sys.argv:
                line += 'getSnx:                           # IGS weekly SINEX downloading option\n'
                line += '  opt4snx: 1                      #   1st: (0: off  1: on)\n'
                line += '  l2s4snx: 1                      #   2nd: Valid only for IGS weekly SINEX file with long name, 0: long name, 1: short name, 2: long and short name\n'
            else:
                line += 'getSnx:                           # IGS weekly SINEX downloading option\n'
                line += '  opt4snx: 0                      #   1st: (0: off  1: on)\n'
                line += '  l2s4snx: 1                      #   2nd: Valid only for IGS weekly SINEX file with long name, 0: long name, 1: short name, 2: long and short name\n'

            if '-ion' in sys.argv:
                index = sys.argv.index('-ion')
                if sys.argv[index + 1][0] == '-':
                    print('*** ERROR: One item is needed for global ionosphere map (GIM) downloading! Please check it.\n')
                    return 0
                else:
                    line += 'getIon:                           # Global ionosphere map (GIM) downloading option\n'
                    line += '  opt4ion: 1                      #   1st: (0: off  1: on);\n'
                    line += '  ionFrom: ' + sys.argv[index + 1] + '                    #   2nd: Analysis center\n'
                    line += '  l2s4ion: 1                      #   2nd: Valid only for GIM file with long name, 0: long name, 1: short name, 2: long and short name\n'
            else:
                line += 'getIon:                           # Global ionosphere map (GIM) downloading option\n'
                line += '  opt4ion: 0                      #   1st: (0: off  1: on);\n'
                line += '  ionFrom: igs                    #   2nd: Analysis center\n'
                line += '  l2s4ion: 1                      #   2nd: Valid only for GIM file with long name, 0: long name, 1: short name, 2: long and short name\n'

            if '-roti' in sys.argv:
                line += 'getRoti:                          # Rate of TEC index (ROTI) downloading option\n'
                line += '  opt4rot: 1                      #   1st: (0: off  1: on)\n'
            else:
                line += 'getRoti:                          # Rate of TEC index (ROTI) downloading option\n'
                line += '  opt4rot: 0                      #   1st: (0: off  1: on)\n'

            if '-trop' in sys.argv:
                index = sys.argv.index('-trop')
                if sys.argv[index + 1][0] == '-' or sys.argv[index + 2][0] == '-':
                    print('*** ERROR: Two items are needed for tropospheric products downloading! Please check it.\n')
                    return 0
                else:
                    line += 'getTrp:                           # CODE/IGS tropospheric product downloading option\n'
                    line += '  opt4trp: 1                      #   1st: (0:off  1:on);\n'
                    line += '  trpFrom: ' + sys.argv[index + 1] + '                    #   2nd: Analysis center (i.e., \'igs\' or \'cod\');\n'
                    line += '  trpList: ' + sys.argv[index + 1] + '          #   3rd: the file name of site.list. NOTE: It is valid ONLY when the 2nd item \'igs\' is set.\n'
                    line += '  l2s4trp: 1                      #   4th: Valid only for the troposphere files with long name, 0: long name, 1: short name, 2: long and short name\n'
            else:
                line += 'getTrp:                           # CODE/IGS tropospheric product downloading option\n'
                line += '  opt4trp: 0                        #   1st: (0:off  1:on);\n'
                line += '  trpFrom: cod                    #   2nd: Analysis center (i.e., \'igs\' or \'cod\');\n'
                line += '  trpList: site_trp.list          #   3rd: the file name of site.list. NOTE: It is valid ONLY when the 2nd item \'igs\' is set.\n'
                line += '  l2s4trp: 1                      #   4th: Valid only for the troposphere files with long name, 0: long name, 1: short name, 2: long and short name\n'

            if '-atx' in sys.argv:
                line += 'getAtx:                           # ANTEX format antenna phase center correction downloading option\n'
                line += '  opt4atx: 1                      #   1st: (0: off  1: on)\n'
            else:
                line += 'getAtx:                           # ANTEX format antenna phase center correction downloading option\n'
                line += '  opt4atx: 0                      #   1st: (0: off  1: on)\n'

            f_w.write(line)

    else:
        return 0

    cmd = GOOD_bin + ' ' + cfgfile
    os.system(cmd)
    os.remove(cfgfile)

################################################################################
# Main program
################################################################################
if __name__ == '__main__':
    main_run_GOOD()