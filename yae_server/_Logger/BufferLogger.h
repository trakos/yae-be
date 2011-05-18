#ifndef _BUFFER_LOGGER_H_
#define _BUFFER_LOGGER_H_

#include <string>

#include "LoggerMisc.h"

#define BUFFER_LOGGER_BUFFER_SIZE 20

class BufferLogger
{
   public:
	BufferLogger ( ): current(0), lastGet(0) {};
	void operator() ( std::string msg );
	void operator() ( std::wstring msg );
	BufferLogger& operator>> ( std::string &str );
	BufferLogger& operator>> ( std::wstring &str );
   protected:
	std::string buffer[BUFFER_LOGGER_BUFFER_SIZE];
	// current to miejsce gdzie jest najnowszy msg
	// lastGet to miejsce gdzie będzie następne pobranie wiadomości
	// lastGet równe 20 oznacza, że current przekroczył lastGet o "okrążenie" całe
	int current;
	int lastGet;
};

#endif //_BUFFER_LOGGER_H_
