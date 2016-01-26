#include <iostream>
#include <vector>
#include <cmath>

#include "defs.h"
#include "util.h"

#include <sdsl/rmq_support.hpp>

using namespace std;
using namespace sdsl;

void EWPM ( double ** pattern, unsigned int m, int * text, unsigned int n, int sigma, double z, vector < unsigned int > * Occ )
{
	cout << "Pattern length:" << m << "    Text length:" << n << endl;
	//construct TP'
	unsigned int N = m + n;
	int * tp = new int [N+3];
	for ( unsigned int i = 0; i < n; i++ )
	{
		tp[i] = text[i];
	}

	for ( unsigned int i = 0; i < m; i++ )
	{
		double max = 0;
		tp[i+n] = 0;
		for ( int j = 0; j < sigma; j++ )
		{
			if ( max < pattern[i][j] )
			{
				max = pattern[i][j];
				tp[i+n] = j + 1;
			}
		}
	}

	//compute LCP for TP'
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
		prob *= pattern[i][ tp[i+n]-1 ];
	}

	//Hamming distance and Occurrences
	for ( unsigned int i = 0; i < n - m + 1; i++ )
	{
		int hd = 0;
		vector < int > MisPos;
		unsigned int lp = i;
		unsigned int rp = n;
		while ( lp < n && rp < N )
		{
			unsigned int l = min ( iSA[lp], iSA[rp] );
			unsigned int r = max ( iSA[lp], iSA[rp] );
			unsigned int span = LCP[ rmq ( l+1, r ) ];
			lp += span;
			rp += span;
			if ( rp < N )
			{
				hd += 1;
				MisPos.push_back ( rp - n );
			}
			if ( hd > floor ( log ( z ) / log ( 2 ) ) )
				break;
			lp += 1;
			rp += 1;
		}

		if ( hd > floor ( log ( z ) / log ( 2 ) ) )
			continue;

		double p = prob;
		for ( unsigned int j = 0; j < hd; j++ )
		{
			unsigned int lmis = MisPos[j] + i;
			unsigned int rmis = MisPos[j];
			double pl = pattern[rmis][ tp[lmis]-1 ];
			double pr = pattern[rmis][ tp[rmis+n]-1 ];
			p = p * pl / pr;
		}
		
		if ( p >= 1/z )
			Occ->push_back ( i );
	}
}


