/*
 * ETClient/Console.h
 *
 *  Created on: 2010-11-26
 *      Author: trakos
 */

#ifndef ET_CLIENT_CONSOLE_H_
#define ET_CLIENT_CONSOLE_H_

#include <Windows.h>
#include <string>


class ETClientConsole
{
	protected:
		HWND hConWin;
		HWND hWriteCmd;
		HWND hReadCmd;
		int charConsolePointer;
		unsigned long processId;
		static ETClientConsole instance;
		bool showWindow;
		ETClientConsole();

	public:

		static ETClientConsole& getInstance();
		void moveToTheEnd();
		bool attachConsole();
		bool sendMessage(std::wstring message);
		std::wstring getLastLine();
		bool isConsoleAttached();
		void toggleWindow(bool show);
};

#endif /* ET_CLIENT_CONSOLE_H_ */
