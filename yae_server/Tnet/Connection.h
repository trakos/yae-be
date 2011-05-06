/*
 * Connection.h
 *
 *  Created on: 2011-01-25
 *      Author: trakos
 */

#ifndef TNET_CONNECTION_H_
#define TNET_CONNECTION_H_

#ifndef __unix__
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
		Tnet_Connection(int socket, std::string ip, int port);
		virtual ~Tnet_Connection();

		Tnet_Message receive( unsigned int timeout = 0 );
		void send(Tnet_Message message);

		std::string receiveString( unsigned int timeout = 0 );
		int receiveInt( unsigned int timeout = 0 );
		void sendString(std::string);
		void sendInt(int);

		int simpleReceiveInt( unsigned int timeout = 0 );
		const char* simpleReceive(unsigned int sizeToReceive, unsigned int timeout = 0);
		void simpleSend(std::string);
		void simpleSend(int);

		std::string getIp();
		int getPort();

		unsigned int timeTakenOnLastOperation;
	protected:
		std::string ip;
		int port;
		int clientSocket;
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

		void initialize(int socket, std::string ip, int port);
		void socketMode(bool asynchronous);
		void sendError(std::string functionName);
		bool sleepForTenthSecond();
};

#endif /* TNET_CONNECTION_H_ */
