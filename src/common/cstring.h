/*------------------------------------------------------------------------------
* cstring.h : string manipulations
*
* Copyright (C) 2020-2099 by Ubiquitous Navigation & Integrated positioning lab in Quest (UNIQ), all rights reserved.
*    This file is part of GAMP II - GOOD (Gnss Observations and prOducts Downloader)
*
* history : 2020/08/16 1.0  new (by Feng Zhou)
*           2020/10/12 3.0  mojor modifications (by Feng Zhou)
*-----------------------------------------------------------------------------*/
#ifndef CSTRING_H
#define CSTRING_H

#include "common.h"
#include "logger.h"

class CString
{
public:
    /**
    * @brief   : int2str - to convert from integer value to string
    * @param[I]: ii (integer value)
    * @param[I]: len (length of string)
    * @param[O]: none
    * @return  : number in string format
    * @note    :
    **/
    static std::string int2str(int ii, int len)
    {
        std::string xii(std::to_string(ii));
        std::string sii;
        if (len == 2) sii = "00";
        else if (len == 3) sii = "000";
        else if (len == 4) sii = "0000";
        sii.replace(len - xii.size(), xii.size(), xii);

        return sii;
    } /* end of int2str */
    /**
    * @brief     : ToUpper - convert the string from lower case to upper case
    * @param[I/O]: str (string)
    * @return    : none
    * @note      :
    **/
    static void ToUpper(std::string& str)
    {
        for (size_t i = 0; i < str.size(); i++) str[i] = toupper(str[i]);
    } /* end of ToUpper */

    /**
    * @brief     : ToLower - convert the string from upper case to lower case
    * @param[I/O]: str (string)
    * @return    : none
    * @note      :
    **/
    static void ToLower(std::string& str)
    {
        for (size_t i = 0; i < str.size(); i++) str[i] = tolower(str[i]);
    } /* end of ToLower */

    /* split buff by sep */
    static std::vector<std::string> split(std::string buff, std::string sep)
    {
        std::vector<std::string> tokens;
        tokens.clear();
        size_t lastpos = buff.find_first_not_of(sep, 0);
        size_t pos = buff.find(sep, lastpos);

        while (lastpos != std::string::npos)
        {
            tokens.emplace_back(buff.substr(lastpos, pos - lastpos));
            lastpos = buff.find_first_not_of(sep, pos);
            pos = buff.find(sep, lastpos);
        }
        return tokens;
    }

    /**
    * @brief     : trim - trim head and tail space of the string
    * @param[I/O]: s (input string with head and tail space; output string without head and tail space)
    * @return    : none
    * @note      :
    **/
    static void trim(std::string& s)
    {
        if (s.empty()) return;

        s.erase(0, s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
    } /* end of trim */

    /**
    * @brief   : GetFile - get the full name of a specific file from the current directory
    * @param[I]: dir (the current directory)
    * @param[I]: str (std::string to match)
    * @param[O]: fileName (the full name of a specific file)
    * @return  : true: found, false: NOT found
    * @note    :
    **/
    static bool GetFile(std::string dir, std::string str, std::string& filename)
    {
        /* change directory */
#ifdef _WIN32   /* for Windows */
        _chdir(dir.c_str());
#else           /* for Linux or Mac */
        chdir(dir.c_str());
#endif

        std::string cmd, filelist = "files.list";
#ifdef _WIN32   /* for Windows */
        cmd = "dir /b | find \"" + str + "\"" + " > " + filelist;
#else           /* for Linux or Mac */
        cmd = "ls *" + str + "* > " + filelist;
#endif
        std::system(cmd.c_str());

        bool isfound = false;
        filename.clear();
        if (access(filelist.c_str(), 0) == 0)
        {
            std::ifstream fillst(filelist.c_str());
            if (!fillst.is_open())
            {
                Logger::Trace(TERROR, "*** ERROR(CString::GetFile): open files.list = " + filelist + " file failed, please check it");

                return false;
            }

            std::string fil;
            getline(fillst, fil);
            trim(fil);
            if (fil.size() > 0)
            {
                filename = fil;
                isfound = true;
            }
            else isfound = false;

            /* close 'files.list' */
            fillst.close();
        }

        /* delete 'files.list' */
#ifdef _WIN32   /* for Windows */
        cmd = "del " + filelist;
#else           /* for Linux or Mac */
        cmd = "rm -rf " + filelist;
#endif
        std::system(cmd.c_str());

        return isfound;
    } /* end of GetFile */

    /**
    * @brief   : GetFilesAll - get the name list of all the files from the current directory
    * @param[I]: dir (the current directory)
    * @param[I]: str (string to match)
    * @param[O]: files (the name list of all the files)
    * @return  : none
    * @note    :
    **/
    static void GetFilesAll(std::string dir, std::string str, std::vector<std::string>& files)
    {
        std::string cmd, filelist = "files.list";
#ifdef _WIN32   /* for Windows */
        cmd = "dir /b " + dir + " | find \"" + str + "\"" + " > " + filelist;
#else           /* for Linux or Mac */
        cmd = "ls " + dir + "/*" + str + "* > " + filelist;
#endif
        int stat = std::system(cmd.c_str());
        if (!stat) stat = stat;

        files.clear();
        if (access(filelist.c_str(), 0) == 0)
        {
            std::ifstream fillst(filelist.c_str());
            if (!fillst.is_open())
            {
                std::string out = "*** ERROR(CString::getFiles): open files.list = " + filelist + " file failed, please check it";
                std::cout << out << std::endl;
                return;
            }

            std::string filename;
            while (getline(fillst, filename))
            {
                trim(filename);
#ifdef _WIN32   /* for Windows */
                std::string sep;
                sep.push_back((char)FILEPATHSEP);
                if (filename.length() > str.length())
                {
                    filename = dir + sep + filename;
                    files.push_back(filename);
                }
#else           /* for Linux or Mac */
                files.push_back(filename);
#endif
            }

            /* close 'files.list' */
            fillst.close();
        }

        /* delete 'files.list' */
#ifdef _WIN32   /* for Windows */
        cmd = "del " + filelist;
#else           /* for Linux or Mac */
        cmd = "rm -rf " + filelist;
#endif
        stat = std::system(cmd.c_str());
        if (!stat) stat = stat;
    } /* end of GetFilesAll */
};

#endif  // CSTRING_H