#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <array/array.h>
#include "rcon.h"
#include "yae.h"

struct pb_data
{
	int id;
	char pbguid[PBGUID_LEN+1];
	unsigned int ip;
	pb_os os;
	char nick[NICK_LEN+1];
};

struct etpro_data
{
	int id;
	char etproguid[ETPROGUID_LEN+1];
};

struct status_data
{
	int id;
	char nick[NICK_LEN+1];
	unsigned int ip;
};

struct slac_data
{
	int id;
	char slacnick[SLACNICK_LEN+1];
	unsigned int slacid;
};

using t::array;

void ar2str( char* dest, array<char> src, int buflen )
{
	int c;
	for(c=0; c<src.get_size() && c<buflen; c++ )
	{
		dest[c] = src[c];
	}
	dest[c] = '\0';
}

void res2ar( array<array<array<char> > > result, array<char>& str )
{
	for( int l2=0; l2<result.get_size(); l2++ )
	{
		for( int w=0; w<result[l2].get_size(); w++ )
		{
			str.merge(result[l2][w]);
			str[str.get_size()] = ' ';
		}
		str[str.get_size()] = '\n';
	}
}

void raw_rcon( unsigned int ip, long port, const char* password, const char* command, array<char>& str )
{
	
	int timeout=5,utimeout=0; 		// time before timeouting first packet
	int interval=0,uinterval = 1000000*0.1; // interval between packets
	int maxpackets=20;			// ddos protection
	
	int ret, sock;
	struct sockaddr_in sock_addr;
	char data[TRCON_BUFSIZE];
	/*ret = inet_aton(ip_char, &sock_addr.sin_addr);
	if( ret == 0 )
	{
		throw TrconIpException(ip_char);
	}*/
	sock_addr.sin_addr.s_addr = ip;
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(port);
	memset(sock_addr.sin_zero, 0, sizeof(sock_addr.sin_zero));
	sprintf(data, "\xff\xff\xff\xff" "rcon %s %s", password, command);
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if( sock<0 )
	{
		throw TrconSocketException(TRCON_SOCKET, errno);
	}
	ret = connect(sock, (struct sockaddr*)&sock_addr, sizeof(struct sockaddr_in) );
	if( ret<0 )
	{
		throw TrconSocketException(TRCON_CONNECT, errno);
	}
	ret = send(sock,data,strlen(data)+1,0);
	if ( ret<0 )
	{
		throw TrconSocketException(TRCON_SEND, errno);
	}
	struct timeval tv;
	tv.tv_sec  = timeout;
	tv.tv_usec = utimeout;
	int i=0;
	int packet=0;
	do
	{
		fd_set sockSet;
		FD_ZERO(&sockSet);
		FD_SET(sock, &sockSet);
		ret = select(sock+1, &sockSet, NULL, NULL, &tv);
		if( ret==0 && i==0 )
		{
			throw TrconTimeoutException();
		}
		else if( ret<0 )
		{
			throw TrconSocketException(TRCON_SELECT, errno);
		}
		tv.tv_sec  = interval;
		tv.tv_usec = uinterval;
		if( ret > 0 )
		{
			ret = recv(sock,data,TRCON_BUFSIZE*sizeof(char),0);
			for( int nr=0; nr<ret; nr++ )
			{
				str[i++] = data[nr];
			}
			packet++;
		}		
	} while( ret > 0 && packet<maxpackets );
	if( packet >= maxpackets )
	{
		throw TrconParseException(TRCON_MAXPACKETS, str);
	}
	ret = close(sock);
	if( ret<0 )
	{
		throw TrconSocketException(TRCON_CLOSE, errno);
	}
	if( str.get_size() == 0 )
	{
		throw TrconTimeoutException();
	}
}

