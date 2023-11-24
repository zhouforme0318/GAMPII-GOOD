/*------------------------------------------------------------------------------
 * GAMP II: intelliGent Analysis system for Multi-sensor integrated navigation and Positioning v2.0
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
 *----------------------------------------------------------------------------*/
#include "common/logger.h"
#include "core/config.h"


/* main function -------------------------------------------------------------*/
int main(int argc, char * argv[])
{
    std::string cfgfile;  /* configuration file */

// #if (defined(_WIN32) && defined(_DEBUG))
//     strcpy(cfgFile, "D:\\Projects\\proj_VScode\\GOOD\\dataset\\GOOD_cfg.yam");
// #else
//     if (argc != 2)
//     {
//         std::cout << std::endl;
//         std::cout << "* Usage: run_GOOD GOOD_cfg.txt" << std::endl;
//         std::cout << "         run_GOOD GOOD_cfg.yaml" << std::endl;
//         std::cout << std::endl;

//         return -1;
//     }
//     else strcpy(cfgFile, argv[1]);
// #endif
    if (argc != 2)
    {
        std::cout << std::endl;
        Logger::Trace(TEXPORT, "* Usage: run_GOOD GOOD_cfg.yaml");
        std::cout << std::endl;

        return -1;
    }
    else cfgfile = argv[1];

    Logger::Trace(TEXPORT, "GAMP II: intelliGent Analysis system for Multi-sensor integrated navigation and Positioning v2.0");
    Logger::Trace(TEXPORT, "===================================================================================================");
    Logger::Trace(TEXPORT, "* Now, we are running GAMP II - GOOD (GNSS Observations and prOducts Downloader)\n");

    Config::run(cfgfile);

#if (defined(_WIN32) && defined(_DEBUG))  /* for Windows */
    std::cout << std::endl << "Press any key to exit!" << std::endl;
    getchar();
#endif

    return 0;
}