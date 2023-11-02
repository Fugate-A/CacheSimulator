
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

double Hits = 0;
double Misses = 0;

int Reads = 0;
int Writes = 0;

int wb = -1;
int rp = -1;

int set = -1;
long long int tag = -1;

//---------------------------------------------------------

void updateLRU( long long int addy )
{
  int tempPlace = -1;

  for( int i = 0; i < assoc; i++ )
  {
    if( tagArray[ set ][ i ] == tag )
    {
      tempPlace = i;
      break;
    }
  }

  bool holder = dirty[ set ][ tempPlace ];

  for( int i = tempPlace; i > 0; i-- )
  {
    tagArray[ set ][ i ] = tagArray[ set ][ i - 1 ];
    dirty[ set ][ i ] = dirty[ set ][ i - 1 ];
  }

  tagArray[ set ][ 0 ] = tag;
  dirty[ set ][ 0 ] = holder;

}

//---------------------------------------------------------

void updateFIFO( long long int addy )
{
  //
}

//---------------------------------------------------------

void simulate( char op, long long int addy )
{
  set = ( addy / BlockSize ) % nos;
  tag = addy / BlockSize;

  for( int i = 0; i < assoc; i++ )
  {
    if( tag == tagArray[ set ][ i ] )
    {
      Hits++;

      if( rp == 0 ) //rp = 0 is LRU and rp = 1 is FIFO
      {
        updateLRU( addy );
        
        if( op == 'W' && wb == 1 )
        {
          dirty[ set ][ 0 ] = true;
        }
      }

      else if( op == 'W' && wb == 1 )
      {
        dirty[ set ][ i ] = true;
      }
      
      else//if( op = w and wb = 1 )
      {
        Writes++;
      }
      return;
    }
  }

  Misses++;
  Reads++;

  if( rp == 0 )
  {
    if( dirty[ set ][ assoc - 1 ] == true )
    {
      Writes++;
    }

    for( int i = assoc - 1; i > 0; i-- )
    {
      tagArray[ set ][ i ] = tagArray[ set ][ i - 1 ];
      dirty[ set ][ i ] = dirty[ set ][ i - 1 ];
    }

    tagArray[ set ][ 0 ] = tag;
    dirty[ set ][ 0 ] = false;

    if( op == 'W' && wb == 1 )
    {
      dirty[ set ][ 0 ] = true;
    }

    if( op == 'W' && wb == 0 )
    {
      Writes++;
    }
  }

  else
  {
    updateFIFO( addy );

    if( op == 'W' && wb == 1 )
    {
      dirty[ set ][ 0 ] = true;
    }

    else if( op == 'W' && wb == 0 )
    {
      Writes++;
    }
  }
}

//---------------------------------------------------------

char* policyString( int rp )
{
  if( rp == 0 )
  {
    return "LRU";
  }

  else
  {
    return "FIFO";
  }
}

//---------------------------------------------------------

char* wbString( int wb )
{
  if( wb == 0 )
  {
    return "through";
  }

  else
  {
    return "back";
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

  /*printf("CacheSize: %lldB\tAssoc: %d\trp: %d\twb: %d\ntrace path: %s",
              CacheSize, assoc, rp, wb, tracefilepath);*/

  printf("1");

  nos = CacheSize / ( BlockSize * assoc );  

  printf("2");

  tagArray = malloc( nos * sizeof( long long int* ) );
  dirty = malloc( assoc * sizeof( bool ) );

  printf("3");

  for( int i = 0; i < nos; i++ )
  {
    tagArray[ i ] = malloc( assoc * sizeof( long long int ) );
    
    dirty[ i ] = malloc( assoc * sizeof( bool ) );
  }

  printf("4");
  
  for( int i = 0; i < nos; i++ )
  {
    printf("\nthis is i: %d",i);
    for( int j = 0; j < assoc; j++ )
    {
      printf("\nthis is j: %d",j);
      tagArray[ i ][ j ] = -1;

      dirty[ i ][ j ] = false;
    }
  }

  printf("5");

  while( fscanf( tracefile, "%c %llx", &op, &addy ) != EOF )
  {
    simulate( op, addy );
  }

  fclose( tracefile );

  printf("\nResults:\nMiss ratio: %lf\nWrites: %d\nReads: %d\n\nExtra Information:\n\tHits: %d\tMisses: %d\n\n\tInputs:\n\t\tCache Size: %d\tAssociativity: %d\t Policy: %s, Write_%s",
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
