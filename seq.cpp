/* 
 * This files implements four kinds of seq version, including 1d-state, 1d-symbol, 2d-state, 2d-symbol and 2d-symbol-shuffle seq.
 * The output operation is store operation, which means store the current state for each iteration.
 * This file is edited by J.Q..
*/

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <cstdlib>
#include <sys/timeb.h>
#include <ctime>
#include <climits>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>  
#include <algorithm>
#include <smmintrin.h> // sse4.2
#include <immintrin.h>   // avx
#include <math.h>
#include "OffLine.h"

using namespace std;		

//-----------------Different versions--------------------------
//1d-state
void seq1()
{
        long i;
	int current,symbol;
	current=start_state;

	for(i=0;i<len;i++)
	{
		symbol=input[i];
		current=T1[current*symbol_num+symbol];
		//seq_store1[i]=current; 			//Output
	}
	cout<<"Seq1 Final State is "<<current<<endl;
}

//1d-symbol
void seq2()
{
        long i;
	int current,symbol;
	current=start_state;

	for(i=0;i<len;i++)
	{
		symbol=input[i];
		current=T2[symbol*state_num+current];
		//seq_store2[i]=current; 			//Output
	}
	cout<<"Seq Final State is "<<current<<endl;
}

//2d-state
void seq3()
{
        long i;
	int current,symbol;
	current=start_state;

	for(i=0;i<len;i++)
	{
		symbol=input[i];
		current=T3[current][symbol];
		//seq_store3[i]=current; 			//Output
	}
	cout<<"Seq Final State is "<<current<<endl;
}


//2d-symbol
void seq4()
{
        long i;
	int current,symbol;
	current=start_state;
	
	for(i=0;i<len;i++)
	{
		symbol=input[i];
		current=T4[symbol][current];
		//seq_store4[i]=current; 			//Output
	}
	cout<<"Seq Final State is "<<current<<endl;
}

//shuffle-seq which level3 adding
void seq_shuffle()
{
	uint8_t current[16] __attribute__ ((aligned (8)));	
	__m128i current_v; 	// S
	__m128i symbol_v; 	// T

	long i;
	int j,k,symbol;

	// state initialization, but the number of state shoule be less than 256
	for(j=0;j<16;j++)
	{
		current[j]=(unsigned int)start_state;  
    	}

	current_v=_mm_load_si128((__m128i*)&current[0]);

    	for(i = 0; i < len; i++) 			// The iteration process
    	{
		uint8_t *val = (uint8_t*) &current_v;
		int location=(int)(val[start_state]&240);
        	
		symbol=input[i];
		symbol_v = _mm_load_si128((__m128i*)&T_dna[symbol][location]);
		current_v=_mm_shuffle_epi8(symbol_v, current_v);

    		//_mm_store_si128((__m128i*)current,current_v);  // The output function
		//seq_shuffle_store[i]=current[start_state];
	}

    	_mm_store_si128((__m128i*)current,current_v);
        cout<<"spec shuffle end state "<<(unsigned int)current[0]<<endl;
}

