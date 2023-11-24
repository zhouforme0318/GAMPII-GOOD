# GOOD, version 3.0
Global Navigation Satellite System (GNSS) data processing requires a user to download various observation and product files. These file downloads, which are particularly important for precise point positioning (PPP), can be a burden for new and experienced GNSS users. Up to now, there are few standalone open-source software tools available to accomplish this task. In order to address this need, an easy-to-use GNSS data and product downloading toolkit called GOOD (GNSS Observations and prOducts Downloader) was developed by <strong><font color=#ED1B24>UNI</font><font color=#27A9DD>Q</font></strong> navigation lab (**U**biquitous **N**avigation & **I**ntegrated positioning lab in **Q**uest) at Shandong University of Science and Technology (SDUST). GOOD is compatible with most popular operating systems, actively maintained on GitHub, and stable at the current version 3.0.
# Features
Main feature of GOOD are various GNSS observations and products downloading, i.e., 
- IGS daily, hourly, and high-rate observations with short file name (RINEX version 2.xx)
- MGEX daily, hourly, and high-rate observations with long file name (RINEX version 3.xx)
- IGS + MGEX (taking the union of IGS and MGEX, while the priority of MGEX sites is higher) daily, hourly, and high-rate observations
- Curtin University of Technology (CUT) daily observations with long file name (RINEX v3.xx)
- Hong Kong CORS 30s, 5s, and 1s observations with long file name (RINEX version v3.xx)
- NGS/NOAA CORS daily observations with long file name (RINEX v3.xx)
- EUREF Permanent Network (EPN) observations (long file name in RINEX v3.xx and short file name in RINEX v2.xx)
- Plate Boundary Observatory (PBO) observations (long file name in RINEX v3.xx)
- Various types of broadcast ephemeris, i.e., GPS- and GLONASS-only in RINEX v2.xx, mixed types in RINEX v3.xx and v4.xx for multiple-GNSS
- Various types of IGS, MGEX, and other analysis center (AC) final, rapid, and ultra-rapid precise orbit and clock products, CNES real-time orbit and clock products in offline file format
- Various types of IGS final and ultra-rapid earth rotation/orientation parameter (ERP/EOP)
- ORBEX (ORBit EXchange format) products from five institutions, i.e., CODE, GFZ, GRG, WHU, and CNES
- GPS and GLONASS differential code bias (DCB) products from CODE and multi-GNSS differential signal bias (DSB) products from CAS
- Code observable-specific signal bias (OSB) products from six institutions, i.e., CAS, CODE, GFZ, GRG, WHU, and CNES
- IGS weekly SINEX solutions
- Various types of final, rapid, hourly rapid, 15-min rapid, 1-day and 2-day predicted global ionosphere map (GIM) products
- Rate of TEC index (ROTI) products
- CODE and IGS final tropospheric products
- IGS ANTEX format antenna phase center corrections  
**But it is not limited to these, users can use the source code to expand its functionalities according to their needs.**
# 1 Compilation and Install
## 1.1 Compilation environment
GOOD is managed by CMake and can be compiled on Linux/MacOS/Windows.  
The configuration file in YAML format is used as a parameter to run GOOD after successfully compiling it. To debug the program, it is also required to add the configuration file as a parameter.
## 1.2 Dependency libraries
GOOD requires only one third-party library, which is yaml-cpp. We have added it in the **src/thirdparty** directory.
## 1.3 To compile under Linux
You should install the build-essential libraries following the commonds:
```shell
sudo apt-get install cmake
sudo apt-get install build-essential
```
Then, you can clone the repository locally and compile GOOD as follows:
```shell
# Clone the repository
git clone https://github.com/zhouforme0318/GAMPII-GOOD.git ~/

# Build GOOD
cd ~/GAMPII-GOOD
mkdir build && cd build

cmake ../ -DCMAKE_BUILD_TYPE=Release 
make -j3

# Run GOOD
cd ~/GAMPII-GOOD
./bin/run_GOOD ./dataset_Linux/GOOD_cfg.yaml
```
## 1.4 To compile under Windows

The MSVC (Microsoft Visual C/C++) compiler is generally used in Windows. We recommend you to compile GOOD with the VS Code software.

You should first install the MSVC compiler and VS code software (including the necessary plug-ins, such as C/C++, C/C++ Extension Pack, CMake, and CMake Tools).

After preparing your own compilation environment, you can clone the repository locally and open the GAMPII-GOOD folder in VS Code:

- Set compiler: open the Command Palette (Ctrl+Shift+P) and type "CMake: Select a Kit", select the MSVC compilier
- Set compile parameter: type "CMake: Select Variant" in the Command Palette, select "Release"
- Configure CMake: type "CMake: Configure" in the Command Palette
- Compile Project: type "CMake: Build" in the Command Palette

