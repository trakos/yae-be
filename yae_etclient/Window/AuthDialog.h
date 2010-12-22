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

struct WindowAuthDialogReturn
{
	std::wstring login;
	std::wstring password;
	bool success;
};

class WindowAuthDialog
{
	protected:
		static WindowAuthDialog instance;
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		WindowAuthDialog();
		HWND hLogin;
		HWND hPassword;
		HWND hButton;
		HWND hWindow;

	public:
		static WindowAuthDialog& getInstance();
		WindowAuthDialogReturn ask();
		int currentFocus;
		std::vector< HWND > editForm;
		bool buttonClicked;
};

#endif /* WINDOW_AUTH_DIALOG_H_ */
