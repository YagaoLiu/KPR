#include <iostream>

using namespace std;

unsigned int LCParray ( int * text, unsigned int n, int * SA, int * ISA, int * LCP )
{
	int i = 0, j = 0;
	
	LCP[0] = 0;
	for ( i = 0; i < n; i++ )
	{
		if ( ISA[i] != 0 )
		{
			if ( i == 0 ) j = 0;
			else j = ( LCP[ ISA[i-1] ] >= 2 ) ? LCP[ ISA[i-1] ] - 1 : 0 ;
			while ( text[i+j] == text[SA[ISA[i]-1]+j] )
				j++;
			LCP[ISA[i]] = j;
		}
	}

	return 1;
}
