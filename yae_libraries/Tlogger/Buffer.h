#ifndef _TLOGGER_BUFFER_H
#define _TLOGGER_BUFFER_H

#include <string>

#include <utils.h>

#define BUFFER_LOGGER_BUFFER_SIZE 20

class Tlogger_Buffer
{
   public:
	Tlogger_Buffer ( ): current(0), lastGet(0) {};
	void operator() ( std::string msg );
	void operator() ( std::wstring msg );
	Tlogger_Buffer& operator>> ( std::string &str );
	Tlogger_Buffer& operator>> ( std::wstring &str );
   protected:
	std::string buffer[BUFFER_LOGGER_BUFFER_SIZE];
	// current to miejsce gdzie jest najnowszy msg
	// lastGet to miejsce gdzie będzie następne pobranie wiadomości
	// lastGet równe 20 oznacza, że current przekroczył lastGet o "okrążenie" całe
	int current;
	int lastGet;
};

#endif //_TLOGGER_BUFFER_H
