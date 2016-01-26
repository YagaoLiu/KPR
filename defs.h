#include <vector>
#include <string>

using namespace std;

#define ALLOC_SIZE		10000
#define DNA				"ACGT"

#ifndef TSWITCH_H
#define TSWITCH_H
struct TSwitch
{
	double	z;
	int		mod;
	string	pattern_file_name;
	string	text_file_name;
	string	output_file_name;
	string	alphabet;
};
#endif


int decode_switches ( int argc, char * argv[], struct TSwitch * sw );
void usage ( void );
unsigned int LCParray ( int * text, unsigned int n, int * SA, int * ISA, int * LCP );
void suffixArray ( int * s, int * SA, int n, int K );
void EWPM ( double ** pattern, unsigned int m, int * text, unsigned int n, int sigma, double z, vector < unsigned int > * Occ );
void EWTM ( int * pattern, unsigned int m, double ** text, unsigned int n, int sigma, double z, vector < unsigned int > * Occ );
