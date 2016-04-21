#ifndef __OFFLINE_H__
#define __OFFLINE_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <cstdlib>
#include <sys/timeb.h>
#include <unistd.h>
#include <ctime>
#include <climits>
#include <iostream>
#include <fstream>
#include <vector>  
#include <algorithm>
#include <smmintrin.h> 		// sse4.2
#include <immintrin.h>   	// avx

using namespace std;

#define unrolltime 2	//the times unrolling 

//-------------The global variables for the seq and normal sse versions-------------------

// input params 
int 	state_num;	// number of states
int 	symbol_num;	// number of symbols
int 	enum_state;	// number of enumerative states (equal to symbol_num)
int 	spec_split1;	// number of species for spec version
int 	spec_split2;	// number of species for spec_avx version
int   	start_state;	// the start state
long 	len;  		// input array length
int 	KIND;		// the mode chosen

// runtime data 
int*	input __attribute__ ((aligned (32)));

// transition table for seq version
int*	T1 __attribute__ ((aligned (32)));  // dfa table 1d-state
int*	T2 __attribute__ ((aligned (32)));  // dfa table 1d-symbol
int**	T3 __attribute__ ((aligned (32)));  // dfa table 2d-state
int**	T4 __attribute__ ((aligned (32)));  // dfa table 2d-symbol

// parameter for spec
int*	predict_state;

// parameters for sse
uint8_t** 	T_dna __attribute__ ((aligned (8)));
uint8_t***	T_s;
int** 		U; // The unique state arrays  
uint8_t** 	U_bit;
int** 		L; // The look_up arrays 
int* 		NU; // The number of unique states for each column
int* 		NUV; //The correspoding 16-bit register used
int 		MAX_R=0;

// pthread parameter
int charlen;

void showmanu()
{
	cout<<endl;
	cout<<"Please Input 9 parameters as required..."<<endl<<endl;
	cout<<"-----------INPUT FORMAT-----------------"<<endl;
	cout<<"./progm---path2table---path2input---#STATE---#SYMBOL"<<endl;
	cout<<"---START_STATE---TOKEN_TYPE---ALGORITHM_VERSION"<<endl<<endl;
	sleep(1);
}

//-------------Some instructions dealing with global variables for seq version------------
// load dfa table from a file wiht 2D-state major file
// load for T1 
void load_dfa_table1(char* dfafile1)
{
	int i,j;
	ifstream dfafile;
	dfafile.open(dfafile1);

	T1= new int [state_num*symbol_num];

	for(i=0;i<state_num;i++)
		for(j=0;j<symbol_num;j++)
		{
			int temp;
			dfafile>>temp;
			if(temp==(-1))
				temp=start_state;
			T1[i*symbol_num+j]=temp;
		}
	dfafile.close();
}

// lod for T2
void load_dfa_table2(char* dfafile1)
{
	int i,j;
	ifstream dfafile;
	dfafile.open(dfafile1);

	T2 = new int [symbol_num*state_num];

	for(i=0;i<state_num;i++)
		for(j=0;j<symbol_num;j++)
		{
			int temp;
			dfafile>>temp;
			if(temp==(-1))
				temp=start_state;
			T2[i+j*state_num]=temp;
		}
	dfafile.close();
}

// load for T3
void load_dfa_table3(char* dfafile1)
{
	int i,j;
	ifstream dfafile;
	dfafile.open(dfafile1);

	T3 = new int* [state_num];
	for(i=0;i<state_num;i++)
		T3[i]=new int [symbol_num];

	for(i=0;i<state_num;i++)
		for(j=0;j<symbol_num;j++)
		{
			int temp;
			dfafile>>temp;
			if(temp==(-1))
				temp=start_state;
			T3[i][j]=temp;
		}								
	dfafile.close();
}

// load for T4
void load_dfa_table4(char* dfafile1)
{
	int i,j;
	ifstream dfafile;
	dfafile.open(dfafile1);

	T4 = new int* [symbol_num];
	for(i=0;i<symbol_num;i++)
		T4[i]=new int [state_num];

	for(i=0;i<state_num;i++)
		for(j=0;j<symbol_num;j++)
		{
			int temp;
			dfafile>>temp;
			if(temp==(-1))
				temp=start_state;
			T4[j][i]=temp;
		}
	dfafile.close();
}

