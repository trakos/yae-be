#include <Tnet/Client.h>

#ifdef __unix__
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <sys/socket.h>
	#include <string.h>
	#define closesocket(a) close(a)
#else
	#include <windows.h>
	#include <winsock.h>
	#define EWOULDBLOCK WSAEWOULDBLOCK
#endif

#include <string>

Tnet_Client::Tnet_Client(std::string ip, int port) : Tnet_Connection()
{
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = this->getIpFromString(ip);
	memset(address.sin_zero, '\0', sizeof(address));
	int socketid = socket(AF_INET, SOCK_STREAM, PF_UNSPEC );
	if ( socketid == -1 )
	{
		this->sendError("socket");
	}
	int ret = connect(socketid, (struct sockaddr *) &address, sizeof(struct sockaddr));
	if ( ret == -1 )
	{
		this->sendError("connect");
	}
	this->initialize(socketid, ip, port, address);
}
