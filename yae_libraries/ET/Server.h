/*
 * ET/Server.h
 *
 *  Created on: 2011-05-21
 *      Author: trakos
 */



#ifndef ET_SERVER_H_
#define ET_SERVER_H_

#include<string>
#include<boost/smart_ptr.hpp>

#include<Tnet/Connection.h>
#include<Tnet/UDP/Client.h>
#include<ET/Status.h>

class ET_Server_Exception : public std::runtime_error
{
	public:
		ET_Server_Exception(const std::string &what) : std::runtime_error( what ) {};
};

class ET_Server
{
	public:
		ET_Server(std::string ip, unsigned int port);
		ET_Status getPublicStatus(unsigned int timeout=50);
		unsigned int getNumericIp();
		std::string getIp();
		unsigned int getPort();
		static std::string colorless(std::string name);
	protected:
		std::string ip;
		unsigned int port;
		boost::shared_ptr<Tnet_UDP_Client> connection;
};

#endif /* ET_SERVER_H_ */
