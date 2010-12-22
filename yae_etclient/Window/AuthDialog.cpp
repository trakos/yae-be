/*
 * ETClient/Listener.cpp
 *
 *  Created on: 2010-12-02
 *      Author: trakos
 */

#include <Window/AuthDialog.h>
#include <string>

WindowAuthDialog WindowAuthDialog::instance = WindowAuthDialog();

WindowAuthDialog::WindowAuthDialog(){}

WindowAuthDialog& WindowAuthDialog::getInstance()
{
	return WindowAuthDialog::instance;
}

WindowAuthDialogReturn WindowAuthDialog::ask()
{
	WindowAuthDialogReturn returnData;
	returnData.success = false;
	this->currentFocus = -1;
	this->buttonClicked = false;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wc;
	HFONT hfDefault;
	MSG message;
	const char g_szClassName[] = "authDialog";

	HBRUSH hBrush = CreateSolidBrush(RGB(208,208,208));


	//Step 1: Registering the Window Class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WindowAuthDialog::WndProc;
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
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return returnData;
	}

	// Step 2: Creating the Window
	this->hWindow
			= CreateWindowEx(
					0,//WS_EX_CLIENTEDGE,
					g_szClassName, "YAE - log in",
					WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 240,
					120, NULL, NULL, hInstance, NULL);

	if (this->hWindow == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
				MB_ICONEXCLAMATION | MB_OK);
		return returnData;
	}

	this->hLogin = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE, 20, 10, 200, 20, this->hWindow, NULL, GetModuleHandle(NULL), NULL);
	this->hPassword = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_PASSWORD, 20, 35, 200, 20, this->hWindow, NULL, GetModuleHandle(NULL), NULL);
	this->hButton = CreateWindowEx(0, "BUTTON", "login", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 70, 60, 100, 20, this->hWindow, NULL, GetModuleHandle(NULL), NULL);
	std::vector< HWND > form = { this->hLogin, this->hPassword, this->hButton };
	this->editForm = form;

	if (this->hLogin == NULL || this->hPassword == NULL || this->hButton == NULL)
	{
		MessageBox(this->hWindow, "Could not create edit box or button.", "Error", MB_OK | MB_ICONERROR);
	}

	hfDefault = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
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
	return returnData;
}

LRESULT CALLBACK WindowAuthDialog::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
					WindowAuthDialog::getInstance().currentFocus = -1;
					break;
				case EN_SETFOCUS:
					for( int k = 0; k < WindowAuthDialog::getInstance().editForm.size(); k++ )
					{
						HWND item = WindowAuthDialog::getInstance().editForm[k];
						if( item == (HWND)lParam )
						{
							WindowAuthDialog::getInstance().currentFocus = k;
							break;
						}
					}
					break;
			}
			break;
		case WM_PARENTNOTIFY:
			switch( LOWORD(wParam) )
			{
				case WM_LBUTTONDOWN:
					WindowAuthDialog::getInstance().buttonClicked = true;
					break;
			}
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
