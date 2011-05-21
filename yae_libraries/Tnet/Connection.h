/*
 * Connection.h
 *
 *  Created on: 2011-01-25
 *      Author: trakos
 */

#ifndef TNET_CONNECTION_H_
#define TNET_CONNECTION_H_

#ifdef __unix__
	#include <netinet/in.h>
#else
	#include <windows.h>
	#include <winsock.h>
#endif

#include <string>
#include <Tnet/Message.h>
#include <Tnet/Exception.h>
#include <Tnet/ConnectionClosedException.h>
#include <Tnet/TimeoutException.h>
#include <Tnet/ProtocolException.h>
#include <Tnet/MessageBeingReceivedException.h>

enum Tnet_PossibleActions { TNET_NONE, TNET_RECEIVING_STRING, TNET_RECEIVING_INT, TNET_RECEIVING_MESSAGE };


struct Tnet_MessageReceivedStatus
{
	Tnet_Message value;
	int sizeInts;
	int sizeStrings;
	std::string lastKey;
	bool started;
	bool mReceived;
	bool sizeIntsReceived;
	bool sizeStringsReceived;
	bool keyReceived;
	bool endingReceived;
};

struct Tnet_IntReceivedStatus
{
	int value;
	bool started;
	bool iReceived;
	bool intReceived;
	bool endingReceived;
};

struct Tnet_StringReceivedStatus
{
	int size;
	std::string value;
	bool started;
	bool sReceived;
	bool sizeReceived;
	bool stringReceived;
	bool endingReceived;
};

class Tnet_Connection
{
	public:
		Tnet_Connection(int socket, std::string ip, unsigned int port, sockaddr_in peer);
		virtual ~Tnet_Connection();

		virtual Tnet_Message receive( unsigned int timeout = 0 );
		virtual void send(Tnet_Message message);

		virtual std::string receiveString( unsigned int timeout = 0 );
		virtual int receiveInt( unsigned int timeout = 0 );
		virtual void sendString(std::string);
		virtual void sendInt(int);

		virtual int simpleReceiveInt( unsigned int timeout = 0);
		virtual const char* simpleReceive(unsigned int sizeToReceive, unsigned int timeout = 0, bool managedConnection=true);
		virtual void simpleSend(std::string string);
		virtual void simpleSend(int number);

		std::string getIp();
		unsigned int getPort();
		unsigned int getIpFromString(std::string hostString);

		unsigned int timeTakenOnLastOperation;
	protected:
		std::string ip;
		unsigned int port;
		int clientSocket;
		sockaddr_in peer;
#ifndef __unix__
		WSADATA wsData;
#endif
		bool wsInitialized;
		char* buffer;
		char* returnBuffer;
		unsigned int bufferSize;
		unsigned int bufferPosition;

		Tnet_MessageReceivedStatus messageReceivedStatus;
		Tnet_IntReceivedStatus intReceivedStatus;
		Tnet_StringReceivedStatus stringReceivedStatus;

		Tnet_PossibleActions bufferedAction;

		Tnet_Connection();

		virtual ssize_t coreReceive(unsigned int sizeToReceive);
		virtual ssize_t coreSend(const char* buffer, size_t len);


		void initialize(int socket, std::string ip, unsigned int port, sockaddr_in peer);
		void socketMode(bool asynchronous);
		void sendError(std::string functionName);
		bool sleepForTenthSecond();
};

#endif /* TNET_CONNECTION_H_ */
