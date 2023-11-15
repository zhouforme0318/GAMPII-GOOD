/*------------------------------------------------------------------------------
* config.h : header file of config.cpp
*-----------------------------------------------------------------------------*/
#pragma once

class Config
{
private:

    /**
    * @brief   : init - some initializations before processing
    * @param[O]: popt (processing options)
    * @param[O]: fopt (FTP options, nullptr:NO output)
    * @return  : none
    * @note    :
    **/
    static void init(prcopt_t* popt, ftpopt_t* fopt);

    /**
    * @brief   : ReadCfgYaml - read configure file with with YAML format to get processing options
    * @param[I]: cfgfile (configure file)
    * @param[O]: popt (processing options)
    * @param[O]: fopt (FTP options, nullptr:NO output)
    * @return  : true:ok, false:error
    * @note    :
    **/
    static bool ReadCfgYaml(std::string cfgfile, prcopt_t *popt, ftpopt_t *fopt);

public:
    Config() = default;
    ~Config() = default;

    /**
    * @brief   : run - start GOOD processing
    * @param[I]: cfgfile (configure file with full path)
    * @param[O]: none
    * @return  : none
    * @note    :
    **/
    static void run(std::string cfgfile);
};