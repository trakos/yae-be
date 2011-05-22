/*
 * ETClient/Console.h
 *
 *  Created on: 2010-11-26
 *      Author: trakos
 */

#ifndef ET_CLIENT_CONSOLE_H_
#define ET_CLIENT_CONSOLE_H_

#ifndef __unix__
	#include <Windows.h>
#endif
#include <string>


class ET_Client_Console
{
	protected:
#ifndef __unix__
		HWND hConWin;
		HWND hWriteCmd;
		HWND hReadCmd;
#else
#endif
		int charConsolePointer;
		unsigned long processId;
		static ET_Client_Console* instance;
		bool showWindow;
		ET_Client_Console();

	public:

		static ET_Client_Console& getInstance();
		void moveToTheEnd();
		bool attachConsole();
		bool sendMessage(std::wstring message);
		std::wstring getLastLine();
		bool isConsoleAttached();
		void toggleWindow(bool show);
};

#endif /* ET_CLIENT_CONSOLE_H_ */
