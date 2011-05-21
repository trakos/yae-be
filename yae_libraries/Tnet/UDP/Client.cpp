#include <Tnet/UDP/Client.h>

#include <string.h>

Tnet_UDP_Client::Tnet_UDP_Client(std::string ip, int port) : Tnet_UDP_Connection()
{
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = this->getIpFromString(ip);
	memset(address.sin_zero, '\0', sizeof(address));
	int socketid = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if ( socketid == -1 )
	{
		this->sendError("socket");
	}
	this->initialize(socketid, ip, port, address);
}
