/*
 * main.cpp
 *
 *  Created on: 2010-11-23
 *      Author: trakos
 */


#include <utils.h>
#include <indent.h>
#include <ETClient/Console.h>
#include <Logger/Logger.h>
#include <ETClient.h>


int main()
{

	IndentFacet::initialize();
	LOG.couting = LSDBG;
	ETClientConsole::getInstance().toggleWindow(true);
	return ETClient::getInstance().mainLoop();
}
