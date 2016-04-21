//
//  ctimer.cpp
//
//  Original C version created by Zhijia Zhao on 12/23/13.
//  Edited by Junqiao Qiu on 04/05/2016
//


#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

unsigned long hash(char *str);

/****************************************************
              Simple Version
 
 1) This version is not thread-safe. It can only work 
    well within a thread.
 2) It doesn't support nested the begin() ande end().
 ****************************************************/

struct timeval start, end;
long mtime, seconds, useconds;


void ctimer_begin()
{
	gettimeofday(&start, NULL);
}


void ctimer_end()
{
    	gettimeofday(&end, NULL);
    
    	seconds  = end.tv_sec  - start.tv_sec;
    	useconds = end.tv_usec - start.tv_usec;
    
    	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    
    	printf("\n\t %ld ms\n", mtime);
}


/****************************************************
         Thread-safe / Nested-safe Version
 ****************************************************/

/* 
 * used for convert a signature (e.g. "lookback time")
 * to a hash value.
 */
unsigned long hash(char *str)
{
    	unsigned long hash = 5381;
    	int c;
    
    	while ((c = *str++))
        	hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    
    	return hash;
}

/*
 * used to store the start timestamps for a thread
 */
struct start_element_struct 
{
    	unsigned long hash_v;
    	struct timeval start;
    	struct start_element_struct *next;
} ;

typedef struct start_element_struct element;



/*
 * insert a start timestamp to the list.
 */
void insert(element *head, element *el) 
{
    	element *tmp = head->next;
    	head->next = el;
    	el->next = tmp;
}

/* 
 * delete an element 
 */
void delete_h(element *head, element *el) 
{
    	element *i, *j;
    	for (i = head, j = i->next; i->next != NULL; i = i->next, j = i->next) 
	{
		if(j->hash_v == el->hash_v) 
		{
            		i->next = j->next;
            		free(j);
            		break;
        	}
    	}
}

/* 
 * find the timestamp for a given hash value 
 */
element * search(element *head, unsigned long hash_v) 
{
    	element *i;
    	for (i = head; i != NULL; i = i->next) 
	{
        	if (i->hash_v == hash_v) 
		{
            		return i;
        	}
    	}
    	fprintf(stderr, "doesn't find the start timestamp.");
    	return NULL;
}

/* the list of timestamps lists */
element *heads;


/* 
 * malloc the memory for the heads array, since the # of threads
 * are unknown. using array other than linked list avoids 
 * synchronization.
 */
void ctimer_init_sig(int threads_num) 
{
    	heads =(element*) malloc(sizeof(element) * (threads_num + 1));
}

/* 
 * start a ctimer for a thread with a specific signature
 */
void ctimer_begin_sig(int tid, char* signature)
{
    	element *el =(element*) malloc(sizeof(element));
    	struct timeval start_sig;
    	gettimeofday(&start_sig, NULL);
    	el->start = start_sig;
    	el->hash_v = hash(signature);

    	insert(&heads[tid], el);
}

/*
 * stop a ctimer for a thread with a specific signature
 * and print out the elapsed time accordingly.
 */
long ctimer_end_sig(int tid, char* signature)
{
    	element *el = search(&heads[tid], hash(signature));
    	struct timeval *start_sig = &el->start;
    
    	struct timeval end_sig;
    	gettimeofday(&end_sig, NULL);
     
    	seconds  = end_sig.tv_sec  - start_sig->tv_sec;
    	useconds = end_sig.tv_usec - start_sig->tv_usec;
    
    	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    	printf("\n%s(tid %d): %ld ms\n", signature, tid, mtime);
    
    	delete_h(&heads[tid], el);
    
    	return mtime;
}
