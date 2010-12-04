/*
 * main.cpp
 *
 *  Created on: 2010-11-23
 *      Author: trakos
 */
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <initializer_list>
#include <utils.h>
#include <ETClient/Console.h>
#include <ETClient/Input.h>
#include <ETClient/Listener.h>

using std::wstring;
using std::wcout;
using std::endl;

typedef std::vector< wstring > array;
typedef std::vector< std::vector < wstring > > array2D;


int main()
{
	ETClientConsole::getInstance().toggleWindow(true);
	ETClientConsole::getInstance().moveToTheEnd();
	ETClientConsole::getInstance().sendMessage(L"players");

	return 0;
	while(1)
	{
		wstring str = ETClientListener::getInstance().getCommand();
		if(str!=L"")
		{
			wcout << 'p' << str << 'e' << endl;
		}
		Sleep(100);
	}


	ETClientConsole etconsole = ETClientConsole::getInstance();
	etconsole.sendMessage(L"echo lol");
	while(!etconsole.isConsoleAttached())
	{
		etconsole.attachConsole();
		fflush(stdout);
		Sleep(1000);
	}
	while(1)
	{
		wstring lastLine = etconsole.getLastLine();
		if(!lastLine.empty())
		{
			wcout << lastLine << std::flush;
		}
	}
}
