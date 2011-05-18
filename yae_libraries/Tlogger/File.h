#ifndef _TLOGGER_FILE_H
#define _TLOGGER_FILE_H

#include <string>
#include <fstream>
#include <iostream>
#ifdef __unix__
	#include <sys/stat.h>
#endif

#include <utils.h>


class Tlogger_File
{
   public:
	Tlogger_File( std::string file, std::string dir = "/var/log" );
	Tlogger_File(){};
	~Tlogger_File();
	bool setfile( std::string file, std::string dir="/var/log" );
	void operator() ( std::string msg );
	void close();
   protected:
	std::fstream logfile;
};

#endif //_TLOGGER_FILE_H