// load input to memory 
void load_input(char* inputfile) 
{
	ifstream in;
	string str;
	long i=0;

	in.open(inputfile);
	len=0;
	while(getline(in,str))
	{
		len=str.size()+len;
	}
	in.close();

	in.open(inputfile);
	input = new int [len];	
	while(in)
	{
		char chara;
		in>>chara;

		//---------------For different benchmark-----------------------
		//DIV
		if(KIND==0)
		{
			if( chara == '0' )
				input[i] = 0;
			else if ( chara == '1' )
				input[i] = 1;
		}
		// SNORT
		else if(KIND==1)
		{
			input[i]=int(chara);
			if(input[i]>255||input[i]<0)
				i--;
		}
		//DNA
		else if(KIND==2)
		{
			if( chara == 'A' )
				input[i] = 0;
			else if ( chara == 'T' )
				input[i] = 1;
			else if ( chara == 'C' )
				input[i] = 2;
			else if ( chara == 'G' )
				input[i] = 3;
		}		
		// PROTN
		else if(KIND==3)
		{
			if( chara == 'A' )
				input[i] = 0;
			else if ( chara == 'C' )
				input[i] = 1;
			else if ( chara == 'D' )
				input[i] = 2;
			else if ( chara == 'E' )
				input[i] = 3;
			else if ( chara == 'F' )
				input[i] = 4;
			else if ( chara == 'G' )
				input[i] = 5;
			else if ( chara == 'H' )
				input[i] = 6;
			else if ( chara == 'I' )
				input[i] = 7;
			else if ( chara == 'K' )
				input[i] = 8;
			else if ( chara == 'L' )
				input[i] = 9;
			else if ( chara == 'M' )
				input[i] = 10;
			else if ( chara == 'N' )
				input[i] = 11;
			else if ( chara == 'P' )
				input[i] = 12;
			else if ( chara == 'Q' )
				input[i] = 13;
			else if ( chara == 'R' )
				input[i] = 14;
			else if ( chara == 'S' )
				input[i] = 15;
			else if ( chara == 'T' )
				input[i] = 16;
			else if ( chara == 'V' )
				input[i] = 17;
			else if ( chara == 'W' )
				input[i] = 18;
			else if ( chara == 'Y' )
				input[i] = 19;
		}
		// EVENODD
		else if (KIND==4)
		{
			if( chara == 'a' )
				input[i] = 0;
			else if ( chara == 'b' )
				input[i] = 1;
			else if ( chara == 'c' )
				input[i] = 2;
			else if ( chara == 'd' )
				input[i] = 3;
		}
		//---------------------------------------------------
		i++;
	}
	in.close();
}

// Constructing the dfa table with elements are 8-bits large 
void get_dfa()
{
	int i,j;
	T_dna=(uint8_t **)malloc(sizeof(uint8_t *) * symbol_num);
    	for(i = 0; i < symbol_num; i++)
         	*(T_dna + i) = (uint8_t *)malloc(sizeof(uint8_t) * state_num); 
	for(i=0;i<symbol_num;i++)
        	for(j=0;j < state_num;j++)
               		T_dna[i][j]=T4[i][j];
}

//--------Instructions for completing the variables used in conv and randcoales versions-----------
	
