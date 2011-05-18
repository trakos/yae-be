#ifndef _FILE_LOGGER_H_
#define _FILE_LOGGER_H_

#include <string>
#include <fstream>
#include <iostream>
#ifdef __unix__
	#include <sys/stat.h>
#endif

#include "LoggerMisc.h"


class FileLogger
{
   public:
	FileLogger( std::string file, std::string dir = "/var/log" );
	FileLogger(){};
	~FileLogger();
	bool setfile( std::string file, std::string dir="/var/log" );
	void operator() ( std::string msg );
	void close();
   protected:
	std::fstream logfile;
};

#endif //_FILE_LOGGER_H_
