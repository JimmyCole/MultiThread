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
#include <math.h>
#include "OffLine.h"
#include <smmintrin.h> // sse4.2
#include <immintrin.h>   // avx

#define MAXR 16
#define MR 1
#define MRM 0

using namespace std;	

void sse_dna()
{
	if(state_num<17)
	{
		uint8_t current[16] __attribute__ ((aligned (8)));	
		__m128i current_v; 	// S
		__m128i symbol_v; 	// T

		long i;
		int j,k,symbol;

		/* state initialization */
		for(j=0;j<16;j++)
		{
			if(j<enum_state)
				current[j]=(unsigned int)j;  
			else	current[j]=0;
	    	}

		current_v=_mm_load_si128((__m128i*)&current[0]);

	    	for(i = 0; i < len; i++) 			// The iteration process
	    	{
			symbol=input[i];
			symbol_v = _mm_load_si128((__m128i*)&T_dna[symbol][0]);
			current_v=_mm_shuffle_epi8(symbol_v, current_v);

	    		//_mm_store_si128((__m128i*)current,current_v);  // The output function
			//sse_store[i]=current[start_state];
		}

	    	_mm_store_si128((__m128i*)current,current_v);

	    	for(j = 0; j < enum_state; j++)
			cout<<"sse end state "<<(unsigned int)current[j]<<endl;
	}

	else
	{

		int loop_time, total;
		loop_time=MR;
		total=loop_time*16;
		int mask_time = loop_time-2;

		uint8_t current[MAXR] __attribute__ ((aligned (8)));	
		__m128i current_v[MR]; 	// S
		__m128i symbol_v[MR]; 	// T
		__m128i bound_v[MRM]; 	// {15, 31, 47...}
		__m128i sub_v[MRM];   	// {16, 32, 48...}
		__m128i spbound_v;  	// {16}
		__m128i mask[MRM]; 
		__m128i spmask;

		long i;
		int j,k,symbol;

		/* state initialization */
		for(j=0;j<MAXR;j++)
		{
			if(j<enum_state)
				current[j]=(unsigned int)j;  
			else	current[j]=0;
	    	}
		for(j=0;j<loop_time;j++)
			current_v[j]=_mm_load_si128((__m128i*)&current[j*16]);

		/* mask initialization */
		for(j=0;j<MRM;j++)
		{
			bound_v[j]=_mm_set1_epi8((j+1)*16-1); 	//To check whether the state is larger than j*16-1
			sub_v[j]=_mm_set1_epi8((j+1)*16);
		}
		spbound_v=_mm_set1_epi8(16);			//To check whether the state is larger than j*16-1

	    	for(i = 0; i < len; i++) 			// The iteration process
	    	{
			symbol=input[i];
			for(j=0;j<loop_time;j++)
				symbol_v[j] = _mm_load_si128((__m128i*)&T_dna[symbol][j*16]);

			for(k=0;k<loop_time;k++)
			{
				/* calculate the mask */
				spmask=_mm_cmpgt_epi8(spbound_v,current_v[k]);
				for(j=mask_time;j>=0;j--)
				{
					__m128i tempmask;
					mask[j]=_mm_cmpgt_epi8(current_v[k],bound_v[j]); //If larger, all 1

					if(j<mask_time)
					{
						tempmask=_mm_cmpgt_epi8(sub_v[j+1],current_v[k]);
						mask[j]=_mm_and_si128(mask[j],tempmask);
					}				
				}
				current_v[k]=_mm_and_si128(current_v[k],bound_v[0]);
			
				/* shuffle and blend */
				for(j=mask_time;j>=0;j--)
					current_v[k]=_mm_blendv_epi8(current_v[k],_mm_shuffle_epi8(symbol_v[j+1], current_v[k]), mask[j]);			

				current_v[k]=_mm_blendv_epi8(current_v[k],_mm_shuffle_epi8(symbol_v[0], current_v[k]),spmask);

	    			//_mm_store_si128((__m128i*)&current[k*16],current_v[k]);  // The output function
			}
			//sse_store[i]=current[start_state];
		}

		for(j=0;j<loop_time;j++)
	    		_mm_store_si128((__m128i*)&current[j*16],current_v[j]);

	    	for(j = 0; j < enum_state; j++)
			cout<<"sse end state "<<(unsigned int)current[j]<<endl;
	}

}

