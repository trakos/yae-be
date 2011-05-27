/*
 * EtClient.h
 *
 *  Created on: 2011-05-17
 *      Author: trakos
 */

#ifndef ETCLIENT_H_
#define ETCLIENT_H_

#include <ET/Status.h>
#include <string>

class ETClient
{
	protected:
		ETClient();
		static ETClient instance;
		unsigned int iteration;
		ET_Status last_status;
	public:
		static ETClient& getInstance();
		void onlineWithoutET();
		void onlineWithET();
		void performYaeSearch(bool forceUpdate);
		void printServerStatus();
		void forceAuth(std::string login="", std::string password="");
		int mainLoop();
};

#endif /* ETCLIENT_H_ */