void analyze_packet(const array<char>& str, array<array<array<char> > >& result, int prefix_length = 0 )
{
	const char* ffffprint_c = "\xff\xff\xff\xffprint";
	array<char> ffffprint;
	int len=strlen(ffffprint_c);
	for( int w=0; w<len; w++ )
	{
		ffffprint[w] = ffffprint_c[w];
	}
	array<array<char> > lines = str.explode('\n');
	for( int l=0; l<lines.get_size(); l++ )
	{
		result[l] = lines[l].explode(' ');
	}
	for( int l=0; l<result.get_size(); l++ )
	{
		int last = result[l].get_size()-1;
		array<char> tmp = result[l][last];
		array<array<char> > exp = tmp.explode('\xff');
		if( result[l][last] == ffffprint )
		{
			if( last == 0 )
			{
				result.unset(l--);
			}
			else
			{
				result[l].unset(last);
				result[l].merge(result[l+1]);
				result.unset(l+1);
			}
		}
		else if(exp.get_size() > 1 )
		{
			result[l][last] = exp[0];
			result[l].merge(result[l+1]);
			result.unset(l+1);
		}
	}	
	if( result.get_size() == 1 )
	{
		if( result[0].get_size() == 2 )
		{
			array<char> badrcon1;
			badrcon1[0] = 'B'; badrcon1[1] = 'a'; badrcon1[2] = 'd';
			if( result[0][0] == badrcon1 )
			{
				// in theory we should check if ( result[0][1] == "rconpassword." )
				throw TrconPasswordException();
			}
		}
	}
	for( int l=0; l<result.get_size(); l++ )
	{
		for( int k=0; k<prefix_length; k++ )
		{
			result[l][0].unset(0);
			if( result[l][0].get_size() == 0 )
			{
				result[l].unset(0);
				k++; // usuwamy tym samym spacje!
			}
		}
	}
	result.print_r();
}

pb_data* pb_sv_plist( array<array<array<char> > > & result, int& players_count)
{
	players_count = result.get_size()-2;
	//printf("Liczba linii w odpowiedzi na pb_sv_plist (bez ffffprint\\n): %d\n", result.get_size());
	//printf("Liczba graczy: %d\n", players_count);
	if(players_count<0)
	{
		throw TrconParseException(TRCON_NOPB,"");
	}
	else if(players_count==0)
	{
		return 0;
	}
	struct pb_data* players = new pb_data[players_count];
	try
	{
		for( int l=1; l< result.get_size()-1; l++ )
		{
			if( result[l][3][0] == 'I' || result[l][4][0] == 'I' ) // result[l][3] == INIT
			{
				// gracz w trakcie laczenia, nie damy rady sprawdzic systemu, brak guida zapewne itp
				players_count--;
				result.unset(l);
				l--;
				continue;
			}
			// klejenie nicku (jesli mial w sobie spacje, to zostal zexplodowany)
			while( result[l].get_size() > 9 )
			{
				result[l][8][ result[l][8].get_size() ] = ' ';
				result[l][8].merge(result[l][9]);
				result[l].unset(9);
			}
			// usuwanie apostrofow w nicku
			result[l][8].unset(0);
			result[l][8].unset( result[l][8].get_size()-1 );
			// kopiowanie danych
			pb_data& player = players[l-1];
			array<array<char> > tmp;
			char id[TRCON_IDLEN+1];
			ar2str(id, result[l][0], TRCON_IDLEN);
			char* endptr;
			player.id = strtol(id, &endptr, 10);
			if( *endptr != '\0' )
			{
				array<char> str;
				res2ar(result, str);
				char buf[15+TRCON_IDLEN];
				int diff = endptr - id;
				sprintf(buf, "- %d - %s", diff, id);
				int len = strlen(buf);
				int c = str.get_size();
				for( int w=0; w<len; w++ )
				{
					str[c++] = buf[w];
				}
				delete[] players;
				players=0;
				throw TrconParseException(TRCON_PARSEID, str);
			}
			tmp = result[l][1].explode('(');
			ar2str(player.pbguid, tmp[0], PBGUID_LEN);
			// ip
			if(result[l][2][0] == 'b' && result[l][2][1] == 'o' && result[l][2][2] == 't' )
			{
				// bot
				result.unset(l);
				l--;
				players_count--;
				continue;
			}
			tmp = result[l][2].explode(':');
			char ip[TRCON_IPLEN+1];
			ar2str(ip, tmp[0], TRCON_IPLEN);
			if( inet_pton(AF_INET, ip, &player.ip) == 0 )
			{
				array<char> str;
				res2ar(result, str);
				delete[] players;
				players=0;
				throw TrconParseIpException(ip, str);
			}
			if( result[l][7][1] == 'W' )
			{
				player.os = WINDOWS;
			}
			else if( result[l][7][1] == 'L' )
			{
				player.os = LINUX;
			}
			else
			{
				player.os = UNKOWN;
			}
           	ar2str(player.nick, result[l][8], NICK_LEN);
        }
	}
	catch(int e)
	{
		if(players) delete[] players;
		throw;
	}
	/*catch( TrconParseIpException& e )
	{
		if(players) delete[] players;
		throw;
	}
	catch( TrconParseException& e )
	{
		if(players) delete[] players;
		throw;
	}*/
	return players;
}

