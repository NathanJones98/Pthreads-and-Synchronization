#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "hash.h"

#include "pthread.h"

#define SAMPLES_TO_COLLECT   10000000
#define RAND_NUM_UPPER_BOUND   100000
#define NUM_SEED_STREAMS            4

/* 
 * ECE454 Students: 
 * Please fill in the following team struct 
 */
team_t team = {
    "Ok",                  /* Team name */

    "Nathan Jones",                    /* Member full name */
    "1005003023",                 /* Member student number */
    "nathan.jones@mail.utoronto.ca",                 /* Member email address */
};

unsigned num_threads;
unsigned samples_to_skip;

class sample;

class sample {
  unsigned my_key;
 public:
  sample *next;
  unsigned count;

  sample(unsigned the_key){my_key = the_key; count = 0;};
  unsigned key(){return my_key;}
  void print(FILE *f){printf("%d %d\n",my_key,count);}
};

// This instantiates an empty hash table
// it is a C++ template, which means we define the types for
// the element and key value here: element is "class sample" and
// key value is "unsigned".  
hash<sample,unsigned> h;

/*
* Function which does all seeding. The function is used by each thread and 
* splices the work done depending on the total number of threads
*/
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int counter=0;


void *seed(void *vargp) {
  
  
  int thread_id =  *((int*) vargp);
  int i,j,k;
  int rnum;
  unsigned key;
  sample *s;
  //printf("Self: %lu Thread: %d Mod 2: %d\n", (int unsigned long)pthread_self(), thread_id, mod2);
  int from, to;

  
  if(num_threads == 1){
    from = 0; //first 
    to = NUM_SEED_STREAMS-1; //last
  } else if (num_threads == 2){
    if(thread_id == 0){
      from = thread_id; 
      to = thread_id+1;
    } else {
      from = thread_id+1; 
      to = thread_id+2;
    }
  } else if(num_threads == 4){
    from = thread_id;
    to = thread_id;
  } else {
    return NULL;
  }
  
  
  for (; from<=to; from++){ 
    
    //printf("\n Self: %lu Loop: %d / %d\n", (int unsigned long)pthread_self(), from, to);
    rnum = from;

    // collect a number of samples
    for (j=0; j<SAMPLES_TO_COLLECT; j++){ 

      
      // skip a number of samples
      for (k=0; k<samples_to_skip; k++){
	      rnum = rand_r((unsigned int*)&rnum);
      }

      // force the sample to be within the range of 0..RAND_NUM_UPPER_BOUND-1
      key = rnum % RAND_NUM_UPPER_BOUND;

      /////////////////////////// Critical Section ///////////////////////////
      // if this sample has not been counted before
      pthread_mutex_lock(&mutex1);
      if (!(s = h.lookup(key))){
        // insert a new element for it into the hash table
        s = new sample(key);
        h.insert(s);
      }
      
      // increment the count for the sample
      s->count++;
      pthread_mutex_unlock(&mutex1);
      
    }
  }
  
  return NULL;
  
}

int main (int argc, char* argv[]){

  /////////////////////////// Print out team information ///////////////////////////
  printf( "Team Name: %s\n", team.team );
  printf( "\n" );
  printf( "Student 1 Name: %s\n", team.name1 );
  printf( "Student 1 Student Number: %s\n", team.number1 );
  printf( "Student 1 Email: %s\n", team.email1 );
  printf( "\n" );

  /////////////////////////// Parse program arguments ///////////////////////////
  if (argc != 3){
    printf("Usage: %s <num_threads> <samples_to_skip>\n", argv[0]);
    exit(1);  
  }

  //1, 2, or 4
  sscanf(argv[1], " %d", &num_threads); // not used in this single-threaded version
  sscanf(argv[2], " %d", &samples_to_skip);

  /////////////////////////// initialize a 16K-entry (2**14) hash of empty lists ///////////////////////////
  h.setup(14);

  /////////////////////////// Create Threads ///////////////////////////
  /*
  * threads[1] -> process all four seed streams
  * threads[2] -> process two seed streams each
  * threads[4] -> process one seed stream per thread
  * */
  pthread_t threads[num_threads];
  int pass_vals[num_threads];

  
  for (int p = 0; p<num_threads; p++){
    pass_vals[p] = p;
    pthread_create(&threads[p], NULL, seed, (void*) &pass_vals[p]);
    //printf("\ninter: %d \n", p);
  }

  for (int p = 0; p < num_threads; p++){  
    pthread_join(threads[p], NULL);
    //printf("\nThread Join: %d\n", p);
  }

  // print a list of the frequency of all samples
  h.print();
}