#include <Tnet/Connection.h>

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

#include <string>

Tnet_Connection::Tnet_Connection(int _clientSocket, std::string _ip, int _port)
{
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
	this->initialize(_clientSocket,_ip,_port);
}

Tnet_Connection::Tnet_Connection()
{
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
}

void Tnet_Connection::initialize(int _clientSocket, std::string _ip, int _port)
{
	this->clientSocket = _clientSocket;
	this->ip = _ip;
	this->port = _port;
	this->buffer = NULL;
	this->bufferSize = 0;
	this->bufferPosition = 0;
	this->returnBuffer = NULL;
	this->messageReceivedStatus.started = false;
	this->intReceivedStatus.started = false;
	this->stringReceivedStatus.started = false;
	this->bufferedAction = TNET_NONE;
	this->socketMode(true);
}

std::string Tnet_Connection::getIp()
{
	return this->ip;
}

int Tnet_Connection::getPort()
{
	return this->port;
}

Tnet_Connection::~Tnet_Connection()
{
	closesocket(this->clientSocket);
	if( this->buffer )
	{
		delete[] this->buffer;
	}
	if( this->returnBuffer )
	{
		delete[] this->returnBuffer;
	}
}

/**
 * @param bool asynchronous whether to make the serverSocket non-blocking
 */
void Tnet_Connection::socketMode(bool asynchronous)
{
	#ifdef __unix__
		fcntl(this->clientSocket, F_SETFL, asynchronous ? O_NONBLOCK : 0 );
	#else
		unsigned long makeAsynchronous = asynchronous ? 1 : 0;
		ioctlsocket(this->clientSocket, FIONBIO, (unsigned long*)&makeAsynchronous);
	#endif
}

