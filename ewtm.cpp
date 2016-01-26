#include <iostream>
#include <vector>
#include <cmath>

#include "defs.h"
#include "util.h"

#include <sdsl/rmq_support.hpp>

using namespace std;
using namespace sdsl;

void EWTM ( int * pattern, unsigned int m, double ** text, unsigned int n, int sigma, double z, vector < unsigned int > * Occ )
{
	cout << "Pattern length:" << m << "    Text length:" << n << endl;
	//construct TP
	unsigned int N = m + n;
	int * tp = new int [N+3];
	for ( unsigned int i = 0; i < n; i++ )
	{
		double max = 0;
		tp[i] = 0;
		for ( int j = 0; j < sigma; j++ )
		{
			if ( max < text[i][j] )
			{
				max = text[i][j];
				tp[i] = j + 1;
			}
		}
	}

	for ( unsigned int i = n; i < N; i++ )
	{
		tp[i] = pattern[i - n];
	}

	//compute LCP for T'P
	tp[N] = tp[N+1] = tp[N+2] = 0;
	
	int * SA	= new int [N+3];
	int * iSA	= new int [N];
	int * LCP	= new int [N];

	SA[N] = SA[N+1] = SA[N+2] = 0;

	suffixArray ( tp, SA, N, sigma );

	for ( unsigned int i = 0; i < N; i++ )
		iSA[ SA[i] ] = i;

	LCParray ( tp, N, SA, iSA, LCP );

	vector < int > v ( N, 0 );
	for ( unsigned int i = 0; i < N; i++ )
		v[i] = LCP[i];

	rmq_succinct_sct<> rmq ( &v );

	double prob = 1;
	for ( unsigned int i = 0; i < m; i++ )
	{
		prob *= text[i][ tp[i]-1 ];
	}

	//Hamming distance and Occurrences
	for ( unsigned int i = 0; i < n - m + 1; i++ )
	{
		int hd = 0;					//Hamming distance
		vector < int > MisPos;			//Mismatches in Hamming distance
		unsigned int lp = i;
		unsigned int rp = n;
		while ( lp < n && rp < N )
		{
			unsigned int l = min ( iSA[lp], iSA[rp] );
			unsigned int r = max ( iSA[lp], iSA[rp] );
			unsigned int span = LCP[ rmq ( l + 1, r ) ];
			lp += span;
			rp += span;
			if ( rp < N )
			{
				hd += 1;
				MisPos.push_back ( lp );
			}
			if ( hd > floor ( log ( z ) / log ( 2 ) ) )
				break;
			lp += 1;
			rp += 1;
		}

		if ( hd > floor ( log ( z ) / log ( 2 ) ) )
		{
			if ( i < n - m )
			{
				prob = prob / text[i][ tp[i]-1 ];
				prob = prob	* text[i+m][ tp[i+m]-1 ];
			}
			continue;
		}

		double p = prob;
		for ( unsigned int j = 0; j < hd; j++ )
		{
			unsigned int lmis = MisPos[j];
			unsigned int rmis = MisPos[j] + n - i;
			double	pl = text[lmis][ tp[lmis]-1 ];
			double	pr = text[lmis][ tp[rmis]-1 ];
			p = p * pr / pl;
		}

		if ( p >= 1 / z )
			Occ->push_back ( i );
		if ( i < n - m )
		{
			prob = prob / text[i][ tp[i]-1 ];
			prob = prob	* text[i+m][ tp[i+m]-1 ];
		}
	}
}

