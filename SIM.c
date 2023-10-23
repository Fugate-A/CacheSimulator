
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//#define NoS 1           //number of sets
//#define assoc 1         //associativity
#define BlockSize 64    //Block Size

int Hits = 0;
int Misses = 0;

int CacheSize = -1;     //cache size
int assoc = -1;         //associativity
int rp = -1;            //replacement policy -> 0 means LRU and 1 means FIFO
int wb = -1;            //write back policy -> 0 means write through and 1 means write back
char *tracefile;        //tracefile name and its path


//for function headers

int main(int arg, char** args)
{
  if(arg != 6)
  {
    printf("Invalid Number of Arguments.");
    return 1;
  }

  CacheSize = atoi(args[1]);       //atoi = ASCII to Int function in C
  assoc = atoi(args[2]);
  rp = atoi(args[3]);
  wb = atoi(args[4]);
  tracefile = args[5];

  if( rp == 0 )
  {
    //call LRU
  }

  else//if( rp == 1 )
  {
    //call FIFO
  }

  printf("Miss ratio: %d\nWrites: %d\nReads: %d", /*PLACEHOLDERS*/ 1 , 1 , 1 );

  //printf("the cache size is %d\nthe assoc is %d\nthe rp is %d\nthe wb is %d\nthe name and path is %s", CacheSize, assoc, rp, wb, tracefile);
}