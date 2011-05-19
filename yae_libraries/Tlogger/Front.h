#ifndef _TLOGGER_FRONT_H
#define _TLOGGER_FRONT_H
#include <string>
#include <iomanip>
#include <cstdio>

#include <Tlogger/File.h>
#include <Tlogger/Buffer.h>
#include <utils.h>

enum Tlogger_Front_Priority { LOG_DISABLE, LOG_CRITICAL, LOG_ERROR, LOG_WARNING, LOG_NOTICE, LOG_DEBUG, LOG_SUPERDEBUG };

class Tlogger_Front
{
   public:
	//Tlogger_Front( std::string file, std::string dir="/var/log" ): warnings(0), errors(0), criticals(0), bufflog(), filelog( file, dir ), logging(WARNING), buffering(DEBUG), couting(DISABLE) {};
	void operator() ( std::string msg, Tlogger_Front_Priority prior);
	void operator() ( std::wstring msg, Tlogger_Front_Priority prior);
	bool setfile ( std::string file, std::string dir="/var/log" ) { return filelog.setfile( file, dir ); };
	Tlogger_Front& operator>> ( std::string &str );
	static Tlogger_Front& getInstance();
	static Tlogger_Front& getInstance(std::string filename, unsigned int line, std::string function);
	Tlogger_Front_Priority buffering;
	Tlogger_Front_Priority logging;
	Tlogger_Front_Priority couting;
   protected:
	static Tlogger_Front instance;
	Tlogger_Front(): warnings(0), errors(0), criticals(0), bufflog(), filelog(), logging(LOG_WARNING), buffering(LOG_DEBUG), couting(LOG_ERROR) {};
	int warnings;
	int errors;
	int criticals;
	Tlogger_Buffer bufflog;
	Tlogger_File filelog;
	std::string instanceInvokeFilename;
	unsigned long instanceInvokeLine;
	std::string instanceInvokeFunction;
};

#define LOG Tlogger_Front::getInstance(__FILE__, __LINE__, __PRETTY_FUNCTION__)
#define LDIS LOG_DISABLE
#define LCRIT LOG_CRITICAL
#define LERR LOG_ERROR
#define LWARN LOG_WARNING
#define LNOTE LOG_NOTICE
#define LDBG LOG_DEBUG
#define LSDBG LOG_SUPERDEBUG

#endif //_TLOGGER_FRONT_H
