#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <cstdlib>
#include <sys/timeb.h>
#include <ctime>
#include <climits>
#include <iostream>
#include <fstream>
#include <vector>  
#include <algorithm>
#include "OffLine.h"
#include <smmintrin.h> // sse4.2
#include <immintrin.h>   // avx
#include <pthread.h>

#define NUM_THREADS     8

using namespace std;

// Normal speculation version
void* spec1(void *threadid)
{
	int scurrent;
	int avxsymbol;
	long i,j,offset;
	long tid =  (long)threadid;

	if(tid==0)
		scurrent=start_state;
	else
		scurrent=predict_state[tid-1];
	
	offset=charlen*tid;
	for(i=0;i<charlen;i++)
	{
		scurrent=T1[scurrent*symbol_num+input[i+offset]];
		//memcpy(&spec_store1[i*spec_split1], scurrent, sizeof(int) * spec_split1);
	}

	spec_final[tid]=scurrent;
	//cout<<"The final state of thread"<<tid<<" is "<<scurrent<<endl;
	pthread_exit((void*)threadid);

}

void* spec2(void *threadid)
{
	int scurrent;
	int avxsymbol;
	long i,j,offset;
	long tid =  (long)threadid;
	
	if(tid==0)
		scurrent=start_state;
	else
		scurrent=predict_state[tid-1];
	
	offset=charlen*tid;
	for(i=0;i<charlen;i++)
	{
		scurrent=T2[input[i+offset]*state_num+scurrent];
		//memcpy(&spec_store2[i*spec_split1], scurrent, sizeof(int) * spec_split1);
	}

	spec_final[tid]=scurrent;
	//cout<<"The final state of thread "<<tid<<" is "<<scurrent<<endl;
	pthread_exit((void*)threadid);
}

void* spec3(void *threadid)
{
	int scurrent;
	int avxsymbol;
	long i,j,offset;
	long tid =  (long)threadid;

	if(tid==0)
		scurrent=start_state;
	else
		scurrent=predict_state[tid-1];
	
	offset=charlen*tid;
	for(i=0;i<charlen;i++)
	{
		scurrent=T3[scurrent][input[i+offset]];

	}

	spec_final[tid]=scurrent;
	//cout<<"The final state of thread "<<tid<<" is "<<scurrent<<endl;
	pthread_exit((void*)threadid);
}

void* spec4(void *threadid)
{
	int scurrent;
	int avxsymbol;
	long i,j,offset;
	long tid =  (long)threadid;

	if(tid==0)
		scurrent=start_state;
	else
		scurrent=predict_state[tid-1];
	
	offset=charlen*tid;
	for(i=0;i<charlen;i++)
	{
		scurrent=T4[input[i+offset]][scurrent];
		//memcpy(&spec_store4[i*spec_split1], scurrent, sizeof(int) * spec_split1);
	
	}

	spec_final[tid]=scurrent;
	//cout<<"The final state of thread "<<tid<<" is "<<scurrent<<endl;
	pthread_exit((void*)threadid);
}


void spec_avxshuffle()
{
	long i;
	int j,k,symbol1,symbol2;
	uint8_t current[32] __attribute__ ((aligned (8)));
    	__m256i current_v;
    	__m256i symbol_v;    

    	/* Initialize the start states */
    	for(j=0;j<32;j++)
    	{
		if(j<state_num)
			current[j]=(uint8_t)start_state;
            	else if (j>=16&&j<16+state_num)
                    	current[j]=(uint8_t)predict_state[j-16];
		else
		    	current[j]=(uint8_t)0;
    	}
    
    	current_v=_mm256_load_si256((__m256i*)current);
	long bound=len/2;
	int start_state1=start_state;
	int start_state2=start_state+16;

    	for(i = 0; i < bound; i++)
    	{
		//int location1=;//(int)ceil(current[start_state1]/16)*16;
		//int location2=;//(int)ceil(current[start_state2]/16)*16;
		uint8_t *val = (uint8_t*) &current_v;
		int location1=(int(val[0]))&240;
		int location2=(int(val[16]))&240;
		symbol1=input[i];
		symbol2=input[i+bound];

		symbol_v = _mm256_inserti128_si256(_mm256_castsi128_si256(_mm_loadu_si128((__m128i*)&T_dna[symbol1][location1])), _mm_loadu_si128((__m128i*)&T_dna[symbol2][location2]),1);
	    	current_v = _mm256_shuffle_epi8(symbol_v, current_v);

		_mm256_store_si256((__m256i*)current,current_v); 		//output function
		sse_store[i]=location1;
		sse_store[i+bound]=location2;
    	}
    
	_mm256_store_si256((__m256i*)current,current_v);
	for(i=0;i<2;i++)
		cout<<"speculation ending states "<<(unsigned int)current[i*16]<<endl;
    
	//cout<<"ending state: "<<spec_shuffle_store[len]<<endl;
}

