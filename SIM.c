//------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//------------------------------------------------------------------

#define BLOCK_SIZE 64

//------------------------------------------------------------------

int nos;
int assoc;
int CacheSize;
long long int **tagArray;
bool **dirty;

double Hits = 0;
double Misses = 0;

int Reads = 0;
int Writes = 0;

int rp = -1;
int wb = -1;

int set = -1;
long long int tag = -1;

//------------------------------------------------------------------

void UpdateLRU( long long int addy )
{
  int set = ((unsigned long long int)addy / BLOCK_SIZE) % nos;
  tag = addy / BLOCK_SIZE;
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
    }

    tagArray[ set ][ 0 ] = tag;
    dirty[ set ][ 0 ] = holder;
  }

void UpdateFIFO( long long int addy ) {
    set = ((unsigned long long int)addy / BLOCK_SIZE) % nos;
    long long int tag = addy / BLOCK_SIZE;
    if (dirty[set][assoc - 1] == true)
        Writes++;
    for (int i = assoc - 1; i > 0; i--) {
        tagArray[set][i] = tagArray[set][i - 1];
        dirty[set][i] = dirty[set][i - 1];
    }
    tagArray[set][0] = tag;
    dirty[set][0] = false;
}

void Simulate_access(char op, long long int addy) {
    int set = ((unsigned long long int)addy / BLOCK_SIZE) % nos;
    long long int tag = addy / BLOCK_SIZE;
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

int main(int arg, char **args) {
    if (arg != 6) {
        printf("Invalid Number of Arguments.");
        return 1;
    }

    CacheSize = atoi(args[1]);
    assoc = atoi(args[2]);
    rp = atoi(args[3]);
    wb = atoi(args[4]);
    char *tracefile = args[5];

    FILE *file = fopen(tracefile, "r");
    if (file == NULL) {
        printf("Error: Could not open the trace file.\n");
        return 1;
    }

    nos = CacheSize / (BLOCK_SIZE * assoc);

    tagArray = (long long int **)malloc(nos * sizeof(long long int *));
    dirty = (bool **)malloc(nos * sizeof(bool *));

    for (int i = 0; i < nos; i++) {
        tagArray[i] = (long long int *)malloc(assoc * sizeof(long long int));
        dirty[i] = (bool *)malloc(assoc * sizeof(bool));
    }

    for (int i = 0; i < nos; i++) {
        for (int j = 0; j < assoc; j++) {
            tagArray[i][j] = -1;
            dirty[i][j] = false;
        }
    }

    char op;
    long long int addy;
    while (fscanf(file, " %c %llx\n", &op, &addy) != EOF) {
        Simulate_access(op, addy);
    }

    fclose(file);

    double Misses_ratio = (double)Misses / (Misses + Hits);
    printf("Misses Ratio %lf\n", Misses_ratio);
    printf("Writes %d\n", Writes);
    printf("Reads %d\n", Reads);

    for (int i = 0; i < nos; i++) {
        free(tagArray[i]);
        free(dirty[i]);
    }
    free(tagArray);
    free(dirty);
    return 0;
}
