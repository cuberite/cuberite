#include <iostream>
#include "cNBTData.h"
#include "cTimer.h"
#include "cQuicksort.h"
#include "cDeNotch.h"
#ifdef _WIN32
#include "wdirent.h"
#else
#include <dirent.h>
#endif

int main () {

	cTimer Timer;
	clock_t progBegin = clock(); //start main program timer
	std::string dir;
	DIR* dp;
	struct dirent *entry;
        int found;
	std::string entrys;
        std::string str2;
	std::string str3;
        std::string filexPos;
	std::string filezPos;
	std::string pak_name;
	//string* dir_array;
	int dir_num_files = 0;
	int ctr = 0;
#ifdef _WIN32
	if(dp = opendir("region\\")){
#else
	if(dp = opendir("region/")){
#endif
		while(entry = readdir(dp)){
			entrys = entry->d_name;
			found = entrys.find(".mcr");
			if ( (found!=std::string::npos) ) {
				str2 = entrys.substr (2,sizeof(entrys));
				filexPos = str2.substr (0,(int)str2.find("."));
				str3 = str2.substr ((int)str2.find(".")+1, sizeof(str2));
				filezPos = str3.substr (0,(int)str3.find("."));
				pak_name = "X" + filexPos + "_Z" + filezPos + ".pak";

				clock_t begin=clock(); //start execution timer

				cDeNotch DeNotch;
				DeNotch.Converter ( entrys, pak_name );


			        clock_t end=clock();
        			std::cout << "Time to convert chunk: " << double(Timer.diffclock(end,begin)) << " Seconds"<< std::endl;
              		}
              }
              closedir(dp);
        }
	clock_t progEnd = clock(); //end main program timer
	std::cout << "Time to complete converter: " << double(Timer.diffclock(progEnd,progBegin)) << " Seconds"<< std::endl;
        return 0;
};
