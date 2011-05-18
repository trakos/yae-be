/*
 * ETClient/Input.cpp
 *
 *  Created on: 2010-12-02
 *      Author: trakos
 */

#include <ET/Client/Console.h>
#include <ET/Client/Input.h>
#include <ET/Client/Info.h>
#include <boost/foreach.hpp>
#include <vector>
#include <utils.h>


ET_Client_Input ET_Client_Input::instance = ET_Client_Input();
char ET_Client_Input::color = '5';

ET_Client_Input::ET_Client_Input(){}

ET_Client_Input& ET_Client_Input::getInstance()
{
	return ET_Client_Input::instance;
}

bool ET_Client_Input::shortMessageW(std::wstring message,std::wstring nocolor,std::wstring color)
{
	bool success = ET_Client_Console::getInstance().sendMessage(L"echo ^3YAE: ^5"+message+nocolor+L"^5"+color);
	if ( success )
	{
		Sleep(ET_Client_Info::getInstance().waitingForETCommandTime);
		ET_Client_Console::getInstance().getLastLine();
	}
	return success;
}

bool ET_Client_Input::shortMessage(std::string message,std::string nocolor,std::string color)
{
	return this->shortMessageW(atow(message), atow(nocolor), atow(color));
}

/*bool ET_Client_Input::multiLineMessage(std::wstring message)
{
	std::vector< std::wstring > array = wsplit(message,L"\n\r");
	bool success = true;
	BOOST_FOREACH(std::wstring x, array)
	{
		success &= this->shortMessageW(x);
	}
	return success;
}*/

