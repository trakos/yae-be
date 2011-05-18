/*
 * ETClient/Input.cpp
 *
 *  Created on: 2010-12-02
 *      Author: trakos
 */

#include <ETClient/Console.h>
#include <ETClient/Input.h>
#include <ETClient/Info.h>
#include <boost/foreach.hpp>
#include <vector>
#include <utils.h>


ETClientInput ETClientInput::instance = ETClientInput();
char ETClientInput::color = '5';

ETClientInput::ETClientInput(){}

ETClientInput& ETClientInput::getInstance()
{
	return ETClientInput::instance;
}

bool ETClientInput::shortMessageW(std::wstring message,std::wstring nocolor,std::wstring color)
{
	bool success = ETClientConsole::getInstance().sendMessage(L"echo ^3YAE: ^5"+message+nocolor+L"^5"+color);
	if ( success )
	{
		Sleep(ETClientInfo::getInstance().waitingForETCommandTime);
		ETClientConsole::getInstance().getLastLine();
	}
	return success;
}

bool ETClientInput::shortMessage(std::string message,std::string nocolor,std::string color)
{
	return this->shortMessageW(atow(message), atow(nocolor), atow(color));
}

/*bool ETClientInput::multiLineMessage(std::wstring message)
{
	std::vector< std::wstring > array = wsplit(message,L"\n\r");
	bool success = true;
	BOOST_FOREACH(std::wstring x, array)
	{
		success &= this->shortMessageW(x);
	}
	return success;
}*/

