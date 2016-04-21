#include <stdio.h>
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
#include <smmintrin.h> // sse4.2
#include <immintrin.h>   // avx
#include "OffLine.h"

#define unrolltime 2	//the times unrolling 

using namespace std;

// spec avx 1d-state version 
void spec_avx1() 
{
	long i,j,k;

	__m256i scurrent_v;
	__m256i avxsymbol_v;
	__m256i avxmask_v;
	int* scurrent __attribute__ ((aligned (32)));
	scurrent= new int [spec_split2];
	int* current __attribute__ ((aligned (32)));
	current= new int [spec_split2];
	int* avxsymbol __attribute__ ((aligned (32)));
	avxsymbol=new int [spec_split2] ;
	int* addr __attribute__ ((aligned (32)));
	addr=new int [spec_split2];


	avxmask_v = _mm256_set1_epi32(0XFFFFFFFF);
	scurrent[0]=start_state;
	for(i=0;i<spec_split2-1;i++)
		scurrent[i+1]=predict_state[i];

	scurrent_v=_mm256_maskload_epi32 ((int*)scurrent, avxmask_v);

	long bound=len/spec_split2;

	long cal[8];
	for(i=0;i<8;i++)
		cal[i]=i*bound;

	for(i = 0; i < bound; i++)
	{
        	avxsymbol_v=_mm256_set_epi32(input[i+cal[7]],input[i+cal[6]],input[i+cal[5]],input[i+cal[4]],input[i+cal[3]],input[i+cal[2]],input[i+cal[1]],input[i]);

        	__m256i addr_v = _mm256_add_epi32(_mm256_mullo_epi32(scurrent_v, _mm256_set1_epi32(symbol_num)), avxsymbol_v);

		scurrent_v = _mm256_i32gather_epi32 (T1, addr_v, 4);

		//_mm256_store_si256((__m256i*)scurrent, scurrent_v);	//The output function
		//memcpy(&spec_avxstore1[i*spec_split2], scurrent, sizeof(int)*spec_split2); 
    	}

    	_mm256_store_si256((__m256i*)current, scurrent_v);

    	for(i = 0; i < spec_split2; i++)
	{
		cout<<"speculation(avx1) ending state "<<current[i]<<endl;
		spec_final[i]=current[i];
	}
	
	delete []scurrent;
	delete []current;
	delete []avxsymbol;
	delete []addr;
}

// spec avx 1d-symbol version 
void spec_avx2() 
{
	long i,j,k;

	__m256i scurrent_v;
	__m256i avxsymbol_v;
	__m256i avxmask_v;
	int* scurrent __attribute__ ((aligned (32)));
	scurrent= new int [spec_split2];
	int* current __attribute__ ((aligned (32)));
	current= new int [spec_split2];
	int* avxsymbol __attribute__ ((aligned (32)));
	avxsymbol=new int [spec_split2] ;
	int* addr __attribute__ ((aligned (32)));
	addr=new int [spec_split2];
	
	avxmask_v = _mm256_set1_epi32(0XFFFFFFFF);
	scurrent[0]=start_state;	
	for(i=0;i<spec_split2-1;i++)
		scurrent[i+1]=predict_state[i];
	scurrent_v=_mm256_maskload_epi32 ((int*)&scurrent[0], avxmask_v);
	
	long bound=len/spec_split2;

	long cal[8];
	for(i=0;i<8;i++)
		cal[i]=i*bound;

	for(i = 0; i < bound; i++)
	{
        	avxsymbol_v=_mm256_set_epi32(input[i+cal[7]],input[i+cal[6]],input[i+cal[5]],input[i+cal[4]],input[i+cal[3]],input[i+cal[2]],input[i+cal[1]],input[i]);
        	__m256i addr_v = _mm256_add_epi32(_mm256_mullo_epi32(avxsymbol_v, _mm256_set1_epi32(state_num)), scurrent_v);
		scurrent_v = _mm256_i32gather_epi32 (T2, addr_v, 4);
		
		//_mm256_store_si256((__m256i*)scurrent, scurrent_v);	//The output function
		//memcpy(&spec_avxstore2[i*spec_split2], scurrent, sizeof(int)*spec_split2); 
    	}
    	_mm256_store_si256((__m256i*)current, scurrent_v);

    	for(i = 0; i < spec_split2; i++)
        {
		cout<<"speculation(avx2) ending state "<<current[i]<<endl;
		spec_final[i]=current[i];
	}
	
	delete []scurrent;
	delete []current;
	delete []avxsymbol;
	delete []addr;
}


