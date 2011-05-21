/*
 * Connection.h
 *
 *  Created on: 2011-05-21
 *      Author: trakos
 */

#ifndef TNET_UDP_CONNECTION_H_
#define TNET_UDP_CONNECTION_H_

#include <Tnet/Connection.h>

class Tnet_UDP_Connection : public Tnet_Connection
{
		Tnet_UDP_Connection(int socket, std::string ip, unsigned int port, sockaddr_in peer);
	protected:
		Tnet_UDP_Connection();
		virtual ssize_t coreReceive(unsigned int sizeToReceive);
		virtual ssize_t coreSend(const char* buffer, size_t len);
};

#endif /* TNET_UDP_CONNECTION_H_ */
