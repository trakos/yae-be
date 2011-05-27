/*
 * ETClient/Listener.cpp
 *
 *  Created on: 2010-12-02
 *      Author: trakos
 */

#include <Window/AuthDialog.h>
#include <Tlogger/Front.h>
#include <string>

Window_AuthDialog Window_AuthDialog::instance = Window_AuthDialog();

Window_AuthDialog::Window_AuthDialog()
{
	const char g_szClassName[] = "authDialog";
	HBRUSH hBrush = CreateSolidBrush(RGB(208,208,208));
	WNDCLASSEX wc;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = Window_AuthDialog::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = hBrush;//16);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		LOG("Window Registration Failed!",LERR);
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
	}
}

Window_AuthDialog& Window_AuthDialog::getInstance()
{
	return Window_AuthDialog::instance;
}

Window_AuthDialogReturn Window_AuthDialog::ask(std::string title, std::string login, std::string password )
{
	Window_AuthDialogReturn returnData;
	returnData.success = false;
	this->currentFocus = -1;
	this->buttonClicked = false;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wc;
	HFONT hfDefault;
	MSG message;
	const char g_szClassName[] = "authDialog";

	HBRUSH hBrush = CreateSolidBrush(RGB(208,208,208));

	// Step 2: Creating the Window
	if ( title == "" )
	{
		title = "log in";
	}
	std::string windowTitle = "YAE - "+title;
	this->hWindow = CreateWindowEx(
					0,//WS_EX_CLIENTEDGE,
					g_szClassName, windowTitle.c_str(),
					WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 240,
					120, NULL, NULL, hInstance, NULL);

	if (this->hWindow == NULL)
	{
		LOG("Window Creation Failed!",LERR);
		MessageBox(NULL, "Window Creation Failed!", "Error!",MB_ICONEXCLAMATION | MB_OK);
		//return returnData;
	}

	this->hLogin = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 20, 10, 200, 20, this->hWindow, NULL, GetModuleHandle(NULL), NULL);
	this->hPassword = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_PASSWORD, 20, 35, 200, 20, this->hWindow, NULL, GetModuleHandle(NULL), NULL);
	this->hButton = CreateWindowEx(0, "BUTTON", "login", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 70, 60, 100, 20, this->hWindow, NULL, GetModuleHandle(NULL), NULL);
	std::vector< HWND > form = { this->hLogin, this->hPassword, this->hButton };
	this->editForm = form;

	if (this->hLogin == NULL || this->hPassword == NULL || this->hButton == NULL)
	{
		LOG("Could not create edit box or button.",LERR);
		MessageBox(this->hWindow, "Could not create edit box or button.", "Error", MB_OK | MB_ICONERROR);
		//return returnData;
	}

	hfDefault = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
	SetWindowTextA(this->hLogin, login.c_str());
	SetWindowTextA(this->hPassword, password.c_str());
	SendMessage(this->hLogin, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM(FALSE, 0));
	SendMessage(this->hPassword, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM(FALSE, 0));
	SendMessage(this->hButton, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM(FALSE, 0));

	ShowWindow(this->hWindow, SW_SHOWNORMAL);
	UpdateWindow(this->hWindow);
	SetWindowLongPtr(this->hWindow, GWL_STYLE, GetWindowLongPtr(this->hWindow, GWL_STYLE)^WS_MAXIMIZEBOX);
	SetWindowPos(this->hWindow, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	SetFocus(this->editForm[0]);

	// Step 3: The Message Loop
	while (GetMessage(&message, NULL, 0, 0) > 0 && !this->buttonClicked)
	{
		TranslateMessage(&message);
		switch (message.message)
		{
			case WM_KEYDOWN:
				switch (message.wParam)
				{
					case VK_TAB:
						SetFocus(this->editForm[currentFocus + 1]);
						break;
					case VK_RETURN:
						this->buttonClicked = true;
						PostQuitMessage(0);
						break;
					default:
						DispatchMessage(&message);
				}
				break;
			default:
				DispatchMessage(&message);
		}
	}
	DeleteObject(hBrush);
	if(this->buttonClicked)
	{
		wchar_t login[64 + 1];
		wchar_t password[64 + 1];
		GetWindowTextW(this->hLogin, &(login[0]), 64);
		GetWindowTextW(this->hPassword, &(password[0]), 64);
		returnData.login = login;
		returnData.password = password;
		returnData.success = true;
	}
	DestroyWindow(this->hButton);
	DestroyWindow(this->hLogin);
	DestroyWindow(this->hPassword);
	DestroyWindow(this->hWindow);
	while (PeekMessage(&message, NULL, 0, 0, 0))
	{
		GetMessage(&message, NULL, 0, 0);
	}
	return returnData;
}

#include <iostream>

LRESULT CALLBACK Window_AuthDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_COMMAND:
			switch( HIWORD(wParam) )
			{
				case EN_KILLFOCUS:
					Window_AuthDialog::getInstance().currentFocus = -1;
					return DefWindowProc(hwnd, msg, wParam, lParam);
				case EN_SETFOCUS:
					for( int k = 0; k < Window_AuthDialog::getInstance().editForm.size(); k++ )
					{
						HWND item = Window_AuthDialog::getInstance().editForm[k];
						if( item == (HWND)lParam )
						{
							Window_AuthDialog::getInstance().currentFocus = k;
							break;
						}
					}
					return DefWindowProc(hwnd, msg, wParam, lParam);
				case BN_CLICKED:
					if ( Window_AuthDialog::getInstance().isHButton((HWND)lParam) )
					{
						Window_AuthDialog::getInstance().buttonClicked = true;
					}
					return DefWindowProc(hwnd, msg, wParam, lParam);
			}
			break;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
