#pragma once

#define TRCON_BUFSIZE 16384
#define TRCON_IDLEN 4
#define TRCON_SLACIDLEN 9
#define TRCON_IPLEN 15

#include<array/array.h>
#include "yae.h"



enum trcon_exception 
{
	TRCON_ARRAY,		// int id with number of fail from t::array class (or maybe its draft)
	TRCON_ATON, 		// char* ip  from which we tried to read ip
	TRCON_SOCKET, 		// errno
	TRCON_CONNECT,		// errno
	TRCON_SEND,			// errno
	TRCON_CLOSE,		// errno
	TRCON_SELECT,		// errno
	TRCON_TIMEOUT,		// void
	TRCON_MAXPACKETS, 	// t::array<char>
	TRCON_PARSEID,		// t::array<char>
	TRCON_PARSEIP,		// t::array<char>, char* ip
	TRCON_IDTOOBIG,		// t::array<char>
	TRCON_WRONGRCON,	// void
	TRCON_PBPREFIX,		// void
	TRCON_NOSLAC,		// void
	TRCON_NOETPRO,		// void
	TRCON_NOPB			// void
};

int rcon( server_data* server, player_data* &players, int fails, char* server_name );
void ar2str( char* dest, t::array<char> src, int buflen );

class TrconException
{
	public:
		TrconException( trcon_exception _type ): type(_type),command("") 
		{
			packet[packet.get_size()] = 'n';
			packet[packet.get_size()] = '/';
			packet[packet.get_size()] = 'a';
		}
		const char* command;
		enum trcon_exception type;
		t::array<char> packet;
		const char* getTypeAsString()
		{
			switch(type)
			{
				case TRCON_ARRAY:		return "TRCON_ARRAY";
				case TRCON_ATON:		return "TRCON_ATON";
				case TRCON_SOCKET:		return "TRCON_SOCKET";
				case TRCON_CONNECT:		return "TRCON_CONNECT";
				case TRCON_SEND:		return "TRCON_SEND";
				case TRCON_CLOSE:		return "TRCON_CLOSE";
				case TRCON_SELECT:		return "TRCON_SELECT";
				case TRCON_TIMEOUT:		return "TRCON_TIMEOUT";
				case TRCON_MAXPACKETS:	return "TRCON_MAXPACKETS";
				case TRCON_PARSEID:		return "TRCON_PARSEID";
				case TRCON_PARSEIP:		return "TRCON_PARSEIP";
				case TRCON_IDTOOBIG:	return "TRCON_IDTOOBIG";
				case TRCON_WRONGRCON:	return "TRCON_WRONGRCON";
				case TRCON_PBPREFIX:	return "TRCON_PBPREFIX";
				case TRCON_NOSLAC:		return "TRCON_NOSLAC";
				case TRCON_NOETPRO:		return "TRCON_NOETPRO";
				case TRCON_NOPB:		return "TRCON_NOPB";
				default:				return "UNKNOWN";
			}
		}
};

class TrconParseException : public TrconException
{
	public:
		TrconParseException(trcon_exception _type, t::array<char> _additionaldata ): TrconException(_type), additionaldata(_additionaldata) {}
		TrconParseException(trcon_exception _type, const char* str ): TrconException(_type)
		{
			for( int c=0; str[c] != '\0'; c++ )
			{
				additionaldata[c] = str[c];
			}
		}
		t::array<char> additionaldata;
};

class TrconSocketException : public TrconException
{
	public:
		TrconSocketException(trcon_exception _type, int _errno): TrconException(_type), error_number(_errno) {}
		int error_number;
};

class TrconTimeoutException : public TrconException
{
	public:
		TrconTimeoutException(): TrconException(TRCON_TIMEOUT) {}
};

class TrconPasswordException : public TrconException
{
	public:
		TrconPasswordException(): TrconException(TRCON_WRONGRCON) {}
};

class TrconIpException : public TrconException
{
	public:
		TrconIpException( const char* _ip, trcon_exception _type=TRCON_ATON ): TrconException(_type)
		{
			for( int c=0; _ip[c] != '\0'; c++ )
			{
				ip[c] =_ip[c];
			}
		}
		t::array<char> ip;
};

class TrconParseIpException: public TrconIpException, public TrconParseException
{
	public:
		TrconParseIpException( const char* _ip, t::array<char> _additionaldata )
			:TrconIpException(_ip, TRCON_PARSEIP), TrconParseException(TRCON_PARSEIP,_additionaldata) {}
};

class TrconArrayException: public TrconException
{
	public:
		TrconArrayException( int _id, t::array<char> _packet ): TrconException(TRCON_ARRAY), id(_id)
		{
			packet = _packet;
		}
		int id;
};
