
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

Logger& Logger::getInstance(std::string filename, unsigned int line, std::string function)
{
	Logger::instance.instanceInvokeLine = line;
	Logger::instance.instanceInvokeFilename = filename;
	Logger::instance.instanceInvokeFunction = function;
	return Logger::instance;
}

Logger& Logger::operator>> ( string &str )
{
	bufflog >> str;
	return (*this);
}

void Logger::operator() ( wstring wmsg, logger_priority prior )
{
	string msg(wmsg.begin(), wmsg.end());
	(*this)(msg,prior);
	return;
}

#ifdef debug
	#include <vector>
	vector<std::string> split(std::string string, std::string character)
	{
		vector<std::string> array;
		int found;
		while ( (found=string.find_first_of(character)) != string.npos)
		{
			if(found!=0)
			{
				array.push_back(string.substr(0, found));
			}
			string = string.substr(found+1);
		}
		if (string.length() > 0)
		{
			array.push_back(string);
		}
		return array;
	}
#endif

void Logger::operator() ( std::string msg, logger_priority prior )
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
#ifdef debug
	string classname="",function;
	vector<string> splitted = split(this->instanceInvokeFunction,"(");
	function = splitted.front();
	splitted = split(function," ");
	function = splitted.back();
	splitted = split(function,":");
	if(splitted.size()>1)
	{
		classname = splitted[0];
		function = splitted[1];
	}
	/*if( splitted.size() != 2)
	{
		classname = "";
		function = splitted[0];
		splitted = split(function," ");
		function = splitted[0];
	}
	else
	{
		classname = splitted[0];
		function = splitted[1];
		splitted = split(classname," ");
		classname = splitted[1];
	}
	splitted = split(function,"(");
	function = splitted[0];*/
	ss << setw(20) << this->instanceInvokeFilename << ":" << setw(3) << left << this->instanceInvokeLine << "  " << setw(15) << right << classname << "::" << left << setw(15) << function << " [" << setw(12) << left << errortype << "]: " << msg << endl;
#else
 	ss << setw(20) << datetime << " " << setw(20) << this->instanceInvokeFilename << ":" << setw(3) << left << this->instanceInvokeLine << "  " << setw(50) << right << this->instanceInvokeFunction << " [" << setw(12) << left << errortype << "]: " << msg << endl;
#endif
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
