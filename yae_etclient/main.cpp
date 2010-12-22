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
#include <ETClient/Info.h>
#include <Window/AuthDialog.h>
#include <Logger/Logger.h>
#include <windows.h>

using std::wstring;
using std::wcout;
using std::endl;

typedef std::vector< wstring > array;
typedef std::vector< std::vector < wstring > > array2D;

int main()
{
	WindowAuthDialogReturn data = WindowAuthDialog::getInstance().ask();
	wcout << data.success << "\t" << data.login << " " << data.password << endl;
	//showWindow();
	//MessageBox(NULL, "Goodbye, cruel world!", "Note", MB_OK);
	return 0;
	LOG.couting = LSDBG;
	LOG("",LSDBG);
	int k = MessageBox(NULL, "Za mało pamięci RAM", "My application", MB_OKCANCEL | MB_DEFBUTTON2 | MB_ICONWARNING | MB_SYSTEMMODAL);
	wcout << k << endl;
	return 0;
	ETClientConsole::getInstance().toggleWindow(true);
	ETClientConsole::getInstance().moveToTheEnd();
	ETClientStatus status = ETClientInfo::getInstance().getStatus();
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
