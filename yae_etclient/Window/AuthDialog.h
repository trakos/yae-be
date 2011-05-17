/*
 * MessageBox.h
 *
 *  Created on: 2011-01-03
 *      Author: trakos
 */

#ifndef WINDOW_AUTH_DIALOG_H_
#define WINDOW_AUTH_DIALOG_H_

#include <windows.h>
#include <string>
#include <vector>

struct Window_AuthDialogReturn
{
	std::wstring login;
	std::wstring password;
	bool success;
};

class Window_AuthDialog
{
	protected:
		static Window_AuthDialog instance;
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		HWND hLogin;
		HWND hPassword;
		HWND hButton;
		HWND hWindow;
		Window_AuthDialog();

	public:
		static Window_AuthDialog& getInstance();
		bool isHButton(HWND cmp) { return cmp==this->hButton; }
		Window_AuthDialogReturn ask( std::string title="" );
		int currentFocus;
		std::vector< HWND > editForm;
		bool buttonClicked;
};

#endif /* WINDOW_AUTH_DIALOG_H_ */