void spec_avx_many1() 
{
	long i,j,k;

	__m256i scurrent_v[2];
	__m256i avxsymbol_v[2];
	__m256i addr_v[2];

	int* scurrent __attribute__ ((aligned (32)));
	scurrent= new int [spec_split2];
	int* current __attribute__ ((aligned (32)));
	current= new int [spec_split2];

	scurrent_v[0] = _mm256_set1_epi32(start_state);

	__m256i avxmask_v = _mm256_set1_epi32(0XFFFFFFFF);
	scurrent[0]=predict_state[0];	
	for(i=0;i<spec_split2-1;i++)
		scurrent[i+1]=i%state_num;
	scurrent_v[1]=_mm256_maskload_epi32 ((int*)&scurrent[0], avxmask_v);
	
	long bound=len/2;

	for(i = 0; i < bound; i++)
	{

        	avxsymbol_v[0]=_mm256_set1_epi32(input[i]);
        	addr_v[0] = _mm256_add_epi32(_mm256_mullo_epi32(scurrent_v[0], _mm256_set1_epi32(symbol_num)), avxsymbol_v[0]);
		scurrent_v[0] = _mm256_i32gather_epi32 (T1, addr_v[0], 4);

        	avxsymbol_v[1]=_mm256_set1_epi32(input[i+bound]);
        	addr_v[1] = _mm256_add_epi32(_mm256_mullo_epi32(scurrent_v[1], _mm256_set1_epi32(symbol_num)), avxsymbol_v[1]);
		scurrent_v[1] = _mm256_i32gather_epi32 (T1, addr_v[1], 4);
    	}
    	_mm256_store_si256((__m256i*)current, scurrent_v[0]);
    	for(i = 0; i < 8; i++)
        	cout<<"speculation(avx2) ending state "<<current[i]<<endl;
	spec_final[0]=current[0];
    	_mm256_store_si256((__m256i*)current, scurrent_v[1]);
    	for(i = 0; i < 8; i++)
        	cout<<"speculation(avx2) ending state "<<current[i]<<endl;
	spec_final[1]=current[0];

	delete []scurrent;
	delete []current;
}


void spec_avx_many2() 
{
	long i,j,k;

	__m256i scurrent_v[2];
	__m256i avxsymbol_v[2];
	__m256i addr_v[2];

	int* scurrent __attribute__ ((aligned (32)));
	scurrent= new int [spec_split2];
	int* current __attribute__ ((aligned (32)));
	current= new int [spec_split2];

	scurrent_v[0] = _mm256_set1_epi32(start_state);

	__m256i avxmask_v = _mm256_set1_epi32(0XFFFFFFFF);
	scurrent[0]=predict_state[0];	
	for(i=0;i<spec_split2-1;i++)
		scurrent[i+1]=i%state_num;
	scurrent_v[1]=_mm256_maskload_epi32 ((int*)&scurrent[0], avxmask_v);
	
	long bound=len/2;

	for(i = 0; i < bound; i++)
	{

        	avxsymbol_v[0]=_mm256_set1_epi32(input[i]);
        	addr_v[0] = _mm256_add_epi32(_mm256_mullo_epi32(avxsymbol_v[0], _mm256_set1_epi32(state_num)), scurrent_v[0]);
		scurrent_v[0] = _mm256_i32gather_epi32 (T2, addr_v[0], 4);

        	avxsymbol_v[1]=_mm256_set1_epi32(input[i+bound]);
        	addr_v[1] = _mm256_add_epi32(_mm256_mullo_epi32(avxsymbol_v[1], _mm256_set1_epi32(state_num)), scurrent_v[1]);
		scurrent_v[1] = _mm256_i32gather_epi32 (T2, addr_v[1], 4);
    	}

    	_mm256_store_si256((__m256i*)current, scurrent_v[0]);
    	for(i = 0; i < 8; i++)
        	cout<<"speculation(avx2) ending state "<<current[i]<<endl;
	spec_final[0]=current[0];
    	_mm256_store_si256((__m256i*)current, scurrent_v[1]);
    	for(i = 0; i < 8; i++)
        	cout<<"speculation(avx2) ending state "<<current[i]<<endl;
	spec_final[1]=current[0];

	delete []scurrent;
	delete []current;
}
