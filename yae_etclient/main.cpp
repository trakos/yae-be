/*
 * main.cpp
 *
 *  Created on: 2010-11-23
 *      Author: trakos
 */


#include <utils.h>
#include <ET/Client/Console.h>
#include <Tlogger/Front.h>
#include <ETClient.h>

int main( int argc, char** argv)
{
	IndentFacet::initialize();
	LOG.couting = LSDBG;
	if ( argc > 2 )
		ETClient::getInstance().forceAuth( argv[1], argv[2]);
	else
		ETClient::getInstance().forceAuth();
	ET_Client_Console::getInstance().toggleWindow(true);
	return ETClient::getInstance().mainLoop();
}