//--------------------------------------------------------------------------------------------------------------------
void sse_dna_rangecoalescing()
{
	long i;
	int j,k,symbol;

	int loop_time,total,mask_time;
	loop_time=MR;
	total=loop_time*16;
	mask_time = loop_time-2;

	uint8_t current[MAXR] __attribute__ ((aligned (8)));	
	__m128i current_v[MR]; 	// S
	__m128i symbol_v[MR]; 	// T
	__m128i bound_v[MRM]; 	// {15, 31}
        __m128i sub_v[MRM];   	// {16, 32}
        __m128i spbound_v;  	// {16}
	__m128i mask[MRM]; 
        __m128i spmask;

	symbol=input[0];
	for(int j1=0;j1<total;j1++)
	{
		if(j1<enum_state)
			current[j1]=(unsigned int)L[symbol][j1];  
		else	current[j1]=0;
    	}
	sse_rcstore[0]=current[0];

	for(j=0;j<loop_time;j++)
		current_v[j]=_mm_load_si128((__m128i*)&current[j*16]);

	for(j=0;j<loop_time-1;j++)
	{
		bound_v[j]=_mm_set1_epi8((j+1)*16-1); 	//To check whether the state is larger than j*16-1
		sub_v[j]=_mm_set1_epi8((j+1)*16);
	}
	spbound_v=_mm_set1_epi8(16); 			//To check whether the state is larger than j*16-1

	if(MAX_R<=16)
	{
		   	for(i = 1; i < len; i++) 	// The iteration process
    			{
				int rec,record;
				rec=symbol;
				record=NUV[symbol];
        			symbol=input[i];
		
				for(j=0;j<record;j++)
					symbol_v[j] = _mm_load_si128((__m128i*)&T_s[rec][symbol][j*16]);

				for(k=0;k<loop_time;k++)
					current_v[k]=_mm_shuffle_epi8(symbol_v[0], current_v[k]);   
			
				// The output function
				//__m128i temp=_mm_load_si128((__m128i*)&U_bit[symbol][0]);
				//for(j=0;j<loop_time;j++)
					//_mm_store_si128((__m128i*)&current[j*16],_mm_shuffle_epi8(temp, current_v[j]));
					//_mm_store_si128((__m128i*)&current[j*16],current_v[j]);
				//sse_rcstore[i]=U[symbol][current[start_state]];
			}
	}
	
	else if (MAX_R<=32&&MAX_R>16)
	{
		for(i = 1; i < len; i++) // The iteration process
		{
			int rec,record;							
			rec=symbol;
			record=NUV[symbol];
			symbol=input[i];
			 
			for(j=0;j<record;j++)
				 symbol_v[j] = _mm_load_si128((__m128i*)&T_s[rec][symbol][j*16]);			  
				 
			for(k=0;k<loop_time;k++)
			{
				spmask=_mm_cmpgt_epi8(spbound_v,current_v[k]);
				//mask[0]=_mm_cmpgt_epi8(current_v[k],bound_v[0]); //If larger, all 1		

				//current_v[k]=_mm_and_si128(current_v[k],bound_v[0]);

				//current_v[k]=_mm_blendv_epi8(current_v[k],_mm_shuffle_epi8(symbol_v[1], current_v[k]), mask[0]);
				//current_v[k]=_mm_blendv_epi8(current_v[k],_mm_shuffle_epi8(symbol_v[0], current_v[k]),spmask);
				current_v[k]=_mm_blendv_epi8(_mm_shuffle_epi8(symbol_v[1], current_v[k]),_mm_shuffle_epi8(symbol_v[0], current_v[k]),spmask);
			}
		}
	}

	else
	{
		for(i = 1; i < len; i++) // The iteration process
		{
			int rec,record;							
			rec=symbol;
			record=NUV[symbol];
			symbol=input[i];
			 
			for(j=0;j<record;j++)
				 symbol_v[j] = _mm_load_si128((__m128i*)&T_s[rec][symbol][j*16]);			  
				 
			for(k=0;k<loop_time;k++)
			{
				spmask=_mm_cmpgt_epi8(spbound_v,current_v[k]);
				for(j=record-2;j>=0;j--)
				{
					__m128i tempmask;
					mask[j]=_mm_cmpgt_epi8(current_v[k],bound_v[j]); //If larger, all 1		
					
					if(j<record-2)
					{
						tempmask=_mm_cmpgt_epi8(sub_v[j+1],current_v[k]);
						mask[j]=_mm_and_si128(mask[j],tempmask);
					}
				}
				current_v[k]=_mm_and_si128(current_v[k],bound_v[0]);

				for(j=record-2;j>=0;j--)
					current_v[k]=_mm_blendv_epi8(current_v[k],_mm_shuffle_epi8(symbol_v[j+1],     current_v[k]), mask[j]);
				current_v[k]=_mm_blendv_epi8(current_v[k],_mm_shuffle_epi8(symbol_v[0], current_v[    k]),spmask);
			}
		}
	}

	for(j=0;j<loop_time;j++)
    		_mm_store_si128((__m128i*)&current[j*16],current_v[j]);

	for(j=0;j<enum_state;j++)
		current[j]=U[symbol][current[j]];

    	for(j = 0; j < enum_state; j++)
        	cout<<"sse end state "<<(unsigned int)current[j]<<endl;

	delete []U;
	delete []L;
	delete []U_bit;

}
//---------------------------------------------------------------------------------------------------------------------------------------
//There are still some problems in this version (The speedup needs to be improved) 
void sse_dna_convergence()
{
	long i;
	int j,k,symbol;

	uint8_t current[MAXR] __attribute__ ((aligned (8)));	
	__m128i current_v[MR]; 	// S
	__m128i symbol_v[MR]; 	// T
	__m128i bound_v[MRM]; 	// {15, 31}
        __m128i sub_v[MRM];   	// {16, 32}
        __m128i spbound_v;  	// {16}
	__m128i mask[MRM]; 
        __m128i spmask;

	for(j=0;j<MAXR;j++)
	{
		if(j<enum_state)
			current[j]=(unsigned int)j;  
		else	current[j]=0;
    	}

	int loop_time,total,mask_time,register_num;
	if(enum_state%16>0)
		loop_time=enum_state/16+1;
	else loop_time=enum_state/16;
	register_num=loop_time;
	total=loop_time*16;
	mask_time=loop_time-2;

	for(j=0;j<loop_time;j++)
		current_v[j]=_mm_load_si128((__m128i*)&current[j*16]);

	for(j=0;j<loop_time-1;j++)
	{
		bound_v[j]=_mm_set1_epi8((j+1)*16-1); 	//To check whether the state is larger than j*16-1
		sub_v[j]=_mm_set1_epi8((j+1)*16);
	}
	spbound_v=_mm_set1_epi8(16); 			//To check whether the state is larger than j*16-1

	uint8_t factor[MAXR];
	int Acc[MAXR];
	vector <int> vec1;
    	for(i = 0; i < len; i++) 			// The iteration process
    	{
        	symbol=input[i];
		for(j=0;j<MR;j++)
			symbol_v[j] = _mm_load_si128((__m128i*)&T_dna[symbol][j*16]);

		for(k=0;k<loop_time;k++)
		{
			spmask=_mm_cmpgt_epi8(spbound_v,current_v[k]);

			for(j=MRM-1;j>=0;j--)
			{
				__m128i tempmask;
        			mask[j]=_mm_cmpgt_epi8(current_v[k],bound_v[j]); //If larger, all 1

				if(j<MRM-1)
				{
					tempmask=_mm_cmpgt_epi8(sub_v[j+1],current_v[k]);
					mask[j]=_mm_and_si128(mask[j],tempmask);
				}				

			}

			current_v[k]=_mm_and_si128(current_v[k],bound_v[0]);
			
			for(j=MRM-1;j>=0;j--)
				current_v[k]=_mm_blendv_epi8(current_v[k],_mm_shuffle_epi8(symbol_v[j+1], current_v[k]), mask[j]);
			current_v[k]=_mm_blendv_epi8(current_v[k],_mm_shuffle_epi8(symbol_v[0], current_v[k]),spmask);
    		}
		
		/* convergence check */
		if(i==99)
		{	
			
			for(j=0;j<loop_time;j++)
    				_mm_store_si128((__m128i*)&factor[j*16],current_v[j]);	
			for(j=0;j<enum_state;j++)
				if(find(vec1.begin(),vec1.end(),(int)factor[j])==vec1.end())
						vec1.push_back((int)factor[j]);
			
			int vec_length=vec1.size();
			cout<<"convergence length is "<<vec_length<<endl;
			for(int count=0;count<enum_state;count++)
				for(j=0;j<vec_length;j++)
					if((int)factor[count]==vec1[j])		
						Acc[count]=j;

			for(j=0;j<vec_length;j++)
				factor[j]=vec1[j];

			if(vec_length%16>0)
				loop_time=vec_length/16+1;
			else loop_time=vec_length/16;
			
			for(j=0;j<loop_time;j++)
				current_v[j]=_mm_load_si128((__m128i*)&factor[j*16]);	
		}

	}

	for(j=0;j<loop_time;j++)
    		_mm_store_si128((__m128i*)&factor[j*16],current_v[j]);
	for(j = 0; j < enum_state; j++)
		current[j]=factor[Acc[j]];

    	for(j = 0; j < enum_state; j++)
        	cout<<"sse end state "<<(unsigned int)current[j]<<endl;
}
