
#ifdef __unix__
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <sys/socket.h>
	#include <string.h>
	#include <errno.h>
	#define closesocket(a) close(a)
	#define getNetErrno() errno
#else
	#include <windows.h>
	#include <winsock.h>
	#define EWOULDBLOCK WSAEWOULDBLOCK
	#define getNetErrno() WSAGetLastError()
#endif

#include <Tnet/Server.h>
#include <string>


Tnet_Server::Tnet_Server(int port) : connectionPending(NULL)
{
	struct sockaddr_in sock_local;
	#ifndef __unix__
		this->wsInitialized = false;
		memset(&(this->wsData), 0, sizeof(this->wsData));
		if (WSAStartup(0x0101, &(this->wsData)) != 0)
		{
			Tnet_Exception e("WSAStartup(): failure!");
			throw e;
		}
		this->wsInitialized = true;
	#endif
	this->serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->serverSocket == -1)
	{
		this->sendError("socket");
	}
	sock_local.sin_family = AF_INET;
	sock_local.sin_addr.s_addr = inet_addr("0.0.0.0");
	sock_local.sin_port = htons(port);
	if(bind(this->serverSocket, (struct sockaddr*)&sock_local, sizeof(struct sockaddr)) == -1)
	{
		this->sendError("bind");
	}
	if(listen(this->serverSocket, SOMAXCONN) == -1)
	{
		this->sendError("listen");
	}
	this->socketMode(true);
}

Tnet_Server::~Tnet_Server()
{
	closesocket(this->serverSocket);
	#ifndef __unix__
		if( this->wsInitialized )
		{
			WSACleanup();
		}
	#endif
}

bool Tnet_Server::sleepForSecond()
{
	#ifdef __unix__
		usleep(1000);
	#else
		Sleep(1);
	#endif
	return true;
}

bool Tnet_Server::isNewConnectionAvailable(unsigned int timeout)
{
	if(this->connectionPending)
	{
		return true;
	}
	struct sockaddr_in sock_remote;
	int size = sizeof(struct sockaddr);
	int connectionSocket;
	unsigned int currentTime = 0;
	do
	{
		connectionSocket = accept(this->serverSocket, (struct sockaddr*)&sock_remote, (socklen_t*)&size);
	} while( currentTime++ != timeout && connectionSocket == -1 && getNetErrno() == EWOULDBLOCK && this->sleepForSecond() );
	if ( connectionSocket == -1 && getNetErrno() == EWOULDBLOCK )
	{
		return false;
	}
	else if ( connectionSocket == -1 )
	{
		this->sendError("accept");
	}
	else
	{
		std::string ip = inet_ntoa(*(struct in_addr*)&sock_remote.sin_addr.s_addr);
		int port = htons(sock_remote.sin_port);
		this->connectionPending = new Tnet_Connection(connectionSocket, ip, port);
		return true;
	}
}

Tnet_Connection* Tnet_Server::getNewConnection()
{
	if ( !this->isNewConnectionAvailable(0) )
	{
		throw Tnet_TimeoutException();
	}
	Tnet_Connection* connection = this->connectionPending;
	this->connectionPending = NULL;
	return connection;
}

/**
 * @param bool asynchronous whether to make the serverSocket non-blocking
 */
void Tnet_Server::socketMode(bool asynchronous)
{
	#ifdef __unix__
		fcntl(this->serverSocket, F_SETFL, asynchronous ? O_NONBLOCK : 0 );
	#else
		unsigned long makeAsynchronous = asynchronous ? 1 : 0;
		ioctlsocket(this->serverSocket, FIONBIO, (unsigned long*)&makeAsynchronous);
	#endif
}

void Tnet_Server::sendError(std::string functionName)
{
	#ifdef __unix__
		const char* errorString = strerror(errno);
	#else
		char errorString[1024];
		FormatMessage(
		   FORMAT_MESSAGE_FROM_SYSTEM,
		   NULL,
		   GetLastError(),
		   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		   errorString, sizeof(errorString), NULL );
	#endif
	Tnet_Exception e(functionName + "(): " + errorString);
	throw e;
}
