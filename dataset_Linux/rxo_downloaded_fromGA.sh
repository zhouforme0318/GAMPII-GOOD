#!/bin/bash
##
## usage: ./rxo_downloaded_fromGA.sh /home/zhouforme/Documents/MySoftwares/proj_Cpp/UNIQ/GOOD/dataset_Linux 2023 1 2
##
datadir=$1  # data directory
year=$2     # 4-digit year
doy=$3      # day of year
ndays=$4    # number of consecutive days
yy=`echo ${year:2:2}`  # 2-digit year
USER=anonymous
PASSWORD=zhouforme@163.com
site_list=${datadir}/site_ga.list  # the full path of site.list
while [[ ${ndays} -gt 0 ]]; do  # day-by-day
    cdoy=`echo ${doy} | awk '{printf("%3.3d\n", $1)}'`
    obsdir=${datadir}/obs/${year}/${cdoy}/GA/daily  # the subdirectory for observations
    if [[ ! -d "${obsdir}" ]]; then
        mkdir -p ${obsdir}
    fi
    for site_old in `cat ${site_list}`; do  # site-by-site
        site_new=`echo ${site_old/2023001/${year}${cdoy}}`
        echo ${site_new}
        site=`echo ${site_new:0:4}`
        site=`echo ${site,,}`  # upper case to lower case
        ofile=${obsdir}/${site}${cdoy}0.${yy}o
        if [[ -f "${ofile}" ]]; then
            continue
        fi
        lftp -u ${USER},${PASSWORD} sftp://52.65.50.124<<EOF  # lftp needs to be installed in advance
            cd rinex/daily/${year}/${cdoy}
            get ${site_new}
            by
EOF
        gzip -d -f ${site_new}
        crxfile=`echo ${site_new%.*}`
        cat ${crxfile} | ${datadir}/thirdparty_Linux/crx2rnx -f - > ${ofile}  # crx to rxo
        rm -f ${crxfile}
    done
    let "doy+=1"
    let "ndays-=1"
done
