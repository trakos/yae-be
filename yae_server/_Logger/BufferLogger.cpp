#include <algorithm>

#include "BufferLogger.h"

using namespace std;

void BufferLogger::operator()(std::wstring wmsg)
{
	string msg(wmsg.begin(), wmsg.end());
	(*this)(msg);
	return;
}

void BufferLogger::operator()(string msg)
{
	buffer[current] = msg;
	current++;
	if (current == BUFFER_LOGGER_BUFFER_SIZE)
	{
		current = 0;
	}
	// jezeli dogonilismy pobieranie z bufora to dodajemy do lastGet
	if (current == lastGet)
	{
		lastGet++;
		if (lastGet == BUFFER_LOGGER_BUFFER_SIZE)
		{
			lastGet = 0;
		}
	}
}
BufferLogger& BufferLogger::operator>>(wstring &wstr)
{
	string msg;
	(*this) >> msg;
	wstr.assign(wstr.begin(), wstr.end());
	return (*this);
}

BufferLogger& BufferLogger::operator>>(string &str)
{
	// jezeli jestesmy na biezaco
	if (current == lastGet)
	{
		str = "--end--";
	}
	// jezeli nie
	else
	{
		str = buffer[lastGet];
		lastGet++;
		if (lastGet == BUFFER_LOGGER_BUFFER_SIZE)
		{
			lastGet = 0;
		}
	}
	return (*this);
}
