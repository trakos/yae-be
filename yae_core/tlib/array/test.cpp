#include "array.h"
#include <stdio.h>

using t;

int main()
{
	array<int> a(1,0);
	a.set_size(5);
	a[0]=0;a[1]=1;a[2]=2;a[3]=3;a[4]=4;a[5]=5; // new size - 10
	a.print_r();
	a.get(5);
	array< array<int> > res = a.explode(3);
	res.print_r();
	return a.get(5);	
}
