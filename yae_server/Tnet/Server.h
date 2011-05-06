/*
 * Server.h
 *
 *  Created on: 2011-01-25
 *      Author: trakos
 */

#ifndef TNET_SERVER_H_
#define TNET_SERVER_H_

#ifndef __unix__
	#include <windows.h>
	#include <winsock.h>
#endif

#include <Tnet/Connection.h>
#include <Tnet/Exception.h>
#include <Tnet/TimeoutException.h>

class Tnet_Server
{
	public:
		Tnet_Server(int port);
		~Tnet_Server();
		bool isNewConnectionAvailable(unsigned int timeout);
		Tnet_Connection* getNewConnection();
	protected:
		int serverSocket;
#ifndef __unix__
		WSADATA wsData;
#endif
		bool wsInitialized;
		Tnet_Connection* connectionPending;
		void socketMode(bool asynchronous);
		void sendError(std::string functionName);
		bool sleepForSecond();
};

#endif /* TNET_SERVER_H_ */
