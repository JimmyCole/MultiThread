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

// Speculation avx vectorization version wiht unrolling
void* spec_avx_unroll1(void *threadid) 
{
	long i,j ;
	int k;	
	long tid =  (long)threadid;

	__m256i current_v;
	__m256i symbol_v;
        __m256i addr_v;
	__m256i mask;

	int* current __attribute__ ((aligned (32)));
	current= new int [spec_split2];
	int* scurrent __attribute__ ((aligned (32)));
	scurrent= new int [spec_split2];
	int* symbol __attribute__ ((aligned (32)));
	symbol=new int [spec_split2] ;
	int* addr __attribute__ ((aligned (32)));
	addr=new int [spec_split2];

	mask = _mm256_set1_epi32(0XFFFFFFFF);
	current[0]=start_state;
	for(i=0;i<spec_split2-1;i++)
		current[i+1]=predict_state[i];
	
	current_v=_mm256_maskload_epi32 ((int*)current, mask);

	long bound = len/(unrolltime*spec_split2);
	int cal[8];
	for(j=0;j<8;j++)
		cal[j]=(tid)*bound+j*(len/spec_split2);

	for(i = 0; i < bound; i++)
	{
        	symbol_v=_mm256_set_epi32(input[i+cal[7]],input[i+cal[6]],input[i+cal[5]],input[i+cal[4]],input[i+cal[3]],input[i+cal[2]],input[i+cal[1]],input[i+cal[0]]);

           	addr_v = _mm256_add_epi32(_mm256_mullo_epi32(current_v, _mm256_set1_epi32(symbol_num)), symbol_v);
            	current_v = _mm256_i32gather_epi32 (T1, addr_v, 4);

		//_mm256_store_si256((__m256i*)current, current_v[k]);	//The output function
		//memcpy(&spec_avx_unroll_store1[i*spec_split2+k*bound], current, sizeof(int)*spec_split2); 
    	}

        _mm256_store_si256((__m256i*)current, current_v);
        for(i = 0; i < spec_split2; i++)
	{        		
		cout<<"ending state (avx+unroll) in the "<<tid<<" thread : "<<current[i]<<endl;
		spec_final[tid*spec_split2+i]=current[i];
	}
    	

	delete []current;
	delete []scurrent;
	delete []symbol;
	delete []addr;
	pthread_exit((void*)threadid);
}


// Speculation avx vectorization version wiht unrolling
void* spec_avx_unroll2(void *threadid) 
{
	long i,j ;
	int k;	
	long tid =  (long)threadid;

	__m256i current_v;
	__m256i symbol_v;
        __m256i addr_v;
	__m256i mask;

	int* current __attribute__ ((aligned (32)));
	current= new int [spec_split2];
	int* scurrent __attribute__ ((aligned (32)));
	scurrent= new int [spec_split2];
	int* symbol __attribute__ ((aligned (32)));
	symbol=new int [spec_split2] ;
	int* addr __attribute__ ((aligned (32)));
	addr=new int [spec_split2];

	mask = _mm256_set1_epi32(0XFFFFFFFF);
	current[0]=start_state;
	for(i=0;i<spec_split2-1;i++)
		current[i+1]=predict_state[i];
	
	current_v=_mm256_maskload_epi32 ((int*)current, mask);

	long bound = len/(unrolltime*spec_split2);
	int cal[8];
	for(j=0;j<8;j++)
		cal[j]=(tid)*bound+j*(len/spec_split2);

	for(i = 0; i < bound; i++)
	{
        	symbol_v=_mm256_set_epi32(input[i+cal[7]],input[i+cal[6]],input[i+cal[5]],input[i+cal[4]],input[i+cal[3]],input[i+cal[2]],input[i+cal[1]],input[i+cal[0]]);

           	addr_v = _mm256_add_epi32(_mm256_mullo_epi32(symbol_v, _mm256_set1_epi32(state_num)), current_v);
            	current_v = _mm256_i32gather_epi32 (T2, addr_v, 4);

		//_mm256_store_si256((__m256i*)current, current_v[k]);	//The output function
		//memcpy(&spec_avx_unroll_store1[i*spec_split2+k*bound], current, sizeof(int)*spec_split2); 
    	}

        _mm256_store_si256((__m256i*)current, current_v);
        for(i = 0; i < spec_split2; i++)
	{        		
		cout<<"ending state (avx+unroll) in the "<<tid<<" thread : "<<current[i]<<endl;
		spec_final[tid*spec_split2+i]=current[i];
	}
    	

	delete []current;
	delete []scurrent;
	delete []symbol;
	delete []addr;
	pthread_exit((void*)threadid);
}