void Tnet_Connection::sendError(std::string functionName)
{
	#ifdef __unix__
		const char* errorString = strerror(errno);
	#else
		char  errorString[1024];
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

bool Tnet_Connection::sleepForTenthSecond()
{
	#ifdef __unix__
		usleep(100);
	#else
		Sleep(100);
	#endif
	return true;
}

void Tnet_Connection::send(Tnet_Message message)
{
	this->simpleSend("m");
	unsigned int size = message.ints.size();
	this->sendInt(size);
	intMapIterator intsEnd = message.ints.end();
	for (intMapIterator it = message.ints.begin(); it != intsEnd; ++it)
	{
		this->sendString(it->first);
		this->sendInt(it->second);
	}
	size = message.strings.size();
	this->sendInt(size);
	stringMapIterator stringsEnd = message.strings.end();
	for (stringMapIterator it = message.strings.begin(); it != stringsEnd; ++it)
	{
		this->sendString(it->first);
		this->sendString(it->second);
	}
	this->simpleSend("\r");
}

Tnet_Message Tnet_Connection::receive( unsigned int timeout )
{
	if ( this->bufferedAction != TNET_NONE && this->bufferedAction != TNET_RECEIVING_MESSAGE )
	{
		throw Tnet_MessageBeingReceivedException();
	}
	this->bufferedAction = TNET_RECEIVING_MESSAGE;
	unsigned int timeTaken = 0;
	if ( !this->messageReceivedStatus.started )
	{
		this->messageReceivedStatus.value.clear();
		this->messageReceivedStatus.started = true;
		this->messageReceivedStatus.mReceived = false;
		this->messageReceivedStatus.sizeIntsReceived = false;
		this->messageReceivedStatus.sizeStringsReceived = false;
		this->messageReceivedStatus.keyReceived = false;
		this->messageReceivedStatus.endingReceived = false;
	}
	try
	{
		if ( !this->messageReceivedStatus.mReceived )
		{
			if (this->simpleReceive( 1, timeout - timeTaken )[0] != 'm')
			{
				throw Tnet_ProtocolException();
			}
			timeTaken += this->timeTakenOnLastOperation;
			this->messageReceivedStatus.mReceived = true;
		}
		if ( !this->messageReceivedStatus.sizeIntsReceived )
		{
			this->messageReceivedStatus.sizeInts = this->receiveInt( timeout - timeTaken );
			timeTaken += this->timeTakenOnLastOperation;
			this->messageReceivedStatus.sizeIntsReceived = true;
		}
		for (int k = this->messageReceivedStatus.value.ints.size(); k < this->messageReceivedStatus.sizeInts; k++)
		{
			if ( !this->messageReceivedStatus.keyReceived )
			{
				this->messageReceivedStatus.lastKey = this->receiveString( timeout - timeTaken );
				timeTaken += this->timeTakenOnLastOperation;
				this->messageReceivedStatus.keyReceived = true;
			}
			int value = this->receiveInt( timeout - timeTaken );
			timeTaken += this->timeTakenOnLastOperation;
			this->messageReceivedStatus.value.ints[this->messageReceivedStatus.lastKey] = value;
			this->messageReceivedStatus.keyReceived = false;
		}
		if ( !this->messageReceivedStatus.sizeStringsReceived )
		{
			this->messageReceivedStatus.sizeStrings = this->receiveInt( timeout - timeTaken );
			timeTaken += this->timeTakenOnLastOperation;
			this->messageReceivedStatus.sizeStringsReceived = true;
		}
		for (int k = this->messageReceivedStatus.value.strings.size(); k < this->messageReceivedStatus.sizeStrings; k++)
		{
			if ( !this->messageReceivedStatus.keyReceived )
			{
				this->messageReceivedStatus.lastKey = this->receiveString( timeout - timeTaken );
				timeTaken += this->timeTakenOnLastOperation;
				this->messageReceivedStatus.keyReceived = true;
			}
			std::string value = this->receiveString( timeout - timeTaken );
			timeTaken += this->timeTakenOnLastOperation;
			this->messageReceivedStatus.value.strings[this->messageReceivedStatus.lastKey] = value;
			this->messageReceivedStatus.keyReceived = false;
		}
		if ( !this->messageReceivedStatus.endingReceived )
		{
			if( this->simpleReceive(1, timeout-timeTaken)[0] != '\r' )
			{
				throw Tnet_ProtocolException();
			}
			timeTaken += this->timeTakenOnLastOperation;
			this->messageReceivedStatus.endingReceived = true;
		}
	}
	catch ( Tnet_TimeoutException e )
	{
		this->bufferedAction = TNET_RECEIVING_MESSAGE;
		throw e;
	}
	this->timeTakenOnLastOperation = timeTaken;
	return this->messageReceivedStatus.value;
}

void Tnet_Connection::sendString(std::string message)
{
	this->simpleSend("s");
	this->simpleSend(message.size());
	this->simpleSend(message);
	this->simpleSend("\r");
}

void Tnet_Connection::sendInt(int number)
{
	this->simpleSend("i");
	this->simpleSend(number);
	this->simpleSend("\r");
}

std::string Tnet_Connection::receiveString( unsigned int timeout )
{
	if ( this->bufferedAction != TNET_NONE && this->bufferedAction != TNET_RECEIVING_STRING && this->bufferedAction != TNET_RECEIVING_MESSAGE )
	{
		throw Tnet_MessageBeingReceivedException();
	}
	this->bufferedAction = TNET_RECEIVING_STRING;
	unsigned int timeTaken = 0;
	if ( !this->stringReceivedStatus.started )
	{
		this->stringReceivedStatus.started = true;
		this->stringReceivedStatus.sReceived = false;
		this->stringReceivedStatus.sizeReceived = false;
		this->stringReceivedStatus.stringReceived = false;
		this->stringReceivedStatus.endingReceived = false;
	}
	if ( !this->stringReceivedStatus.sReceived )
	{
		if( this->simpleReceive(1, timeout-timeTaken)[0] != 's' )
		{
			throw Tnet_ProtocolException();
		}
		timeTaken += this->timeTakenOnLastOperation;
		this->stringReceivedStatus.sReceived = true;
	}
	if ( !this->stringReceivedStatus.sizeReceived )
	{
		this->stringReceivedStatus.size = this->simpleReceiveInt(timeout-timeTaken);
		timeTaken += this->timeTakenOnLastOperation;
		this->stringReceivedStatus.sizeReceived = true;
	}
	if ( !this->stringReceivedStatus.stringReceived )
	{
		this->stringReceivedStatus.value = this->simpleReceive(this->stringReceivedStatus.size, timeout-timeTaken);
		timeTaken += this->timeTakenOnLastOperation;
		this->stringReceivedStatus.stringReceived = true;
	}
	if ( !this->stringReceivedStatus.endingReceived )
	{
		if( this->simpleReceive(1, timeout-timeTaken)[0] != '\r' )
		{
			throw Tnet_ProtocolException();
		}
		timeTaken += this->timeTakenOnLastOperation;
		this->stringReceivedStatus.endingReceived = true;
	}
	this->bufferedAction = TNET_NONE;
	this->timeTakenOnLastOperation = timeTaken;
	this->stringReceivedStatus.started = false;
	return this->stringReceivedStatus.value;
}

int Tnet_Connection::receiveInt( unsigned int timeout )
{
	if ( this->bufferedAction != TNET_NONE && this->bufferedAction != TNET_RECEIVING_INT && this->bufferedAction != TNET_RECEIVING_MESSAGE )
	{
		throw Tnet_MessageBeingReceivedException();
	}
	this->bufferedAction = TNET_RECEIVING_INT;
	unsigned int timeTaken = 0;
	if ( !this->intReceivedStatus.started )
	{
		this->intReceivedStatus.iReceived = false;
		this->intReceivedStatus.intReceived = false;
		this->intReceivedStatus.endingReceived = false;
	}
	if ( !this->intReceivedStatus.iReceived )
	{
		if( this->simpleReceive(1, timeout-timeTaken)[0] != 'i' )
		{
			throw Tnet_ProtocolException();
		}
		timeTaken += this->timeTakenOnLastOperation;
		this->intReceivedStatus.iReceived = true;
	}
	if ( !this->intReceivedStatus.intReceived )
	{
		this->intReceivedStatus.value = this->simpleReceiveInt(timeout-timeTaken);
		timeTaken += this->timeTakenOnLastOperation;
		this->intReceivedStatus.intReceived = true;
	}
	if ( !this->intReceivedStatus.endingReceived )
	{
		if( this->simpleReceive(1, timeout-timeTaken)[0] != '\r' )
		{
			throw Tnet_ProtocolException();
		}
		timeTaken += this->timeTakenOnLastOperation;
		this->intReceivedStatus.endingReceived = true;
	}
	this->bufferedAction = TNET_NONE;
	this->timeTakenOnLastOperation = timeTaken;
	this->intReceivedStatus.started = false;
	return this->intReceivedStatus.value;
}

const char* Tnet_Connection::simpleReceive( unsigned int sizeToReceive, unsigned int timeout )
{
	this->bufferedAction = TNET_NONE;
	if ( sizeToReceive > 32767 )
	{
		throw Tnet_Exception("too big sizeToReceive");
	}
	if( sizeToReceive > this->bufferSize )
	{
		if( this->buffer )
		{
			const char* tmp = this->buffer;
			this->buffer = new char[sizeToReceive+1];
			memcpy(this->buffer,tmp,this->bufferPosition);
			delete[] tmp;
		}
		else
		{
			this->buffer = new char[sizeToReceive+1];
		}
		this->bufferPosition = 0;
		this->bufferSize = sizeToReceive;
	}
	if ( sizeToReceive - this->bufferPosition > 0 )
	{
		int ret;
		unsigned int currentTime = 0;
		// while the whole message is not received,
		// and there isn't any error or the only error is that the socket would be blocking because there is not yet enough data provided
		// and timeout has not yet occured,
		// then we sleep for a second and start receiving again
		do
		{
			ret = recv(this->clientSocket, this->buffer+this->bufferPosition, sizeToReceive - this->bufferPosition, 0);
			if ( ret > 0 )
			{
				this->bufferPosition += ret;
			}
		} while( sizeToReceive - this->bufferPosition > 0 && ( ret != -1 || getNetErrno() == EWOULDBLOCK ) && currentTime++ != timeout && this->sleepForTenthSecond() );
		// otherwise, check why did we drop from that loop
		if ( ret == -1 && getNetErrno() == EWOULDBLOCK )
		{
			throw Tnet_TimeoutException();
		}
		else if ( ret == -1 )
		{
			this->sendError("recv");
		}
		else if ( ret == 0 )
		{
			throw Tnet_ConnectionClosedException();
		}
		else if ( sizeToReceive - this->bufferPosition > 0 )
		{
			throw Tnet_TimeoutException();
		}
		this->timeTakenOnLastOperation = currentTime;
	}
	if ( this->returnBuffer )
	{
		delete[] this->returnBuffer;
	}
	this->returnBuffer = new char[sizeToReceive+1];
	memcpy(this->returnBuffer, this->buffer, sizeToReceive );
	this->returnBuffer[sizeToReceive] = '\0';
	this->bufferPosition-= sizeToReceive;
	if ( this->bufferPosition > 0 )
	{
		memcpy(this->buffer, this->buffer+this->bufferPosition, this->bufferPosition);
	}
	return this->returnBuffer;
}

int Tnet_Connection::simpleReceiveInt( unsigned int timeout )
{
	int number;
	const char* ptr = this->simpleReceive(sizeof(int), timeout);
	return ntohl(*((int*)ptr));
}

void Tnet_Connection::simpleSend(std::string message )
{
	unsigned int ret = ::send(this->clientSocket, message.c_str(), message.length(), 0);
	if (ret == -1)
	{
		this->sendError("send");
	}
}

void Tnet_Connection::simpleSend(int number)
{
	const int networkOrder = htonl(number);
	unsigned int ret = ::send(this->clientSocket, reinterpret_cast<const char *>(&networkOrder), sizeof(int), 0);
	if (ret == -1)
	{
		this->sendError("send");
	}
}