etpro_data* cheaters( array<array<array<char> > >& result, int& players_count)
{
	players_count = result.get_size()-1;
	if(players_count<0)
	{
		throw TrconParseException(TRCON_NOETPRO,"");
	}
	if(players_count==0)
	{
		return 0;
	}
	etpro_data* players = new etpro_data[players_count];
	try
	{
		for( int l=1; l< result.get_size(); l++ )
		{
			etpro_data& player = players[l-1];
			char id[TRCON_IDLEN+1];
			ar2str(id, result[l][0], TRCON_IDLEN);
			char* endptr;
			player.id = strtol(id, &endptr, 10);
			player.id++; //przyjmujemy numeracje pb
			if( *endptr != '\0' )
			{
				array<char> str;
				res2ar(result, str);
				char buf[15+TRCON_IDLEN];
				int diff = endptr - id;
				sprintf(buf, "- %d - %s", diff, id);
				int len = strlen(buf);
				int c = str.get_size();
				for( int w=0; w<len; w++ )
				{
					str[c++] = buf[w];
				}
				delete[] players;
				throw TrconParseException(TRCON_PARSEID, str);
			}
			result[l][ result[l].get_size()-2 ].unset(0); // usuwamy [ z pola na etproguida, a ] pominiemy kopiujac o jedno za malo (ETPROGUID_LEN)
			ar2str(player.etproguid, result[l][ result[l].get_size()-2 ], ETPROGUID_LEN);
		}
	}
	catch(int e)
	{
		delete[] players;
		throw e;
	}
	catch( TrconParseException& e )
	{
		delete[] players;
		throw e;
	}
	return players;
}

slac_data* sl_sv_listplayers( array<array<array<char> > > & result, int& slac_count )
{
	slac_count = result.get_size()-3;
	if(slac_count<0)
	{
		throw TrconParseException(TRCON_NOSLAC,"");
	}
	if(slac_count==0)
	{
		return 0;
	}
	slac_data* players = new slac_data[slac_count];
	try
	{
		for( int l=2; l<result.get_size()-1; l++ )
		{
			slac_data& player = players[l-2];
			array<array<char> > tmp;
			// id
			char id[TRCON_IDLEN+1];
			ar2str(id, result[l][0], TRCON_IDLEN);
			char* endptr;
			player.id = strtol(id, &endptr, 10);
			if( *endptr != '\0' )
			{
				array<char> str;
				res2ar(result, str);
				char buf[15+TRCON_IDLEN];
				int diff = endptr - id;
				sprintf(buf, "- %d - %s", diff, id);
				int len = strlen(buf);
				int c = str.get_size();
				for( int w=0; w<len; w++ )
				{
					str[c++] = buf[w];
				}
				delete[] players;
				players = 0;
				throw TrconParseException(TRCON_PARSEID, str);
			}
			player.id++; //przyjmujemy numeracje pb
			// slacid
			char slacid[TRCON_SLACIDLEN+1];
			ar2str(slacid, result[l][6], TRCON_SLACIDLEN);
			player.slacid = strtol(slacid, &endptr, 10);
			if( *endptr != '\0' )
			{
				array<char> str;
				res2ar(result, str);
				char buf[15+TRCON_SLACIDLEN];
				int diff = endptr - slacid;
				sprintf(buf, "- %d - %s", diff, slacid);
				int len = strlen(buf);
				int c = str.get_size();
				for( int w=0; w<len; w++ )
				{
					str[c++] = buf[w];
				}
				delete[] players;
				players = 0;
				throw TrconParseException(TRCON_PARSEID, str);
			}
			//slacnick
			ar2str(player.slacnick, result[l][2], SLACNICK_LEN);
		}
	}
	catch(int e)
	{
		if(players) delete[] players;
		throw e;
	}
	catch( TrconParseException& e )
	{
		if(players) delete[] players;
		throw e;
	}
	return players;
}

