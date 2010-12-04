/*
 * ETClient/Input.cpp
 *
 *  Created on: 2010-12-02
 *      Author: trakos
 */

#include <ETClient/Console.h>
#include <ETClient/Input.h>
#include <boost/foreach.hpp>
#include <vector>
#include <utils.h>


ETClientInput ETClientInput::instance = ETClientInput();

ETClientInput::ETClientInput(){}

ETClientInput& ETClientInput::getInstance()
{
	return ETClientInput::instance;
}

bool ETClientInput::shortMessage(std::wstring message,std::wstring nocolor,std::wstring color)
{
	return ETClientConsole::getInstance().sendMessage(L"echo ^3YAE: ^5"+message+nocolor+L"^5"+color);
}

/*bool ETClientInput::multiLineMessage(std::wstring message)
{
	std::vector< std::wstring > array = wsplit(message,L"\n\r");
	bool success = true;
	BOOST_FOREACH(std::wstring x, array)
	{
		success &= this->shortMessage(x);
	}
	return success;
}*/

