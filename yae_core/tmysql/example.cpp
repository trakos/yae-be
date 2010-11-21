#include "tmysql.h"
#include <stdio.h>

tmysql sql("localhost", "excello", "ihnyy24pn24ytpny4pni", "excello_excello");

int main()
{
//	tmysql sql("localhost", "excello", "ihnyy24pn24ytpny4pni", "excello_excello");
	selectFields sf(5);
	sf.getCurrentSize();
	sf.add(BOOLEAN, "active", "act");
	sf.add(STRING, "avatar", "avt");
	sf.add(STRING, "username", "login");
	sf.add(INT, "numlogins", "nl");
	selectResult& sr = sql.select(sf, "login", "LIMIT 12187");
	for( int i = 0; i<12187; i++ )
	{
		if( sr[i][1] )
		{
			printf("%s\n", sr[i][1]);
		}
		if( sr[i][3] )
		{
			printf("%d\n", *((int*)sr[i][3]));
		}
	}
	sql.freeResult(sr);
}
