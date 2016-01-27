#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>

#include "defs.h"

using namespace std;

int main (int argc, char **argv)
{
	TSwitch sw;
	string alphabet = DNA;
	unsigned int sigma = alphabet.size();
	int mod;
	string pattern_file;
	string text_file;
	string output;
	double z;
	int * x;				//solid string
	double ** y;			//weighted string
	unsigned int m;			//length of pattern 
	unsigned int n;			//length of text
	unsigned int num_Occ;
	vector < unsigned int > Occ;
	unsigned int column;
	unsigned int row;
	ofstream result;

	clock_t start, finish;

	unsigned int k;

	/* Decodes the arguments */
	k = decode_switches ( argc, argv, &sw );

	/* Check the arguments */
	if ( k < 7 )
	{
		usage();
		return 1;
	}
	else
	{
		if ( sw.mod != 1 && sw.mod != 2 )
		{
			cout << "Erroe: Mode(-m) incorrect!" << endl;
			return 0;
		}
		else
		{
			mod = sw.mod;
		}

		if ( sw.pattern_file_name.size() == 0 )
		{
			cout << "Error: No Pattern input!" << endl;
			return 0;
		}
		else
		{
			pattern_file = sw.pattern_file_name;
		}

		if ( sw.text_file_name.size() == 0 )
		{
			cout << "Error: No Text input!" << endl;
			return 0;
		}
		else
		{
			text_file = sw.text_file_name;
		}

		if ( sw.output_file_name.size() == 0 )
		{
			output = "MatchingReport";		
		}
		else
		{
			output = sw.output_file_name;
		}

		if ( sw.z > 0 )
		{
			z = sw.z;
		}
		else 
		{
			cout << "Error: z must be a position integer!" << endl;
		}
	}
	/* read input Weighted String */
	ifstream fpattern ( pattern_file );
	ifstream ftext ( text_file );
	if ( fpattern.fail() )
	{
		cout << "Error: Cannot open pattern file!" << endl;
		return 0;
	}
	else if ( ftext.fail() )
	{
		cout << "Error: Cannot open text file!" << endl;
	}
	else
	{
		if ( mod == 1 )
		{
			/* EWPM */
			string tstr;
			ftext >> tstr;
			n = tstr.length();
			x = new int [n];
			for ( unsigned int i = 0; i < n; i++ )
				x[i] = alphabet.find ( tstr[i] ) + 1;

			double tprob;
			unsigned int counter = 0;
			while ( !fpattern.eof () )
			{
				fpattern >> tprob;
				counter ++;
			}
			fpattern.clear();
			fpattern.seekg( 0, ios::beg );
			m = counter / sigma;
			y = new double * [m];
			for ( unsigned int i = 0; i < m; i++ )
				y[i] = new double [sigma];
			for ( unsigned int i = 0; i < m; i++ )
				for ( unsigned int j = 0; j < sigma; j++ )
					fpattern >> y[i][j];
		}
		else if ( mod == 2 )
		{
			/* EWTM */
			string tstr;
			fpattern >> tstr;
			m = tstr.length();
			x = new int [m];
			for ( unsigned int i = 0; i < m; i++ )
				x[i] = alphabet.find( tstr[i] ) + 1;

			double tprob;
			unsigned int counter = 0;
			while ( !ftext.eof() )
			{
				ftext >> tprob;
				counter ++;
			}
			ftext.clear();
			ftext.seekg( 0, ios::beg );
			n = counter / sigma;
			y = new double * [n];
			for ( unsigned int i = 0; i < n; i++ )
				y[i] = new double [sigma];
			for ( unsigned int i = 0; i < n; i++ )
				for ( unsigned int j = 0; j < sigma; j++ )
					ftext >> y[i][j];
		}
	}
	fpattern.close();
	ftext.close();
	start = clock();
	if ( mod == 1 )
	{
		EWPM ( y, m, x, n, sigma, z, &Occ );
	}
	if ( mod == 2 )
	{
		EWTM ( x, m, y, n, sigma, z, &Occ );
	}
	num_Occ = Occ.size();
	finish = clock();

	cout << "Number of Occurrences: " << num_Occ << endl;
#if 1
	result.open ( output );
	if ( num_Occ == 0 )
	{
		result << "No Occurrences is found.\n";
	}
	else
	{
		result << "Position of Occurrences:\n";
		for ( unsigned int i = 0; i < num_Occ; i++ )
			result << Occ[i] << '\n';
	}
#endif
	double passtime = ( ( double ) finish - start ) / CLOCKS_PER_SEC;

	cout << "time: " << passtime << "s" << endl;

#if 0	
	result.open	( "esm.dat", ios::app );
	result << m << "\t" << passtime << endl;
	result.close();
#endif

	delete[] x;
	if ( mod == 1 )
	{
		for ( unsigned int i = 0; i < m; i++ )
			delete[] y[i];
		delete[] y;
	}

	if ( mod == 2 )
	{
		for ( unsigned int i = 0; i < n; i++ )
			delete[] y[i];
		delete[] y;
	}

	return 0;
}
