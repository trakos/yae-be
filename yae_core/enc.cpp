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

unsigned long int encode( int n, int  e, int value )
{
	mpz_class r = pow(value,e)%n;
	return r.get_ui();
}

void cipher( const char* text, int* buf )
{
	int n = 22261;
	int e = 195;
	while( *text != '\0' )
	{
		*buf = encode(n,e,(int)*text);
		text++;
		buf++;
	}
	*buf = 0;
}

int main(int argc, char** argv)
{
	if( argc != 2 )
	{
		return 1;
	}
	const char* text = argv[1];
	int encoded[strlen(text)+1];
	cipher( text, encoded );
	int* texti = encoded;
	while( *texti != 0 )
	{
		printf("%.4X", *texti);
		//printf("%d\n", *texti);
		texti++;
	}
	//printf("\n");
}

