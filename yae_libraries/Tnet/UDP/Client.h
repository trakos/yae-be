/*
 * Client.h
 *
 *  Created on: 2011-05-21
 *      Author: trakos
 */

#ifndef TNET_UDP_CLIENT_H_
#define TNET_UDP_CLIENT_H_

#include <Tnet/UDP/Connection.h>

class Tnet_UDP_Client : public Tnet_UDP_Connection
{
	public:
		Tnet_UDP_Client(std::string ip, unsigned int port);
};

#endif /* TNET_UDP_CLIENT_H_ */
