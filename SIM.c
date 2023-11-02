//------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//------------------------------------------------------------------

#define BlockSize 64

//------------------------------------------------------------------

int nos;
int assoc;

long long int **tagArray;
bool **dirty;

double Hits = 0;
double Misses = 0;

int Reads = 0;
int Writes = 0;

int CacheSize;

int rp = -1;
int wb = -1;

int set = -1;
long long int tag = -1;

//------------------------------------------------------------------

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

//------------------------------------------------------------------

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

//------------------------------------------------------------------

/*bool SetTag()
{
  if( set == -1 && tag == -1 )
  {
    return false;
  }

  else//if( set and tag are updated)
  {
    return true;
  }
}*/

//------------------------------------------------------------------

void UpdateLRU( long long int addy )
{
  int tempPlace = -1;
  bool holder;
  
  for( int i = 0; i < assoc; i++ )
  {
    if( tagArray[ set ][ i ] == tag )
    {
      tempPlace = i;
      break;
    }
  }

  holder = dirty[ set ][ tempPlace ];

  for( int i = tempPlace; i > 0; i-- )
  {
    tagArray[ set ][ i ] = tagArray[ set ][ i - 1 ];
    dirty[ set ][ i ] = dirty[ set ][ i - 1 ];

    if( i == 1 )
    {
      tagArray[ set ][ 0 ] = tag;
      dirty[ set ][ 0 ] = holder;
    }
  }
}

//------------------------------------------------------------------

void UpdateFIFO( long long int addy )
{
  if( dirty[ set ][ assoc - 1 ] == true )
  {
    Writes++;
  }

  for( int i = assoc - 1; i > 0; i-- )
  {
    tagArray[ set ][ i ] = tagArray[ set ][ i - 1 ];
    dirty[ set ][ i ] = dirty[ set ][ i - 1 ];

    if( i == 1 )
    {
      tagArray[ set ][ 0 ] = tag;
      dirty[ set ][ 0 ] = false;
    }
  }
}

//------------------------------------------------------------------

void simulate(char op, long long int addy) {
    for (int i = 0; i < assoc; i++) {
        if (tag == tagArray[set][i]) {
            Hits++;
            if (rp == 0) {
                UpdateLRU(addy);
                if (op == 'W' && wb == 1) {
                    dirty[set][0] = true;
                }
            } else {
                if (op == 'W' && wb == 1) {
                    dirty[set][i] = true;
                }
            }
            if (op == 'W' && wb == 0) {
                Writes++;
            }
            return;
        }
    }
    Misses++;
    Reads++;
    if (rp == 0) {
        if (dirty[set][assoc - 1] == true) {
            Writes++;
        }
        for (int i = assoc - 1; i > 0; i--) {
            tagArray[set][i] = tagArray[set][i - 1];
            dirty[set][i] = dirty[set][i - 1];
        }
        tagArray[set][0] = tag;
        dirty[set][0] = false;
        if (op == 'W' && wb == 1) {
            dirty[set][0] = true;
        }
        if (op == 'W' && wb == 0) {
            Writes++;
        }
    } else {
        UpdateFIFO(addy);
        if (op == 'W' && wb == 1) {
            dirty[set][0] = true;
        }
        if (op == 'W' && wb == 0) {
            Writes++;
        }
    }
}

//------------------------------------------------------------------

int main( int noi, char **inputs)
{
  if (noi != 6)
  {
    printf("Not enough inputs, please try again or refer to instructions (readme or makefile)");
    return 1;
  }

  char op;
  long long int addy;

  CacheSize = atoi( inputs[1] );
  assoc = atoi( inputs[2] );
  rp = atoi( inputs[3] );
  wb = atoi( inputs[4] );
  
  char *tracefilepath = inputs[5];
  FILE *tracefile = fopen(tracefilepath, "r");
  
  if (tracefile == NULL)
  {
    printf("No file found in this location or could not open file");
    return 1;
  }

  nos = CacheSize / ( BlockSize * assoc );

  tagArray = malloc( sizeof( long long int* ) * nos );
  dirty = malloc( sizeof( bool* ) * nos );

  for( int i = 0; i < nos; i++ )
  {
    tagArray[i] = malloc( sizeof( long long int ) * assoc );
    dirty[i] = malloc( sizeof( bool ) * assoc );
  }

  for( int i = 0; i < nos; i++ )
  {
    for( int j = 0; j < assoc; j++ )
    {
      tagArray[ i ][ j ] = -1;
      dirty[ i ][ j ] = false;
    }
  }
  
  while( fscanf( tracefile, "%c %llx\n", &op, &addy) != EOF )
  {
    /*if( SetTag() == false )
    {
      //not working
    }*/

    set = ( addy / BlockSize ) % nos;
    tag = addy / BlockSize;
  
    simulate(op, addy);
  }

  printf("\nResults:\nMiss ratio: %lf\nWrites: %d\nReads: %d\n\nExtra Information:\n\tHits: %.0lf\tMisses: %.0lf\n\n\tInputs:\n\t\tCache Size: %d\tAssociativity: %d\n\t\tPolicy: %s -- write_%s\n\n",
        Misses / (Hits + Misses),
        Writes,
        Reads, Hits,
        Misses,
        CacheSize,
        assoc,
        policyString(rp),
        wbString(wb)
    );
    
  for( int i = 0; i < nos; i++ )
  {
    free(tagArray[i]);
    free(dirty[i]);

    if( i == nos - 1 )
    {
      free(tagArray);
      free(dirty);
      
      fclose(tracefile);
    }
  }
  
  return 0;

}

