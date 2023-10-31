
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BlockSize 64

int main( int arg, char** args )
{
    if( arg != 6 )
    {
        printf("Not Enough Arguments");
        return 1;
    }
    
    int CacheSize = atoi(args[1]);       //atoi = ASCII to Int function in C
    int assoc = atoi(args[2]);
    int rp = atoi(args[3]);
    int wb = atoi(args[4]);
    
    char* tracefilepath = args[5];
    FILE * tracefile = fopen(tracefilepath, "r");

    if (tracefile == NULL)
    {
        printf("No file found in this location or could not open file");
        return 1;
    }

    // Calculate the number of sets
    int nos = CacheSize / ( BlockSize * assoc );  //nos = number of sets
    
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

    while( fscanf(tracefile, " %c 0x%llx", &ReadWrite, &hex) == 2 )
    {
        // Process each line here (e.g., printing)
        printf(" Operation: %c\tHex: %llx\n", ReadWrite, hex );

        //input here?
    }

    printf("CacheSize: %d\tassoc: %d\trp: %d\twb: %d\t nos: %d", CacheSize, assoc, rp, wb, nos );













    //free cache
    for( int i = 0; i < nos; i++ )
    {
        free(cache[i]);
    }

    free(cache);

    return 0;
}
