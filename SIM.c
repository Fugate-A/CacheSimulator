
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//---------------------------------------------------------

#define BlockSize 64

//---------------------------------------------------------

int nos;
int assoc;

int CacheSize;

long long int **tag;
bool **dirty;

int Hits = 0;
int Misses = 0;

int Reads = 0;
int Writes = 0;

int wb = -1;
int rp = -1;

//---------------------------------------------------------

//headers

//---------------------------------------------------------
int main( int arg, char** args )
{
  if( arg != 6 )
  {
    printf("Not Enough Arguments");
    return 1;
  }
  
  CacheSize = atoi(args[1]);       //atoi = ASCII to Int function in C
  assoc = atoi(args[2]);
  int rp = atoi(args[3]);
  int wb = atoi(args[4]);
  
  char* tracefilepath = args[5];
  FILE * tracefile = fopen(tracefilepath, "r");

  if (tracefile == NULL)
  {
    printf("No file found in this location or could not open file");
    return 1;
}

  nos = CacheSize / ( BlockSize * assoc );  //nos = number of sets
  int offsetBits = log2( BlockSize );
  int indexBits = log2( nos );
  int tagBits = BlockSize - indexBits - offsetBits;
  
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