//----------------------level4 unroll and avx-----------------------------------------
// Speculation avx vectorization version wiht unrolling
void* spec_unroll_avx1(void *threadid) 
{
	long i,j ;
	int k;
	long tid =  (long)threadid;

	__m256i current_v;
	__m256i symbol_v;
        __m256i addr_v;
	__m256i mask;

	int* current __attribute__ ((aligned (32)));
	current= new int [spec_split2];
	int* scurrent __attribute__ ((aligned (32)));
	scurrent= new int [spec_split2];
	int* symbol __attribute__ ((aligned (32)));
	symbol=new int [spec_split2] ;
	int* addr __attribute__ ((aligned (32)));
	addr=new int [spec_split2];

	mask = _mm256_set1_epi32(0XFFFFFFFF);
	current[0]=start_state;
	for(i=0;i<spec_split2-1;i++)
		current[i+1]=predict_state[i];
	

	current_v=_mm256_maskload_epi32 ((int*)current, mask);

	long bound = len/(unrolltime*spec_split2);
	long wall=len/unrolltime;
	int cal[8];

	for(j=0;j<8;j++)
		cal[j]=tid*wall+j*bound;

	for(i = 0; i < bound; i++)
	{
        	symbol_v=_mm256_set_epi32(input[i+cal[7]],input[i+cal[6]],input[i+cal[5]],input[i+cal[4]],input[i+cal[3]],input[i+cal[2]],input[i+cal[1]],input[i+cal[0]]);

            	addr_v = _mm256_add_epi32(_mm256_mullo_epi32(current_v, _mm256_set1_epi32(symbol_num)), symbol_v);
            	current_v = _mm256_i32gather_epi32 (T1, addr_v, 4);

			//_mm256_store_si256((__m256i*)current, current_v[k]);	//The output function
			//memcpy(&spec_unroll_avx_store1[i*spec_split2+k*wall], current, sizeof(int)*spec_split2); 
    	}

        _mm256_store_si256((__m256i*)current, current_v);
        for(i = 0; i < spec_split2; i++)
	{
        	cout<<"ending state (unroll+avx) in the "<<tid<<" part: "<<current[i]<<endl;
		spec_final[i+tid*spec_split2]=current[i];
	}

	delete []current;
	delete []scurrent;
	delete []symbol;
	delete []addr;
	pthread_exit((void*)threadid);
}


// Speculation avx vectorization version wiht unrolling
void* spec_unroll_avx2(void *threadid) 
{
	long i,j ;
	int k;
	long tid =  (long)threadid;

	__m256i current_v;
	__m256i symbol_v;
        __m256i addr_v;
	__m256i mask;

	int* current __attribute__ ((aligned (32)));
	current= new int [spec_split2];
	int* scurrent __attribute__ ((aligned (32)));
	scurrent= new int [spec_split2];
	int* symbol __attribute__ ((aligned (32)));
	symbol=new int [spec_split2] ;
	int* addr __attribute__ ((aligned (32)));
	addr=new int [spec_split2];

	mask = _mm256_set1_epi32(0XFFFFFFFF);
	current[0]=start_state;
	for(i=0;i<spec_split2-1;i++)
		current[i+1]=predict_state[i];
	

	current_v=_mm256_maskload_epi32 ((int*)current, mask);

	long bound = len/(unrolltime*spec_split2);
	long wall=len/unrolltime;
	int cal[8];

	for(j=0;j<8;j++)
		cal[j]=tid*wall+j*bound;

	for(i = 0; i < bound; i++)
	{
        	symbol_v=_mm256_set_epi32(input[i+cal[7]],input[i+cal[6]],input[i+cal[5]],input[i+cal[4]],input[i+cal[3]],input[i+cal[2]],input[i+cal[1]],input[i+cal[0]]);

            	addr_v = _mm256_add_epi32(_mm256_mullo_epi32(symbol_v, _mm256_set1_epi32(state_num)), current_v);
            	current_v = _mm256_i32gather_epi32 (T2, addr_v, 4);

		//_mm256_store_si256((__m256i*)current, current_v[k]);	//The output function
		//memcpy(&spec_unroll_avx_store1[i*spec_split2+k*wall], current, sizeof(int)*spec_split2); 
    	}

        _mm256_store_si256((__m256i*)current, current_v);
        for(i = 0; i < spec_split2; i++)
	{
        	cout<<"ending state (unroll+avx) in the "<<tid<<" part: "<<current[i]<<endl;
		spec_final[i+tid*spec_split2]=current[i];
	}

	delete []current;
	delete []scurrent;
	delete []symbol;
	delete []addr;
	pthread_exit((void*)threadid);
}
