
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

void updateLRU( long long int addy )
{
  //
}

//---------------------------------------------------------

void updateFIFO( long long int addy )
{
  //
}

//---------------------------------------------------------

void simulate( char op, long long int addy )
{
  int set = ( addy / BlockSize ) % nos;
  long long int tag = addy / BlockSize;

  for( int i = 0; i < assoc; i++ )
  {
    if( tag == tagArray[ set ][ i ] )
    {
      Hits++;

      if( rp == 0 ) //rp = 0 is LRU and rp = 1 is FIFO
      {
        updateLRU( addy );
      }

      else//if( rp == 1 )
      {
        updateFIFO( addy );
      }
    }
    
    else
    {
      Misses++;
    }
  }
}

//---------------------------------------------------------

char* policyString( int rp )
{
  if( rp == 0 )
  {
    return "yes";
  }

  else
  {
    return "no";
  }
}

//---------------------------------------------------------

char* wbString( int wb )
{
  if( wb == 0 )
  {
    return "yes";
  }

  else
  {
    return "no";
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
  
  char op;
  long long int addy;

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

  printf("CacheSize: %lldB\tAssoc: %d\trp: %d\twb: %d\ntrace path: %s",
              CacheSize, assoc, rp, wb, tracefilepath);

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
      tagArray[ i ][ j ] = -1;

      dirty[ i ][ j ] = false;
    }
  }

  while( fscanf( tracefile, "%c %llx", &op, &addy ) != EOF )
  {
    simulate( op, addy );
  }

  fclose( tracefile );

  printf("\nResults:\nMiss ratio: %d\nWrites: %d\nReads: %d\n\nExtra Information:\n\tHits: %d\tMisses: %d\n\n\tInputs:\n\t\tCache Size: %d\tAssociativity: %d\t Policy: %s, Write_%s",
            Misses / ( Hits + Misses ),
            Writes,
            Reads,Hits,
            Misses,
            CacheSize,
            assoc,
            policyString( rp ),
            wbString( wb )
          );

  for( int i = 0; i < nos; i++ )
  {
    free( tagArray[ i ] );
    free( dirty[ i ] );

    if( i == nos - 1 )
    {
      free( tagArray );
      free( dirty );
    }
  }

  return 0;
}