// The offline work for range coalescing 
void OfflineWork()
{	
	cout<<"Offline work."<<endl; 
	int i,j,k,l;
	vector <int> vec;

	// Getting the Unique arrays and look_up arrays
	U=new int* [symbol_num];
	U_bit= (uint8_t**)malloc(sizeof(uint8_t*) * symbol_num);
	L=new int* [symbol_num];
	NU=new int [symbol_num];
	NUV=new int [symbol_num];

	for(i=0;i<symbol_num;i++)
	{
		for(j=0;j<state_num;j++)	
			if(find(vec.begin(),vec.end(),T4[i][j])==vec.end())
				vec.push_back(T4[i][j]);

		int sizeofu;
		sizeofu=vec.size();
		U[i]=new int [sizeofu];
		*(U_bit+i)=(uint8_t*)malloc(sizeof(uint8_t) * sizeofu);
		L[i]=new int [state_num];
		NU[i]=sizeofu;

		if(MAX_R<sizeofu)	
			MAX_R=sizeofu;

		if(NU[i]%16>0)
			NUV[i]=NU[i]/16+1;
		else NUV[i]=NU[i]/16;

		for(j=0;j<sizeofu;j++)
			U[i][j]=vec[j];

		for(j=0;j<state_num;j++)
			for(k=0;k<sizeofu;k++)
				if(T4[i][j]==U[i][k])
					L[i][j]=U[i][k];

		vec.erase(vec.begin(),vec.end());
	}		

	// Constructing the small transition table 
	T_s=(uint8_t ***)malloc(sizeof(uint8_t **) * symbol_num);
    	for(i = 0; i < symbol_num; i++)
         	T_s[i] = (uint8_t **)malloc(sizeof(uint8_t*) * symbol_num); 
	for(i = 0; i < symbol_num; i++)
		for(j = 0; j < symbol_num; j++)
			T_s[i][j]= (uint8_t *)malloc(sizeof(uint8_t) * NU[i]);

    	for(i = 0; i < symbol_num; i++)
		for(j = 0; j < symbol_num; j++)
			for(k = 0; k < NU[i]; k++)
				for(l = 0; l < NU[j]; l++)
					if(T4[j][U[i][k]]==U[j][l])
						T_s[i][j][k]=(uint8_t)l;
	cout<<"Max range is "<<MAX_R<<endl;
}

//--------------SPEC version Offline------------------------

int* intputlayout(int* a, int chunk)
{
	int* b;
	b=new int [len];
	long bound=len/chunk;

	for(long i=0;i<bound;i++)
		for(int j=0;j<chunk;j++)
			b[i*chunk+j]=a[j*bound+i];
	delete []a;
	return b;
}

// Simple predict with no look-back
void predict(int spec_split)		// construct the predict state
{
	int i,j;
	predict_state=new int [spec_split-1];
	int* count=new int [state_num];
	for(i=0;i<state_num;i++)
		count[i]=0;

	for(i=0;i<state_num;i++)
		for(j=0;j<symbol_num;j++)
		{
			count[T3[i][j]]++;
		}
	int max=0;
	int f=start_state;
	for(i=0;i<state_num;i++)
	{
		if(count[i]>max)
		{
			max=count[i];
			f=i;
		}
	}

	for(i=0;i<spec_split-1;i++)
		predict_state[i]=f;
	cout<<"Predict state is "<<f<<endl;
	delete []count;
}

// A little complex predict with lookback
void complex_predict(int situation, int chunk)
{
	int i,j;
	if(situation==1)		// when nonSIMD spec
	{
		predict_state=new int [chunk-1];

		for (j=0;j<chunk-1;j++)
		{
			int look=len*(j+1)/chunk;
			int ite=start_state;
			for(i=0;i<1000;i++)
			{
				int symbol=input[look-1000+i];
				ite=T3[ite][symbol];
			}
			predict_state[j]=ite;
		}
	}
	if(situation==2)		// when nonSIMD spec
	{
		predict_state=new int [chunk-1];

		for (j=0;j<chunk-1;j++)
		{
			int look=len*(j+1)/chunk;
			int ite=start_state;
			for(i=0;i<1000;i++)
			{
				int symbol=input[look-1000+i];
				ite=T3[ite][symbol];
			}
			if(j%2==0)
				predict_state[j/2]=ite;
			if(j%2!=0)
				predict_state[chunk/2+(j-1)/2]=ite;
		}
	}
	if(situation==3)
	{
		predict_state=new int [chunk-1];

		for (j=0;j<chunk-1;j++)
		{
			int look=len*(j+1)/chunk;
			int ite=start_state;
			for(i=0;i<1000;i++)
			{
				int symbol=input[look-1000+i];
				ite=T3[ite][symbol];
			}
			predict_state[j]=ite;
		}
	}
}
//
//--------------------------RE-COMPUTE-------------------------------
void recompute(int spec_split, int* spec)
{
	int i,j,k,temp;

	// Calculate the recompute length 
        int length_recompute[spec_split-1];
        for(i=0;i<spec_split-1;i++)
                length_recompute[i]=0;
	long bound=len/spec_split;

	for(i=0;i<spec_split-1;i++)
	{
	        k=spec[(i+1)*bound-1];
		if(k!=predict_state[i])
		{
			j=0;
			temp=input[(i+1)*bound+j];
	                k=T4[temp][k];
			while(k!=spec[(i+1)*bound+j]&&j<bound)
			{
				spec[(i+1)*bound+j]=k;
				j++;
                                length_recompute[i]++;
			        temp=input[(i+1)*bound+j];
	                        k=T4[temp][k];
                        }
                 }
         }
         for(i = 0; i < spec_split-1; i++)
             printf("recomputing length: %d\n", length_recompute[i]);
}

