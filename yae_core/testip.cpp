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

int main()
{
	unsigned int ipi;
	const char* ips = "212.242.177.10";
	char ip[TRCON_IPLEN+1]; 
	sprintf(ip,ips);
	if( inet_pton(AF_INET, ip, &ipi) == 0 ) 
	{
		throw -1;
	}
	printf("%u\n", ntohl(ipi));
}
