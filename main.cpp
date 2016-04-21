/*
	* This file can be applied to the general benchmarks. But note that, 
	* when the number of state is larger than 256, it will be a little hard to use the sse version.
 
	* The inputs you have to offer are dfa file name, input file name, and
	* number of states, number of symbols, number of species for speculation algorithm, 
	* the start state for seq version, the kind of benchmark, and the mode.
	* edited by J.Q on 04/05/2016.
	* This version is pthread/multithread version. 
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
#include <smmintrin.h> 		// sse4.2
#include <immintrin.h>   	// avx
#include <math.h>
#include <pthread.h>

#include "OffLine.h"
#include "ctimer.cpp"
#include "seq.cpp"
#include "spec_nonSIMD.cpp"
#include "spec_avx.cpp"
#include "spec_avxunroll.cpp"
#include "sse.cpp"

#define NUM_THREADS     8

using namespace std;		

int main(int argv, char* argc[])
{
	char* dfafile;
	char* inputfile;
	ofstream outfile1,outfile2,outfile3,outfile4,outfile5;	// Files which are used to store the time spend

	// Error situation
	if(argv<10)
	{
		showmanu();
		return -1;
	}

	//Initialization
	dfafile=argc[1]; 		// The dfa transtion file name
	inputfile=argc[2]; 		// The input file name
	state_num=atoi(argc[3]);	// The number of states
	symbol_num=atoi(argc[4]);	// The number of symbols
	spec_split1=atoi(argc[5]);	// The number of species (for normal speculation)
	spec_split2=atoi(argc[6]); 	// The number of species (for avx speculation)
	start_state=atoi(argc[7]);	// Start state
	KIND=atoi(argc[8]);		// Choose the kind of benchmark
	int mode=atoi(argc[9]);		// Choose the mode
	enum_state=state_num;		// The number of enumerative states in sse version
	
	//Load operation
        load_input(inputfile);
	load_dfa_table3(dfafile);	// Loading 2D state-major table
	load_dfa_table4(dfafile);	// Loading 2D symbol-major table

	// Determine and show the length of input	
	len=100000000;
	len=(len/(spec_split1*spec_split2))*spec_split1*spec_split2; 
        cout<<endl<<"The input length is "<<len<<endl<<endl;

	// Time varibale
	long cmp1,cmp2,cmp3,cmp4;
	long cmp1s,cmp2s,cmp3s,cmp4s;
	struct timeb startTime1, endTime1;
	struct timeb startTime2, endTime2;
	struct timeb startTime3, endTime3;
	struct timeb startTime4, endTime4;
	struct timeb startTime1s, endTime1s;
	struct timeb startTime2s, endTime2s;
	struct timeb startTime3s, endTime3s;
	struct timeb startTime4s, endTime4s;

	// pthread version variables
	int rc;
   	long t;
   	pthread_t threads[NUM_THREADS];
   	pthread_t threads1[unrolltime];

//----------seq version------------------------------------------
	if (mode==1)
	{
		//--------seq version 1d-state-------------
		load_dfa_table1(dfafile);
		//seq_store1=new int [len];

		// Process
		ftime(&startTime1);
		seq1();		
		ftime(&endTime1);

		// Time spent
		cmp1=(endTime1.time-startTime1.time)*1000+(endTime1.millitm-startTime1.millitm);
		cout<<cmp1<<endl;
		outfile1.open("R1_1.txt", ios::app);
		outfile1 << cmp1 << endl;
		outfile1.close();
		
		//cout<<seq_store1[len/50]<<' '<<seq_store1[len/2]<<endl;
		delete []T1;
		//delete []seq_store1;
		cout<<endl;

		//------seq version 1d-symbol----------------
		load_dfa_table2(dfafile);
		//seq_store2=new int [len];

		// Process
		ftime(&startTime2);
		seq2();
		ftime(&endTime2);

		// Time spent
		cmp2=(endTime2.time-startTime2.time)*1000+(endTime2.millitm-startTime2.millitm);
		cout<<cmp2<<endl;
		outfile2.open("R1_2.txt", ios::app);
		outfile2 << cmp2 << endl;
		outfile2.close();

		//cout<<seq_store2[len/50]<<' '<<seq_store2[len/2]<<endl;
		delete []T2;
		//delete []seq_store2;
		cout<<endl;

		//--------seq version 2d-state------------
		//seq_store3=new int [len];

		// Process
		ftime(&startTime3);
		seq3();
		ftime(&endTime3);

		// Time spent
		cmp3=(endTime3.time-startTime3.time)*1000+(endTime3.millitm-startTime3.millitm);
		cout<<cmp3<<endl;
		outfile3.open("R1_3.txt", ios::app);
		outfile3 << cmp3 << endl;
		outfile3.close();

		//delete []seq_store3;
		cout<<endl;

		//--------------seq version 2d-symbol
		//seq_store4=new int [len];

		// Process
		ftime(&startTime4);
		seq4();
		ftime(&endTime4);

		// Time spent
		cmp4=(endTime4.time-startTime4.time)*1000+(endTime4.millitm-startTime4.millitm);
		cout<<cmp4<<endl;
		outfile4.open("R1_4.txt", ios::app);
		outfile4 << cmp4 << endl;
		outfile4.close();

		//delete []seq_store4;
		cout<<endl;
	}

//---------------------spec nonSIMD version----------------------------------------------
	if(mode==2)
	{
		//predict(spec_split1);
		complex_predict(1,spec_split1);
		for(int cc=0;cc<spec_split1-1;cc++)
			cout<<"Predict state for "<<cc+1<<" chunk is "<<predict_state[cc]<<endl;
		cout<<endl;
		spec_final=new int [spec_split1];

		charlen = len/NUM_THREADS;

		//-------------------nonSIMD spec version 1d-state----------------------
		load_dfa_table1(dfafile);
		//spec_store1=new int [len];

		ftime(&startTime1);
   		for(t=0;t<NUM_THREADS;t++)
		{
     			printf("In main: creating thread %ld\n", t);
     			rc = pthread_create(&threads[t], NULL, spec1, (void *)t);
     			if (rc)
			{
       				printf("ERROR; return code from pthread_create() is %d\n", rc);
       				exit(-1);
       			}
     		}

		for(t=0; t<NUM_THREADS; t++)
    		{
        		pthread_join(threads[t], NULL);
    		}
		ftime(&endTime1);
		
		for (t=0;t<NUM_THREADS;t++)
			cout<<"The final state of thread "<<t<<" is "<<spec_final[t]<<endl;
		
		//spec_store1=layout(spec_store1);	// layout for output
		ftime(&startTime1s);
		//recompute(spec_split1, spec_store1);
		NO_Recompute(1,spec_split1,spec_final);
		ftime(&endTime1s);

		cmp1=(endTime1.time-startTime1.time)*1000+(endTime1.millitm-startTime1.millitm);
		cmp1s=(endTime1s.time-startTime1s.time)*1000+(endTime1s.millitm-startTime1s.millitm);
		cout<<"Total time (ms) "<<cmp1+cmp1s<<endl;
		cout<<"Reprocess time (ms) "<<cmp1s<<endl;
		outfile1.open("R2_1.txt", ios::app);
		outfile1 << cmp1+cmp1s << endl;
		outfile1.close();

		//cout<<spec_store1[len/50]<<' '<<spec_store1[len/2]<<endl;
		delete []T1;
		//delete []spec_store1;
		cout<<endl;

		//-------------------nonSIMD spec version 1d-symbol----------------------
		load_dfa_table2(dfafile);
		//spec_store2=new int [len];

		ftime(&startTime2);
   		for(t=0;t<NUM_THREADS;t++)
		{
     			printf("In main: creating thread %ld\n", t);
     			rc = pthread_create(&threads[t], NULL, spec2, (void *)t);
     			if (rc)
			{
       				printf("ERROR; return code from pthread_create() is %d\n", rc);
       				exit(-1);
       			}
     		}
		for(t=0; t<NUM_THREADS; t++)
    		{
        		pthread_join(threads[t], NULL);
    		}
		ftime(&endTime2);
		for (t=0;t<NUM_THREADS;t++)
			cout<<"The final state of thread "<<t<<" is "<<spec_final[t]<<endl;
		
		//spec_store2=layout(spec_store2);	// layout for output

		ftime(&startTime2s);
		//recompute(spec_split1, spec_store2);
		NO_Recompute(1,spec_split1,spec_final);
		ftime(&endTime2s);

		cmp2=(endTime2.time-startTime2.time)*1000+(endTime2.millitm-startTime2.millitm);
		cmp2s=(endTime2s.time-startTime2s.time)*1000+(endTime2s.millitm-startTime2s.millitm);
		cout<<"Total time (ms) "<<cmp2+cmp2s<<endl;
		cout<<"Reprocess time (ms) "<<cmp2s<<endl;		
		outfile2.open("R2_2.txt", ios::app);
		outfile2 << cmp2+cmp2s << endl;
		outfile2.close();

		delete []T2;
		//delete []spec_store2;
		cout<<endl;

		//------------------nonSIMD seq version 2d-state---------------------------
		load_dfa_table3(dfafile);
		//spec_store3=new int [len];

		ftime(&startTime3);
   		for(t=0;t<NUM_THREADS;t++)
		{
     			printf("In main: creating thread %ld\n", t);
     			rc = pthread_create(&threads[t], NULL, spec3, (void *)t);
     			if (rc)
			{
       				printf("ERROR; return code from pthread_create() is %d\n", rc);
       				exit(-1);
       			}
     		}
		for(t=0; t<NUM_THREADS; t++)
    		{
        		pthread_join(threads[t], NULL);
    		}
		ftime(&endTime3);
		for (t=0;t<NUM_THREADS;t++)
			cout<<"The final state of thread "<<t<<" is "<<spec_final[t]<<endl;

		//spec_store3=layout(spec_store3);	// layout for output

		ftime(&startTime3s);
		//recompute(spec_split1, spec_store3);
		NO_Recompute(1,spec_split1,spec_final);
		ftime(&endTime3s);

		cmp3=(endTime3.time-startTime3.time)*1000+(endTime3.millitm-startTime3.millitm);
		cmp3s=(endTime3s.time-startTime3s.time)*1000+(endTime3s.millitm-startTime3s.millitm);
		cout<<"Total time (ms) "<<cmp3+cmp3s<<endl;
		cout<<"Reprocess time (ms) "<<cmp3s<<endl;
		outfile3.open("R2_3.txt", ios::app);
		outfile3 << cmp3+cmp3s << endl;
		outfile3.close();

		//delete []spec_store3;
		cout<<endl;

		//-----------------nonSIMD seq version 2d-symbol---------------------------
		//load_dfa_table4(dfafile);
		//spec_store4=new int [len];
		
		ftime(&startTime4);
   		for(t=0;t<NUM_THREADS;t++)
		{
     			printf("In main: creating thread %ld\n", t);
     			rc = pthread_create(&threads[t], NULL, spec4, (void *)t);
     			if (rc)
			{
       				printf("ERROR; return code from pthread_create() is %d\n", rc);
       				exit(-1);
       			}
     		}
		for(t=0; t<NUM_THREADS; t++)
    		{
        		pthread_join(threads[t], NULL);
    		}
		ftime(&endTime4);
		for (t=0;t<NUM_THREADS;t++)
			cout<<"The final state of thread "<<t<<" is "<<spec_final[t]<<endl;
		
		//spec_store4=layout(spec_store4);	// layout for output
		ftime(&startTime4s);
		//recompute(spec_split1, spec_store4);
		NO_Recompute(1,spec_split1,spec_final);
		ftime(&endTime4s);

		cmp4=(endTime4.time-startTime4.time)*1000+(endTime4.millitm-startTime4.millitm);
		cmp4s=(endTime4s.time-startTime4s.time)*1000+(endTime4s.millitm-startTime4s.millitm);
		cout<<"Total time (ms) "<<cmp4+cmp4s<<endl;
		cout<<"Reprocess time (ms) "<<cmp4s<<endl;
		outfile4.open("R2_4.txt", ios::app);
		outfile4 << cmp4+cmp4s << endl;
		outfile4.close();

		//delete []spec_store4;
		cout<<endl;

		delete []spec_final;
	}

//-------------------------spec avx-------------------------------
	// AVX speculation version 1d-state
	if(mode==3)
	{
		//predict(spec_split2);
		complex_predict(1,spec_split2);
		for(int cc2=0;cc2<spec_split2-1;cc2++)
			cout<<"Predict state for "<<cc2+1<<" chunk is "<<predict_state[cc2]<<endl;
		spec_final = new int [spec_split2];

		load_dfa_table1(dfafile);
		//spec_avxstore1=new int [len];
		ftime(&startTime1);

		spec_avx1();
		//spec_avxstore1=layout(spec_avxstore1);	// layout for output
		ftime(&endTime1);
		//spec_avxstore1=layout(spec_avxstore1);	// layout for output
		
		ftime(&startTime1s);
		//recompute(spec_split2,spec_avxstore1);
		NO_Recompute(1,spec_split2,spec_final);
		ftime(&endTime1s);
		
		cmp1=(endTime1.time-startTime1.time)*1000+(endTime1.millitm-startTime1.millitm);
		cmp1s=(endTime1s.time-startTime1s.time)*1000+(endTime1s.millitm-startTime1s.millitm);
		cout<<"Total time (ms) "<<cmp1+cmp1s<<endl;
		cout<<"Reprocess time (ms) "<<cmp1s<<endl;
		cout<<endl;
		outfile1.open("R3.txt", ios::app);
		outfile1 << cmp1+cmp1s << endl;
		outfile1.close();

		//delete []spec_avxstore1;
		delete []T1;
		delete []spec_final;
	}

	// AVX speculation version 1d-symbol
	if(mode==4)
	{	
		//predict(spec_split2);
		complex_predict(1,spec_split2);
		for(int cc2=0;cc2<spec_split2-1;cc2++)
			cout<<"Predict state for "<<cc2+1<<" chunk is "<<predict_state[cc2]<<endl;
		spec_final = new int [spec_split2];

		load_dfa_table2(dfafile);
		//spec_avxstore2=new int [len];

		ftime(&startTime1);
		spec_avx2();
		//spec_avxstore2=layout(spec_avxstore2);	// layout for output
		ftime(&endTime1);
		//spec_avxstore2=layout(spec_avxstore2);	// layout for output
		
		ftime(&startTime1s);
		//recompute(spec_split2,spec_avxstore2);
		NO_Recompute(1,spec_split2,spec_final);
		ftime(&endTime1s);

		cmp1=(endTime1.time-startTime1.time)*1000+(endTime1.millitm-startTime1.millitm);
		cmp1s=(endTime1s.time-startTime1s.time)*1000+(endTime1s.millitm-startTime1s.millitm);
		cout<<"Total time (ms) "<<cmp1+cmp1s<<endl;
		cout<<"Reprocess time (ms) "<<cmp1s<<endl;
		cout<<endl;
		outfile1.open("R4.txt", ios::app);
		outfile1 << cmp1+cmp1s << endl;
		outfile1.close();

		//delete []spec_avxstore2;
		delete []T2;
		delete []spec_final;
	}
//-----------------------sse version-------------------------------

	// sse normal version
	if(mode==5)
	{
		get_dfa();
		//sse_store = new uint8_t [len];

		ftime(&startTime1);
		sse_dna();
		ftime(&endTime1);
		
		cmp1=(endTime1.time-startTime1.time)*1000+(endTime1.millitm-startTime1.millitm);
		cout<<"Total time (ms) "<<cmp1<<endl;
		cout<<endl;
		outfile1.open("R5.txt", ios::app);
		outfile1 << cmp1 << endl;
		outfile1.close();

		//cout<<(int)sse_store[len/50]<<' '<<(int)sse_constore[len/2]<<endl;
		//delete []sse_store;
 	}

	// sse with range coalescing version 6
	if(mode==6)
	{
		get_dfa();
		OfflineWork();
		//sse_rcstore = new uint8_t [len];

		ftime(&startTime1);
		sse_dna_rangecoalescing();
		ftime(&endTime1);
		
		cmp1=(endTime1.time-startTime1.time)*1000+(endTime1.millitm-startTime1.millitm);
		cout<<"Total time (ms) "<<cmp1<<endl;
		outfile1.open("R6.txt", ios::app);
		outfile1 << cmp1 << endl;
		outfile1.close();

		//cout<<(int)sse_rcstore[len/50]<<' '<<(int)sse_rcstore[len/2]<<endl;
		//delete []sse_rcstore;
		cout<<endl;
	}
	
	// sse with convergence version  
	if(mode==7) 
	{
		get_dfa();
		//sse_constore=new uint8_t [len];
		
		ftime(&startTime1);
		sse_dna_convergence();
		ftime(&endTime1);
		
		cmp1=(endTime1.time-startTime1.time)*1000+(endTime1.millitm-startTime1.millitm);
		cout<<"Total time (ms) "<<cmp1<<endl;
		cout<<endl;
		outfile1.open("R7.txt", ios::app);
		outfile1 << cmp1 << endl;
		outfile1.close();

		//cout<<(int)sse_constore[len/50]<<' '<<(int)sse_constore[len/2]<<endl;
		//delete []sse_constore;
	} 

//-----------------------avx+unroll version-------------------------------

	// AVX speculation and unrolling version 1d-state
	if(mode==8)
	{
		//predict(spec_split2);
		complex_predict(2,spec_split2*unrolltime);
		for(int cc8=0;cc8<spec_split2*unrolltime-1;cc8++)
			cout<<"Predict state for "<<cc8+1<<" chunk is "<<predict_state[cc8]<<endl;
		spec_final = new int [spec_split2*unrolltime];

		charlen = len/unrolltime;
		load_dfa_table1(dfafile);
		//spec_avx_unroll_store1=new int [len];

		ftime(&startTime1);
   		for(t=0;t<unrolltime;t++)
		{
     			printf("In main: creating thread %ld\n", t);
     			rc = pthread_create(&threads1[t], NULL, spec_avx_unroll1, (void *)t);
     			if (rc)
			{
       				printf("ERROR; return code from pthread_create() is %d\n", rc);
       				exit(-1);
       			}
     		}
		for(t=0; t<unrolltime; t++)
    		{
        		pthread_join(threads1[t], NULL);
    		}
		ftime(&endTime1);
		//spec_avxstore1=layout(spec_avxstore1);	// layout for output
		
		ftime(&startTime1s);
		//recompute_unroll(spec_split2,spec_avx_unroll_store1);	// recompute
		NO_Recompute(2, spec_split2*unrolltime, spec_final);
		ftime(&endTime1s);

		cmp1=(endTime1.time-startTime1.time)*1000+(endTime1.millitm-startTime1.millitm);
		cmp1s=(endTime1.time-startTime1.time)*1000+(endTime1s.millitm-startTime1s.millitm);
		cout<<"Total time (ms) "<<cmp1+cmp1s<<endl;
		cout<<"Reprocess time (ms) "<<cmp1s<<endl;
		cout<<endl;
		outfile1.open("R8.txt", ios::app);
		outfile1 << cmp1+cmp1s << endl;
		outfile1.close();

		//cout<<spec_avx_unroll_store1[len/50]<<' '<<spec_avx_unroll_store1[len/2]<<endl;
		//delete []spec_avx_unroll_store1;
		delete []T1;
		delete []spec_final;
	}

	// AVX speculation and unrolling version 1d-symbol
	if(mode==9)
	{
		//predict(spec_split2);
		complex_predict(2,spec_split2*unrolltime);
		for(int cc9=0;cc9<spec_split2*unrolltime-1;cc9++)
			cout<<"Predict state for "<<cc9+1<<" chunk is "<<predict_state[cc9]<<endl;
		spec_final = new int [spec_split2*unrolltime];

		charlen = len/unrolltime;
		load_dfa_table2(dfafile);
		//spec_avx_unroll_store2=new int [len];

		ftime(&startTime1);
   		for(t=0;t<unrolltime;t++)
		{
     			printf("In main: creating thread %ld\n", t);
     			rc = pthread_create(&threads1[t], NULL, spec_avx_unroll2, (void *)t);
     			if (rc)
			{
       				printf("ERROR; return code from pthread_create() is %d\n", rc);
       				exit(-1);
       			}
     		}
		for(t=0; t<unrolltime; t++)
    		{
        		pthread_join(threads1[t], NULL);
    		}
		ftime(&endTime1);
		//spec_avxstore1=layout(spec_avxstore1);	// layout for output
		
		ftime(&startTime1s);
		//recompute_unroll(spec_split2,spec_avx_unroll_store2);	// recompute
		NO_Recompute(2, spec_split2*unrolltime, spec_final);
		ftime(&endTime1s);

		cmp1=(endTime1.time-startTime1.time)*1000+(endTime1.millitm-startTime1.millitm);
		cmp1s=(endTime1s.time-startTime1s.time)*1000+(endTime1s.millitm-startTime1s.millitm);
		cout<<"Total time (ms) "<<cmp1+cmp1s<<endl;
		cout<<"Reprocess time (ms) "<<cmp1s<<endl;
		cout<<endl;
		outfile1.open("R9.txt", ios::app);
		outfile1 << cmp1+cmp1s << endl;
		outfile1.close();

		//cout<<spec_avx_unroll_store2[len/33]<<spec_avx_unroll_store2[len/2]<<endl;
		//delete []spec_avx_unroll_store2;
		delete []T2;
		delete []spec_final;
	}

	// Unrolling and AVX speculation version 1d-state
	if(mode==10)
	{
		//predict(spec_split2);
		complex_predict(1,spec_split2*unrolltime);
		for(int cc10=0;cc10<spec_split2*unrolltime-1;cc10++)
			cout<<"Predict state for "<<cc10+1<<" chunk is "<<predict_state[cc10]<<endl;
		spec_final = new int [spec_split2*unrolltime];

		charlen = len/unrolltime;

		load_dfa_table1(dfafile);
		//spec_unroll_avx_store1=new int [len];

		ftime(&startTime1);
   		for(t=0;t<unrolltime;t++)
		{
     			printf("In main: creating thread %ld\n", t);
     			rc = pthread_create(&threads1[t], NULL, spec_unroll_avx1, (void *)t);
     			if (rc)
			{
       				printf("ERROR; return code from pthread_create() is %d\n", rc);
       				exit(-1);
       			}
     		}
		for(t=0; t<unrolltime; t++)
    		{
        		pthread_join(threads1[t], NULL);
    		}
		ftime(&endTime1);

		//spec_avxstore1=layout(spec_avxstore1);	// layout for output
		
		ftime(&startTime1s);
		//recompute_unroll(spec_split2,spec_unroll_avx_store1);	// recompute
		NO_Recompute(1, spec_split2*unrolltime, spec_final);
		ftime(&endTime1s);

		cmp1=(endTime1.time-startTime1.time)*1000+(endTime1.millitm-startTime1.millitm);
		cmp1s=(endTime1s.time-startTime1s.time)*1000+(endTime1s.millitm-startTime1s.millitm);
		cout<<"Total time (ms) "<<cmp1+cmp1s<<endl;
		cout<<"Reprocess time (ms) "<<cmp1s<<endl;
		cout<<endl;
		outfile1.open("R10.txt", ios::app);
		outfile1 << cmp1+cmp1s << endl;
		outfile1.close();
		
		//cout<<spec_unroll_avx_store1[len/50]<<' '<<spec_unroll_avx_store1[len/2]<<endl;
		//delete []spec_unroll_avx_store1;
		delete []T1;
		delete []spec_final;
	}

	// Unrolling and AVX speculation version 1d-symbol
	if(mode==11)
	{
		//predict(spec_split2);
		complex_predict(1,spec_split2*unrolltime);
		for(int cc11=0;cc11<spec_split2*unrolltime-1;cc11++)
			cout<<"Predict state for "<<cc11+1<<" chunk is "<<predict_state[cc11]<<endl;
		spec_final = new int [spec_split2*unrolltime];

		charlen = len/unrolltime;

		load_dfa_table2(dfafile);
		//spec_unroll_avx_store2=new int [len];
		
		ftime(&startTime1);
   		for(t=0;t<unrolltime;t++)
		{
     			printf("In main: creating thread %ld\n", t);
     			rc = pthread_create(&threads1[t], NULL, spec_unroll_avx2, (void *)t);
     			if (rc)
			{
       				printf("ERROR; return code from pthread_create() is %d\n", rc);
       				exit(-1);
       			}
     		}
		for(t=0; t<unrolltime; t++)
    		{
        		pthread_join(threads1[t], NULL);
    		}
		ftime(&endTime1);
		//spec_avxstore1=layout(spec_avxstore1);	// layout for output
		
		ftime(&startTime1s);
		//recompute_unroll(spec_split2,spec_unroll_avx_store2);	// recompute
		NO_Recompute(1, spec_split2*unrolltime, spec_final);
		ftime(&endTime1s);
		
		cmp1=(endTime1.time-startTime1.time)*1000+(endTime1.millitm-startTime1.millitm);
		cmp1s=(endTime1s.time-startTime1s.time)*1000+(endTime1s.millitm-startTime1s.millitm);
		cout<<"Total time (ms) "<<cmp1+cmp1s<<endl;
		cout<<"Reprocess time (ms) "<<cmp1s<<endl;
		cout<<endl;
		outfile1.open("R11.txt", ios::app);
		outfile1 << cmp1+cmp1s << endl;
		outfile1.close();

		//cout<<spec_unroll_avx_store2[len/50]<<' '<<spec_unroll_avx_store2[len/2]<<endl;
		//delete []spec_unroll_avx_store2;
		delete []T2;
		delete []spec_final;
	}

	// AVX speculation version 1d-state
	if(mode==13)
	{
		//predict(spec_split2);
		complex_predict(1,unrolltime);
		for(int cc13=0;cc13<unrolltime-1;cc13++)
			cout<<"Predict state for "<<cc13+1<<" chunk is "<<predict_state[cc13]<<endl;
		spec_final = new int [unrolltime];

		load_dfa_table1(dfafile);
		//spec_avxstore1=new int [len];
		
		ftime(&startTime1);
		spec_avx_many1();
		//spec_avxstore1=layout(spec_avxstore1);	// layout for output
		ftime(&endTime1);
		//spec_avxstore1=layout(spec_avxstore1);	// layout for output
		
		ftime(&startTime1s);
		//recompute(spec_split2,spec_avxstore1);
		NO_Recompute(3, unrolltime, spec_final);
		ftime(&endTime1s);
		
		cmp1=(endTime1.time-startTime1.time)*1000+(endTime1.millitm-startTime1.millitm);
		cmp1s=(endTime1s.time-startTime1s.time)*1000+(endTime1s.millitm-startTime1s.millitm);
		cout<<"Total time (ms) "<<cmp1+cmp1s<<endl;
		cout<<"Reprocess time (ms) "<<cmp1s<<endl;
		cout<<endl;
		outfile1.open("R13.txt", ios::app);
		outfile1 << cmp1+cmp1s << endl;
		outfile1.close();

		//delete []spec_avxstore1;
		delete []T1;
		delete []spec_final;
	}

	// AVX speculation version 1d-symbol
	if(mode==14)
	{	
		//predict(spec_split2);
		complex_predict(1,unrolltime);
		for(int cc14=0;cc14<unrolltime-1;cc14++)
			cout<<"Predict state for "<<cc14+1<<" chunk is "<<predict_state[cc14]<<endl;
		spec_final = new int [unrolltime];

		load_dfa_table2(dfafile);
		//spec_avxstore2=new int [len];

		ftime(&startTime1);
		spec_avx_many2();
		//spec_avxstore2=layout(spec_avxstore2);	// layout for output
		ftime(&endTime1);
		//spec_avxstore2=layout(spec_avxstore2);	// layout for output
		
		ftime(&startTime1s);
		//recompute(spec_split2,spec_avxstore2);
		NO_Recompute(3, unrolltime, spec_final);
		ftime(&endTime1s);

		cmp1=(endTime1.time-startTime1.time)*1000+(endTime1.millitm-startTime1.millitm);
		cmp1s=(endTime1s.time-startTime1s.time)*1000+(endTime1s.millitm-startTime1s.millitm);
		cout<<"Total time (ms) "<<cmp1+cmp1s<<endl;
		cout<<"Reprocess time (ms) "<<cmp1s<<endl;
		cout<<endl;
		outfile1.open("R14.txt", ios::app);
		outfile1 << cmp1+cmp1s << endl;
		outfile1.close();

		//delete []spec_avxstore2;
		delete []T2;
		delete []spec_final;
	}

	// sse normal version
	if(mode==15)
	{
		get_dfa();
		predict(spec_split2);
		long cmp18;
		//sse_store = new uint8_t [len];

		ftime(&startTime1);
		spec_avxshuffle();
		ftime(&endTime1);

		cmp1=(endTime1.time-startTime1.time)*1000+(endTime1.millitm-startTime1.millitm);
		cout<<cmp1<<endl;
		cout<<endl;
		outfile1.open("R15.txt", ios::app);
		outfile1 << cmp1 << endl;
		outfile1.close();

		//cout<<(int)sse_store[len/50]<<' '<<(int)sse_store[len/2]<<endl;
		//delete []sse_store;
 	}

	return 0;
}