void NO_Recompute(int situation, int chunk, int* aspec_final)
{
	if (situation==1)		// nonSIMD spec
	{
		for (int i=0;i<chunk-1;i++)
		{
			if (predict_state[i]!=aspec_final[i])
			{

				int ite=aspec_final[i];
				int slocation=len*(i+1)/chunk;
				for(long j=0;j<len/chunk;j++)
					ite=T3[ite][input[slocation+j]];
				aspec_final[i+1]=ite;
			}
		}
	}
	if (situation==2)		// nonSIMD spec
	{
		int* temp1;
		int* temp2;
		temp1=new int [chunk-1];
		temp2=new int [chunk-1];
		for (int k=0;k<chunk-1;k++)
		{
			if(k<chunk/2)
			{
				temp1[k]=predict_state[k*2];
				temp2[k]=aspec_final[k*2];
			}
			else
			{
				temp1[k]=predict_state[2*k-chunk+1];
				temp2[k]=aspec_final[2*k-chunk+1];
			}

		}
		memcpy(predict_state, temp1, sizeof(int)*(chunk-1));
		memcpy(aspec_final, temp2, sizeof(int)*(chunk-1));

		for (int i=0;i<chunk-1;i++)
		{
			if (predict_state[i]!=aspec_final[i])
			{
				int ite=aspec_final[i];
				int slocation=len*(i+1)/chunk;
				for(long j=0;j<len/chunk;j++)
					ite=T3[ite][input[slocation+j]];
				aspec_final[i+1]=ite;
			}
		}
		delete []temp1;
		delete []temp2;
	}
	if (situation==3)		// nonSIMD spec
	{
		for (int i=0;i<chunk-1;i++)
		{
			if (predict_state[i]!=aspec_final[i]&&aspec_final[i]>spec_split2-2)
			{
				int ite=aspec_final[i];
				int slocation=len*(i+1)/chunk;
				for(long j=0;j<len/chunk;j++)
					ite=T3[ite][input[slocation+j]];
				aspec_final[i+1]=ite;
			}
		}
	}
}



// This function is used for output layout
int* layout(int* a)
{
	int* b;
	b=new int [len];
	long bound=len/spec_split1;
	long* c1=new long [spec_split1];
	for(int c2=0;c2<spec_split1;c2++)
		c1[c2]=c2*bound;

	for(int i=0;i<spec_split1;i++)
		for(long j=0;j<bound;j++)
			b[c1[i]+j]=a[j*spec_split1+i];

	delete []a;
	return b;
}

// Store the results in each iteration 
//---------------The global variables for storing results in eahc iteration--------------

// The seq version
int* seq_store1;
int* seq_store2;
int* seq_store3;
int* seq_store4;
int* seq_shuffle_store;

// The spec version
int* spec_store1 __attribute__ ((aligned (32)));
int* spec_store2 __attribute__ ((aligned (32)));
int* spec_store3 __attribute__ ((aligned (32)));
int* spec_store4 __attribute__ ((aligned (32)));

int* spec_avxstore1 __attribute__ ((aligned (32)));
int* spec_avxstore2 __attribute__ ((aligned (32)));

int* spec_avx_unroll_store1 __attribute__ ((aligned (32)));
int* spec_avx_unroll_store2 __attribute__ ((aligned (32)));
int* spec_unroll_avx_store1 __attribute__ ((aligned (32)));
int* spec_unroll_avx_store2 __attribute__ ((aligned (32)));

int* spec_final;

// The sse version
uint8_t* sse_store;
uint8_t* sse_rcstore;
uint8_t* sse_constore;

#endif
