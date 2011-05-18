#ifndef _LOGGER_H_
#define _LOGGER_H_
#include <string>
#include <iomanip>
#include <cstdio>

#include "FileLogger.h"
#include "BufferLogger.h"
#include "LoggerMisc.h"

enum logger_priority { LOG_DISABLE, LOG_CRITICAL, LOG_ERROR, LOG_WARNING, LOG_NOTICE, LOG_DEBUG, LOG_SUPERDEBUG };

class Logger
{
   public:
	//Logger( std::string file, std::string dir="/var/log" ): warnings(0), errors(0), criticals(0), bufflog(), filelog( file, dir ), logging(WARNING), buffering(DEBUG), couting(DISABLE) {};
	void operator() ( std::string msg, logger_priority prior);
	void operator() ( std::wstring msg, logger_priority prior);
	bool setfile ( std::string file, std::string dir="/var/log" ) { return filelog.setfile( file, dir ); };
	Logger& operator>> ( std::string &str );
	static Logger& getInstance();
	static Logger& getInstance(std::string filename, unsigned int line, std::string function);
	logger_priority buffering;
	logger_priority logging;
	logger_priority couting;
   protected:
	static Logger instance;
	Logger(): warnings(0), errors(0), criticals(0), bufflog(), filelog(), logging(LOG_WARNING), buffering(LOG_DEBUG), couting(LOG_ERROR) {};
	int warnings;
	int errors;
	int criticals;
	BufferLogger bufflog;
	FileLogger filelog;
	std::string instanceInvokeFilename;
	unsigned long instanceInvokeLine;
	std::string instanceInvokeFunction;
};

#define LOG Logger::getInstance(__FILE__, __LINE__, __PRETTY_FUNCTION__)
#define LDIS LOG_DISABLE
#define LCRIT LOG_CRITICAL
#define LERR LOG_ERROR
#define LWARN LOG_WARNING
#define LNOTE LOG_NOTICE
#define LDBG LOG_DEBUG
#define LSDBG LOG_SUPERDEBUG

#endif //_LOGGER_H_
