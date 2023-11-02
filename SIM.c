
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//---------------------------------------------------------

#define BlockSize 64

//---------------------------------------------------------

int nos;    //nos = number of sets
int assoc;

int CacheSize;

long long int **tagArray;
bool **dirty;

int Hits = 0;
int Misses = 0;

int Reads = 0;
int Writes = 0;

int wb = -1;
int rp = -1;

//---------------------------------------------------------

void updateLRU( long long int add )
{
  //
}

//---------------------------------------------------------

void updateFIFO( long long int add )
{
  //
}

//---------------------------------------------------------

void simulate( char op, long long int add )
{
  int set = ( add / BlockSize ) % nos;
  long long int tag = add / BlockSize;

  for( int i = 0; i < assoc; i++ )
  {
    if( tag == tagArray[ set ][ i ] )
    {
      Hits++;

      if( rp == 0 ) //rp = 0 is LRU and rp = 1 is FIFO
      {
        updateLRU( add );
      }

      else//if( rp == 1 )
      {
        updateFIFO( add );
      }
    }
    
    else
    {
      Misses++;
    }
  }
}

//---------------------------------------------------------

int main( int noi, char** inputs )
{
  if( noi != 6 )  //number of inputs
  {
    printf("Not enough inputs, please try again or refer to instructions (readme or makefile)");
    return 1;
  }
  
  CacheSize = atoi( inputs[1] );       //atoi = ASCII to Int function in C
  assoc = atoi( inputs[2] );
  rp = atoi( inputs[3] );
  wb = atoi( inputs[4] );
  
  char* tracefilepath = inputs[5];
  FILE * tracefile = fopen(tracefilepath, "r");

  if( tracefile == NULL )
  {
    printf("No file found in this location or could not open file");
    return 1;
  }

  nos = CacheSize / ( BlockSize * assoc );  

  tagArray = malloc( nos * sizeof( long long int* ) );
  dirty = malloc( assoc * sizeof( bool ) );

  for( int i = 0; i < nos; i++ )
  {
    tagArray[ i ] = malloc( assoc * sizeof( long long int ) );
    
    dirty[ i ] = malloc( assoc * sizeof( bool ) );
  }
  
  for( int i = 0; i < nos; i++ )
  {
    for( int j = 0; j < assoc; j++ )
    {
      //
    }
  }





  unsigned long long** cache = /*(unsigned long long**)*/ malloc( nos * sizeof( unsigned long long* ) );

  for( int i = 0; i < nos; i++ )
  {
    cache[i] = /*(unsigned long long*)*/ malloc( assoc * sizeof( unsigned long long ) );
    
    for( int j = 0; j < assoc; j++ )
    {
      cache[i][j] = 0; // Initialize cache entries to zero or any default value
    }
  }

  char ReadWrite;
  unsigned long long hex;

  while( fscanf(tracefile, "%c 0x%llx", &ReadWrite, &hex) == 2 )
  {
    int index = (hex >> offsetBits) & (nos - 1); // Calculate the index

    printf(" Operation: %c\tHex: %llx\tindex: %d\n", ReadWrite, hex, index );
    
  if (fscanf(tracefile, "%c 0x%llx", &ReadWrite, &hex) != 2) {
    printf("Error reading input line %d\n", fscanf(tracefile, "%c 0x%llx", &ReadWrite, &hex));
    break;

  }
  }

  printf("CacheSize: %d\tassoc: %d\trp: %d\twb: %d\tnos: %d\ttag size: %d\tindex size: %d\t offset size: %d", 
          CacheSize, assoc, rp, wb, nos, tagBits, indexBits, offsetBits );





  //free cache
  for( int i = 0; i < nos; i++ )
  {
    free(cache[i]);
  }

  free(cache);

  return 0;
}


