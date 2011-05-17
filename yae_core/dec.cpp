#include <stdio.h>
#include <string.h>
#include <gmp.h> 
#include <gmpxx.h>

mpz_class pow(mpz_class x, mpz_class y)
{
	mpz_class r=1;
	while(y != 0)
	{
		r*=x;
		y--;
	}
	return r;
}

int powi(int x, int y)
{
	int r=1;
	while(y)
	{
		r*=x;
		y--;
	}
	return r;
}

unsigned long int decode( int n, int d, int value )
{
	mpz_class r = pow(value%n,d)%n;
	return r.get_ui();
}

void decipher( const int* text, char* buf )
{
	int n = 22261;
	int d = 5291;
	while( *text != 0 )
	{
		*buf = decode(n,d,*text);
		text++;
		buf++;
	}
	*buf = '\0';
}

int hex2int( char c )
{
	if( c >= '0' && c <= '9' )
	{
		return c-'0';
	}
	else if( c >= 'A' && c <= 'F' )
	{
		return c-'A'+10;
	}
	else
	{
		return 0;
	}
}

void decode( const char* encoded_text, char* buf )
{
	int len = strlen(encoded_text)/4;
	char num[5];
	int encoded[len+1];
	for( int i=0; i<len; i++ )
	{
		encoded[i] = 0;
		for( int k=0; k<4; k++ )
		{
			encoded[i] += hex2int(encoded_text[i*4+k])*powi(16,3-k);
		}
	}
	encoded[len] = 0;
	//char* buf = new char[len+1];
	decipher( encoded, buf );
	//return buf;
}
