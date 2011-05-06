/*
 * Client.h
 *
 *  Created on: 2011-01-25
 *      Author: trakos
 */

#ifndef TNET_CLIENT_H_
#define TNET_CLIENT_H_

#include <Tnet/Connection.h>

class Tnet_Client : public Tnet_Connection
{
	public:
		Tnet_Client(std::string ip, int port);
};

#endif /* TNET_CLIENT_H_ */