status_data* rconstatus( array<array<array<char> > > & result, int& players_count)
{
	players_count = result.get_size()-3;
	if(players_count<0)
	{
		// zdarzylo mi sie to tylko przy No rconpassword set on the server.
		throw TrconPasswordException();
	}
	if(players_count==0)
	{
		return 0;
	}
	status_data* players = new status_data[players_count];
	try
	{
		for( int l=3; l<result.get_size(); l++ )
		{
			status_data& player = players[l-3];
			array<array<char> > tmp;
			// ping r�wny ZMBI oznacza jakis dziwny przypadek, ale ze wtedy i tak nie podany jest nick, to nie ma co trzymac takich danych, pomijamy
			if( result[l][2][0] == 'Z' && result[l][2][1] == 'M' && result[l][2][2] == 'B' && result[l][2][3] == 'I')
			{
				result.unset(l);
				l--;
				players_count--;
				continue;
			}
			// id
			char id[TRCON_IDLEN+1];
			ar2str(id, result[l][0], TRCON_IDLEN);
			char* endptr;
			player.id = strtol(id, &endptr, 10);
			player.id++; //przyjmujemy numeracje pb
			if( *endptr != '\0' )
			{
				array<char> str;
				res2ar(result, str);
				char buf[15+TRCON_IDLEN];
				int diff = endptr - id;
				sprintf(buf, "- %d - %s", diff, id);
				int len = strlen(buf);
				int c = str.get_size();
				for( int w=0; w<len; w++ )
				{
					str[c++] = buf[w];
				}
				delete[] players;
				players = 0;
				throw TrconParseException(TRCON_PARSEID, str);
			}
			// klejenie nicku (jesli mial w sobie spacje, to zostal zexplodowany)
			while( result[l].get_size() > 8 )
			{
				result[l][3][ result[l][3].get_size() ] = ' ';
				result[l][3].merge(result[l][4]);
				result[l].unset(4);
			}
			ar2str(player.nick, result[l][3], NICK_LEN);
			// ip
			if( result[l][5][0] == 'b' && result[l][5][1] == 'o' && result[l][5][2] == 't' )
			{
				// bot
				result.unset(l);
				l--;
				players_count--;
				continue;
			}
			tmp = result[l][5].explode(':');
			char ip[TRCON_IPLEN+1];
			ar2str(ip, tmp[0], TRCON_IPLEN);
			printf("%s\n",ip);
			if( inet_pton(AF_INET, ip, &player.ip) == 0 )
			{
				array<char> str;
				res2ar(result, str);
				delete[] players;
				players = 0;
				throw TrconParseIpException(ip, str);
			}
		}
	}
	catch(int e)
	{
		if(players) delete[] players;
		throw;
	}
	catch( TrconParseException& e )
	{
		if(players) delete[] players;
		throw;
	}
	return players;
}

void getDelimiter( array<char>& str )
{
	array<array<char> > tmp = str.explode('\n');
	if( tmp.get_size() != 2 )
	{
		throw TrconParseException(TRCON_PBPREFIX,str);
	}
	str = tmp[1];
	tmp = str.explode('=');
	if( tmp.get_size() != 2 )
	{
		throw TrconParseException(TRCON_PBPREFIX,str); // = w delimicie :(
	}
	str = tmp[1];
	str.unset(0);		// usuwam spacje po =
}

void getName( array<array<array<char> > > result, array<char>& str )
{
	while(result[0].get_size() > 3)
	{
		result[0][1][ result[0][1].get_size() ] = ' ';
		result[0][1].merge(result[0][2]);
		result[0].unset(2);
	}
	str = result[0][1];
	for( int i=0; i<4; i++ )
	{
		str.unset(0); // usuwamy 'is:"'
	}
	str.unset(str.get_size()-1); // usuwamy '"'
}

