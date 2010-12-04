/*
 * ETClient/Listener.cpp
 *
 *  Created on: 2010-12-02
 *      Author: trakos
 */

#include <ETClient/Listener.h>
#include <ETClient/Console.h>
#include <string>

ETClientListener ETClientListener::instance = ETClientListener();

ETClientListener::ETClientListener(){}

ETClientListener& ETClientListener::getInstance()
{
	return ETClientListener::instance;
}

std::wstring ETClientListener::getCommand()
{
	std::wstring lastLine = ETClientConsole::getInstance().getLastLine();
	if (lastLine.empty())
	{
		return lastLine;
	}
	// find the special command - yae.
	if (lastLine == L"[skipnotify]unknown cmd /yae" || lastLine == L"unknown cmd /yae")
	{
		return L"yae";
	}
	// remove ]/ or ]\ from the start of the string - if applicable
	if (lastLine.find(L"]/", 0) != lastLine.npos || lastLine.find(L"]\\", 0) != lastLine.npos)
	{
		lastLine = lastLine.substr(2);
	}
	// if the line starts with %, it is our command - we are now about to send it further
	if (lastLine[0] == '%')
	{
		lastLine = lastLine.substr(1);
		return lastLine;
	}
	return L"";
	/*
	// remove [skipnotify] from the start of the string - if applicable
	std::wstring skipNotify = L"[skipnotify]";
	if(lastLine.find(skipNotify, 0) != lastLine.npos)
	{
		lastLine = lastLine.substr(skipNotify.length());
	}
	// remove "unknown cmd " from the start - if applicable
	std::wstring unkownCommand = L"unknown cmd ";
	if(lastLine.find(unkownCommand, 0) != lastLine.npos)
	{
		lastLine = lastLine.substr(unkownCommand.length());
	}
	//
	std::wcout << lastLine << std::endl;
	return L"";
	 */
}