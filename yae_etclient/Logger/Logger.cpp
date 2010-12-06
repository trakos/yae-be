
#include <cstdio>
#include <iostream>
#include <cstring>
#include <time.h>
#include <sstream>
#include <algorithm>

#include "Logger.h"


using namespace std;


Logger Logger::instance;

Logger& Logger::getInstance()
{
	return Logger::instance;
}

Logger& Logger::operator>> ( string &str )
{
	bufflog >> str;
	return (*this);
}

void Logger::operator() ( string classname, std::string functionName, wstring wmsg, logger_priority prior )
{
	string msg(wmsg.begin(), wmsg.end());
	(*this)(classname,functionName,msg,prior);
	return;
}

void Logger::operator() ( std::string classname, std::string functionName, std::string msg, logger_priority prior )
{
	if ( (int) logging < (int) prior && (int) buffering < (int) prior && (int) couting < (int) prior )
	{
		return;
	}
	// pobieramy czas
	time_t t;
	t=time(NULL);
	string datetime=ctime(&t);
	stringstream ss (stringstream::in | stringstream::out);
	// tworzymy wariant wg priorytetu
	string errortype;
	switch ( prior )
	{
		case LOG_CRITICAL: 	errortype="CRITICAL"; criticals++; break;
		case LOG_ERROR: 	errortype=" Error"; errors++; break;
		case LOG_WARNING:	errortype="  Warning"; warnings++; break;
		case LOG_NOTICE:	errortype="   notice"; break;
		case LOG_DEBUG:		errortype="    debug"; break;
		case LOG_SUPERDEBUG:errortype="     sdebug"; break;
		default: errortype=""; break;
	}
	// formatujemy log
 	ss << setw(20) << datetime << "  (" << setw(15) << left << classname << ")[" << setw(12) << left << errortype << "]: " << setw(15) << right << functionName << "(): " << msg << endl;
	// przerzucamy do stringa - po dacie jest koniec 1. linijki, a 2. to juz ostatnia i najwa¿niejsza linijka fragmentu nas interesujacego
	string tmp, fulltext="";
	getline(ss, tmp);
	fulltext+=tmp;
	getline(ss, tmp);
	fulltext+=tmp;
	//wypisujemy jezeli jest ku temu okazja
	if ( (int) logging >= (int) prior )
		filelog( fulltext );
	if ( (int) buffering >= (int) prior )
		bufflog( fulltext );
	if ( (int) couting >= (int) prior )
		cout << fulltext << endl;
	return;
}