int rcon( server_data* server, player_data* &players, int fails, char* server_name )
{
	fails = 0;
	players = 0; 						// null
	int players_count = 0;
	int slac_count = 0;
	int pb_count = 0;
	int etpro_count = 0;
	pb_data* pb_players = 0; 			// null
	status_data* status_players = 0;	// null
	etpro_data* etpro_players = 0;		// null
	slac_data* slac_players  = 0;		// null
	
	const char* command = "none";
	array<char> str;
	try
	{
		array<array<array<char> > > result;
		bool is_pb = false;
		bool is_slac = false;
		bool is_etpro = false;
		int id_map[65];
		for( int k=0; k<65; k++ )
		{
			id_map[k] = -1;
		}
		
		command = "sv_hostname";
		raw_rcon(server->ip, server->port, server->rcon_password, "sv_hostname", str);
		analyze_packet( str, result );
		getName( result, str );
		ar2str( server_name, str, SERVERNAME_LEN );
		printf("Nazwa serwera to %s\n", server_name);
		str.clear();
		result.clear();
		usleep(1000000*0.51);
		
		command = "status";
		raw_rcon(server->ip, server->port, server->rcon_password, "status", str);
		analyze_packet( str, result );
		status_players = rconstatus( result, players_count );
		if(players_count==0)
		{
			return 0;
		}
		players = new player_data[players_count];
		for( int p=0; p<players_count; p++ )
		{
			status_data& status_player = status_players[p];
			player_data& player = players[p];
			if( status_player.id > 64 )
			{
				throw TrconParseException(TRCON_IDTOOBIG,str);
			}
			strcpy(player.nick, status_player.nick);
			player.ip = status_player.ip;
			player.etproguid[0] = '\0';
			player.pbguid[0] = '\0';
			player.slacnick[0] = '\0';
			player.slacid = 0;
			player.os = UNKOWN;
			id_map[status_player.id] = p;
			/*//konwersja ip do stringa do wypisania tylko
		    struct in_addr ip; ip.s_addr = status_player.ip;
			char* str = inet_ntoa(ip);
			//wypisanie
			printf("player %d:\n(\n", p);
			printf("\tid: %d\n", status_player.id);
			printf("\tip: %s\n", str);
			printf("\tnick: %s\n", status_player.nick);
			printf(")\n");*/
		}
		delete[] status_players; status_players = 0;
		str.clear();
		result.clear();
		usleep(1000000*0.51);
		
		command = "sl_sv_listplayers";
		try
		{
			raw_rcon(server->ip, server->port, server->rcon_password, "sl_sv_listplayers", str);
			analyze_packet( str, result );
			slac_players = sl_sv_listplayers( result, slac_count );
			for( int p=0; p<slac_count; p++ )
			{
				slac_data& slac_player = slac_players[p];
				/*printf("player %d:\n(\n", p);
				printf("\tid: %d\n", slac_player.id);
				printf("\tslacid: %d\n", slac_player.slacid);
				printf("\tslacnick: %s\n", slac_player.slacnick);
				printf(")\n");*/
				if( slac_player.id > 64 )
				{
					throw TrconParseException(TRCON_IDTOOBIG,str);
				}
				if( id_map[slac_player.id] == -1 )
				{
					fails++;
					continue;
				}
				int id = id_map[slac_player.id];
				player_data& player = players[id];	
				player.slacid = slac_player.slacid;
				player.os = WINDOWS; // UWAGA ! ZAKLADAMY ZE SLAC = WINDOWS!
				strcpy(player.slacnick, slac_player.slacnick);
			}
			is_slac = true;
			delete[] slac_players; slac_players = 0;
		}
		catch(TrconParseException e)
		{
			if( e.type != TRCON_NOSLAC )
			{
				throw;
			}
			printf("slac data not received\n");
		}
		str.clear();
		result.clear();
		usleep(1000000*0.51);
		
		try
		{
			command = "pb_sv_msgprefix";
			raw_rcon(server->ip, server->port, server->rcon_password, "pb_sv_msgprefix", str);
			getDelimiter( str );
			int pb_delim_length = str.get_size() + 1; // dodajemy do dlugosci delimitatora rowniez ":"
			str.clear();
			usleep(1000000*0.51);
			
			command = "pb_sv_plist";
			raw_rcon(server->ip, server->port, server->rcon_password, "pb_sv_plist", str);
			analyze_packet( str, result, pb_delim_length );
			pb_players = pb_sv_plist(result, pb_count);
			if(pb_count<1)
			{
				return 0;
			}
			for( int p=0; p<pb_count; p++ )
			{
				pb_data& pb_player = pb_players[p];
				if( pb_player.id > 64 )
				{
					throw TrconParseException(TRCON_IDTOOBIG,str);
				}
				int id = id_map[pb_player.id];
				if( id == -1 )
				{
					fails++;
					continue;
				}
				player_data& player = players[id];
				strcpy(player.pbguid, pb_player.pbguid);
				strcpy(player.nick, pb_player.nick);
				player.ip = pb_player.ip;
				player.os = pb_player.os;
				/*printf("player %d:\n(\n", p);
				printf("\tid: %d\n", pb_player.id);
				printf("\tpbguid: %s\n", pb_player.pbguid);
				printf("\tip: %d\n", ntohl(pb_player.ip));
				printf("\tos: %d\n", pb_player.os);
				printf("\tnick: %s\n", pb_player.nick);
				printf(")\n");*/
			}
			is_pb = true;
		}
		catch(TrconParseException e)
		{
			if( e.type != TRCON_PBPREFIX && e.type != TRCON_NOPB )
			{
				throw;
			}
			printf("pb data not received\n");
		}
		usleep(1000000*0.51);	// rcon protocol time interval between commands is at least 500 miliseconds
		str.clear();
		result.clear();

		command = "cheaters";
		raw_rcon(server->ip, server->port, server->rcon_password, "cheaters", str);
		analyze_packet( str, result );
		try
		{
			etpro_data* etpro_players = cheaters( result, etpro_count );
			for( int p=0; p<etpro_count; p++ )
			{
				etpro_data& etpro_player = etpro_players[p];
				/*printf("player: %d\n(\n", p);
				printf("\tid: %d\n", etpro_player.id);
				printf("\tetproguid: %s\n", etpro_player.etproguid);
				printf(")\n");*/
				if( etpro_player.id > 64 )
				{
					throw TrconParseException(TRCON_IDTOOBIG,str);
				}
				if( id_map[etpro_player.id] == -1 )
				{
					fails++;
					continue;
				}
				player_data& player = players[ id_map[etpro_player.id] ];
				strcpy(player.etproguid, etpro_player.etproguid);
			}
			is_etpro = true;
			delete[] etpro_players; etpro_players = 0;
		}
		catch(TrconParseException e)
		{
			if( e.type != TRCON_NOETPRO )
			{
				throw;
			}
			printf("etpro data not received\n");
		}
	}
	catch(int e)
	{
		if( etpro_players ) delete[] etpro_players;
		if( slac_players ) delete[] slac_players;
		if( pb_players )delete[] pb_players;
		if( players ) delete[] players;
		etpro_players = 0;
		slac_players = 0;
		pb_players = 0;
		players = 0;
		TrconArrayException ae = TrconArrayException(e,str);
		ae.command = command;
		ae.packet = str;
		throw;
	}
	catch( TrconParseException& e )
	{
		if( etpro_players ) delete[] etpro_players;
		if( slac_players ) delete[] slac_players;
		if( pb_players )delete[] pb_players;
		if( players ) delete[] players;
		etpro_players = 0;
		slac_players = 0;
		pb_players = 0;
		players = 0;
		printf("Exception caught and sent\n");
		e.command = command;
		e.packet = str;
		throw;
	}
	catch( TrconException& e )
	{
		if( etpro_players ) delete[] etpro_players;
		if( slac_players ) delete[] slac_players;
		if( pb_players )delete[] pb_players;
		if( players ) delete[] players;
		etpro_players = 0;
		slac_players = 0;
		pb_players = 0;
		players = 0;
		printf("Exception caught and sent\n");
		e.command = command;
		throw;
	}
	if( etpro_players ) delete[] etpro_players;
	if( slac_players ) delete[] slac_players;
	if( pb_players )delete[] pb_players;
	etpro_players = 0;
	slac_players = 0;
	pb_players = 0;
	for(int p=0;p<players_count;p++)
	{
		player_data& player = players[ p ];
		//konwersja ip do stringa do wypisania tylko
		struct in_addr ip; ip.s_addr = player.ip;
		char* str = inet_ntoa(ip);
		//wypisanie
		printf("player %d:\n(\n", p);
		printf("\tip: %s\n", str);
		printf("\tnick: %s\n", player.nick);
		printf("\tetproguid: %s\n", player.etproguid);
		printf("\tslacid: %d\n", player.slacid);
		printf("\tslacnick: %s\n", player.slacnick);
		printf("\tpbguid: %s\n", player.pbguid);
		printf("\tos(W/L/U): %d\n", player.os);
		printf(")\n");
	}
	return players_count;
}
