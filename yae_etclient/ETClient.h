/*
 * EtClient.h
 *
 *  Created on: 2011-05-17
 *      Author: trakos
 */

#ifndef ETCLIENT_H_
#define ETCLIENT_H_


class ETClient
{
	protected:
		ETClient();
		static ETClient instance;
		unsigned int iteration;
	public:
		static ETClient& getInstance();
		void onlineWithoutET();
		void onlineWithET();
		void performYaeSearch();
		void printServerStatus();
		void forceAuth();
		int mainLoop();
};

#endif /* ETCLIENT_H_ */