Open a PowerShell or CMD terminal in the project directory and run the test:
```shell
.\bin\Release\run_GOOD.exe .\dataset_Win\GOOD_cfg.yaml
```
## 1.5 How to debug with VS Code
The debug operation is as follows:
- Set compiler: open the Command Palette (Ctrl+Shift+P) and type "CMake: Select a Kit", select GCC for Linux or select the MSVC compilier for Windows
- Set compile parameter: type "CMake: Select Variant" in the Command Palette, select **Debug**
- Configure CMake: type "CMake: Configure" in the Command Palette, Compile Project: type "CMake: Build" in the Command Palette
- Set debug startup option: click the fourth button on the left side of VS Code to open the "Run and Debug" menu (or the shortcut key Ctrl+Shift+D), and select "Windows GOOD" or "Linux GOOD" at the top of this menu window.
- Start debugging, click the green triangle symbol at the top of the left window (or the shortcut key F5) to start debugging.
# Attention!
If some GNSS data cannot be successfully downloaded, check your network first. Then, check if the file exists on the remote server. Finally, rerun ‘run_GOOD  GOOD_cfg.yaml’ to try again. Don’t worry, once the data has been downloaded, it will not be downloaded repeatedly.
# Version
- Vers. 1.0  
  2021/04/16 new  
  2021/04/20 MGEX multi-GNSS precise products option added  
- Vers. 1.1  
  2021/04/23 IGN and WHU FPT archives added  
  2021/04/30 the day before and after the current day for precise satellite orbit and clock products downloading is added
- Vers. 1.2  
  2021/04/30 CNES real-time orbit, clock, bias files (in offline mode) option added  
  2021/05/01 from DOY 345/2020, 'brdcDDD0.YYn' was converted from '*.Z' to '*.gz'  
  2021/05/01 added the full path of third-party softwares
- Vers. 1.3  
  2021/05/05 add 'GetRoti' for rate of TEC index (ROTI) file downloading
- Vers. 1.4  
  2021/05/06 add the compatibility for changing directory (chdir) in Windows and Linux OS  
  2021/05/07 add the option 'printInfoWget' in configure file for (not) printing the information of running 'wget'  
  2021/05/08 add IGS and MGEX hourly observation (30s) downloading
- Vers. 1.5  
  2021/05/10 modify some codes to make GOOD adaptable to different compression form (i.e., '*.Z' or '*.gz')  
  2021/05/12 add the option for IGR, IGU, GFU (from GFZ), and WUU (from WHU) products as well as the site-specific broadcast ephemeris files downloading  
  2021/05/15 sub-directory (i.e., daily, hourly, and highrate) creation for observation downloading
- Vers. 1.6  
  2021/05/18 modify some codes to make configuration file and program look more concise  
  2021/05/21 add the extraction and conversion for the 'all' option in IGS and MGEX observation downloading
- Vers. 1.7  
  2021/06/01 add 'getObc', 'getObg', and 'getObh' options for Curtin University of Technology (CUT), Geoscience Australia (GA), and Hong Kong CORS observation downloading
- Vers. 1.8  
  2021/09/01 add various types of (i.e., ESA, GFZ, IGS, WHU) and rapid (i.e., CODE, NRCan, ESA, GFZ, IGS) orbit and clock products for downloading (by Feng Zhou @ SDUST and Sermet Ogutcu @ NEU)  
  2021/09/04 add the option for NGS/NOAA CORS (from Amazon Web Services (AWS)) and EUREF Permanent Network (EPN) observation downloading (by Feng Zhou @ SDUST)  
  2021/09/05 fix the bug of broadcast ephemeris downloading via WHU FTP before and after 2020 (by Feng Zhou @ SDUST and Zan Liu @ CUMT)  
  2021/09/06 modify the setting of directories of GNSS observations and products in configure file to make them more elastic (by Feng Zhou @ SDUST and Caoming Fan @ SDU)
- Vers. 1.9  
  Please refer to the header component of FtpUtil.cpp
- Vers. 2.0  
  Please refer to the header component of FtpUtil.cpp
- Vers. 2.1  
  Please refer to the header component of FtpUtil.cpp
- Vers. 2.2  
  Please refer to the header component of FtpUtil.cpp
- Vers. 3.0  
  Please refer to the header component of FtpUtil.cpp
# Acknowledgments
- Dr Cheng Wang @ BUAA provided the C++ code for observation downloading as reference
- the suggestion of adding the option of the day before and after the current day for precise satellite orbit and clock products downloading is provide by Zan Liu @ CUMT
- the suggestion of adding the option of the full path of third-party softwares is provide by Dr Yudan Yi
- the suggestion of adding the option of rate of TEC index (ROTI) is provide by Lei Liu @ WHU
- the suggestion of adding the option of 'printInfoWget' in configure file for (not) printing the information of running 'wget' by Dr Hong Hu @ AHU
- the suggestion of adding the option of IGR, IGU, GFU (from GFZ), and WUU (from WHU) products as well as the site-specific broadcast ephemeris is provide by Menghao Li @ HEU
- the suggestion of adding the option of various ultra-rapid (i.e., from ESA, GFZ, IGS, WHU) and rapid (i.e., from CODE, NRCan, ESA, GFZ, IGS) orbit and clock products downloading is provided by Sermet Ogutcu @ Necmettin Erbakan University
- the suggestion of modifying the setting of directories of GNSS observations and products in configure file is provided by Caoming Fan @ SDU
# License
The source code is released under GPLv3 license.

GOOD is constantly being updated and improved by <strong><font color=#ED1B24>UNI</font><font color=#27A9DD>Q</font></strong> navigation lab. For any technical issues/questions, please contact Feng Zhou (zhouforme@163.com) or open an issue at this repository.