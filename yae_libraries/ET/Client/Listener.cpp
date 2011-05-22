/*
 * ETClient/Listener.cpp
 *
 *  Created on: 2010-12-02
 *      Author: trakos
 */

#include <ET/Client/Listener.h>
#include <ET/Client/Console.h>
#include <utils.h>
#include <string>

ET_Client_Listener* ET_Client_Listener::instance = 0;

ET_Client_Listener::ET_Client_Listener(){}

ET_Client_Listener& ET_Client_Listener::getInstance()
{
	if (!ET_Client_Listener::instance)
	{
		ET_Client_Listener::instance = new ET_Client_Listener();
	}
	return *ET_Client_Listener::instance;;
}

std::wstring ET_Client_Listener::getCommandW()
{
	std::wstring lastLine = ET_Client_Console::getInstance().getLastLine();
	if (lastLine.empty())
	{
		return lastLine;
	}
	// find the special command - yae.
	if (lastLine == L"[skipnotify]unknown cmd /yae" || lastLine == L"[skipnotify]unknown cmd yae" || lastLine == L"unknown cmd /yae" || lastLine == L"unknown cmd yae" || lastLine == L"Unknown command \"/yae\"" || lastLine == L"Unknown command \"yae\"" || lastLine == L"[skipnotify]yae not allowed during intermission." || lastLine == L"[skipnotify]/yae not allowed during intermission." )
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

std::string ET_Client_Listener::getCommand()
{
	std::wstring command = this->getCommandW();
	return wtoa(command);
}
