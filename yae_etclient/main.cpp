/*
 * main.cpp
 *
 *  Created on: 2010-11-23
 *      Author: trakos
 */


#include <utils.h>
#include <indent.h>
#include <ET/Client/Console.h>
#include <Tlogger/Front.h>
#include <ETClient.h>

int main()
{
	IndentFacet::initialize();
	LOG.couting = LSDBG;
	ET_Client_Console::getInstance().toggleWindow(true);
	//ETClient::getInstance().printServerStatus();
	ETClient::getInstance().forceAuth();
	return ETClient::getInstance().mainLoop();
}
