/*
 * ETClient/Console.cpp
 *
 *  Created on: 2010-11-26
 *      Author: trakos
 */

#include <Windows.h>
#include <ETClient/Console.h>
#include <Logger/Logger.h>

ETClientConsole ETClientConsole::instance = ETClientConsole();

void ETClientConsole::toggleWindow(bool show)
{
	LOG(show?"true":"false",LNOTE);
	this->showWindow = show;
	if(this->isConsoleAttached())
	{
		ShowWindow(this->hConWin,show?SW_SHOW:SW_HIDE);
	}
	return;
}

bool ETClientConsole::attachConsole()
{
	this->hConWin = FindWindowEx(NULL, NULL, NULL, "ET Console");
	if(this->hConWin == NULL)
	{
		this->hWriteCmd = NULL;
		this->hReadCmd = NULL;
		this->processId = 0;
		LOG("not found",LSDBG);
		return false;
	}
	ShowWindow(this->hConWin,this->showWindow?SW_SHOW:SW_HIDE);
	this->hWriteCmd = FindWindowEx(this->hConWin, NULL, "Edit", NULL);
	this->hReadCmd = FindWindowEx(this->hConWin, this->hWriteCmd, "Edit", NULL);
	if( this->hWriteCmd == NULL || this->hReadCmd == NULL )
	{
		this->processId = 0;
		LOG("found window, but no write or read cmd!",LERR);
		return false;
	}
	this->processId = GetWindowThreadProcessId( this->hConWin, NULL );
	LOG(" done, all ok",LDBG);
	return true;
}

ETClientConsole::ETClientConsole(): charConsolePointer(0),processId(0)
{
	LOG("",LSDBG);
}

bool ETClientConsole::sendMessage(std::wstring text)
{
	if (!this->isConsoleAttached())
	{
		LOG("console not attached...",LNOTE);
		return false;
	}
	// save what's written now
	int length = (int)SendMessageW(this->hWriteCmd, WM_GETTEXTLENGTH, (WPARAM)NULL, (LPARAM)NULL);
	wchar_t buffer[length+1];
	SendMessageW(this->hWriteCmd, WM_GETTEXT , (WPARAM)(length+1), (LPARAM)buffer);
	// send what we need to send
	bool success = SendMessageW( this->hWriteCmd, WM_SETTEXT, (WPARAM)NULL, (LPARAM)text.c_str());
	if(!success)
	{
		LOG("found window, but no write or read boxes!",LERR);
	}
	SendMessageW( this->hWriteCmd, WM_CHAR, 13, 1);
	// restore what was written before
	SendMessageW( this->hWriteCmd, WM_SETTEXT, (WPARAM)NULL, (LPARAM)buffer);
	LOG(L"sent: '"+text+L"'",LNOTE);
	return success;
}

/**
 * Method returns only one line, without the trailing \n
 */
std::wstring ETClientConsole::getLastLine()
{
	const wchar_t* charPointer = L"";
	if (!this->isConsoleAttached())
	{
		LOG("console not attached...",LNOTE);
		return (std::wstring)charPointer;
	}
	int length = (int)SendMessageW(this->hReadCmd, WM_GETTEXTLENGTH, (WPARAM)NULL, (LPARAM)NULL);
	wchar_t buffer[length+1];
	if(length != this->charConsolePointer)
	{
		if(length < this->charConsolePointer)
		{
			this->charConsolePointer=0;
			this->attachConsole();
		}
		buffer[length] = '\0';
		SendMessageW(this->hReadCmd, WM_GETTEXT , (WPARAM)(length+1), (LPARAM)buffer);
		// get the start of the part that is new to us
		wchar_t* ptr = buffer + this->charConsolePointer;
		charPointer = ptr;
		// find how much characters are there to the first end of line
		int lineLength = 0;
		while(length>lineLength && *ptr != L'\n')
		{
			ptr++;
			lineLength++;
		}
		// substitute \n\n to \0\0 and increment lineLength to prevent stopping on the very same \n in next call
		*(ptr--) = L'\0';
		*ptr = L'\0';
		lineLength++;
		// save the result (remember that by adding '\0' ptr we truncated the charPointer wstring to contain only one line)
		this->charConsolePointer+=lineLength;
	}
	return (std::wstring)charPointer;
}

void ETClientConsole::moveToTheEnd()
{
	LOG("",LDBG);
	// get messages until there will be 4 empty rows one after another
	// (AFAIK there can only be like one or two empty rows between the TYPICAL output)
	while(this->getLastLine() != L"" || this->getLastLine() != L"" || this->getLastLine() != L"" || this->getLastLine() != L"");
}

bool ETClientConsole::isConsoleAttached()
{
	if (this->processId && IsWindow(this->hConWin) && GetWindowThreadProcessId(this->hConWin,NULL) == this->processId && GetWindowThreadProcessId(this->hReadCmd,NULL) == this->processId && GetWindowThreadProcessId(this->hWriteCmd,NULL) == this->processId)
	{
		return true;
	}
	else if (this->processId && IsWindow(this->hConWin))
	{
		LOG((std::string)"Process does exist, but windows seem to be having wrong pid: "+this->processId+", "+GetWindowThreadProcessId(this->hConWin,NULL)+", "+GetWindowThreadProcessId(this->hReadCmd,NULL)+", "+GetWindowThreadProcessId(this->hWriteCmd,NULL),LNOTE);
	}
	return this->attachConsole();
}

ETClientConsole& ETClientConsole::getInstance()
{
	return ETClientConsole::instance;
}
