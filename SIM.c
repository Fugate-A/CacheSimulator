#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

int NUM_SETS;
int ASSOC;
int CACHE_SIZE;
#define BLOCK_SIZE 64

long long int **tagArray;
bool **dirty;

int Hit = 0;
int Miss = 0;
int Write = 0;
int Read = 0;
int Replacement;
int WriteBack;

void UpdateLRU(long long int add);
void UpdateFIFO(long long int add);

void UpdateLRU(long long int add) {
    int set = ((unsigned long long int)add / BLOCK_SIZE) % NUM_SETS;
    long long int tag = add / BLOCK_SIZE;
    int i;
    for (i = 0; i < ASSOC; i++) {
        if (tagArray[set][i] == tag) {
            break;
        }
    }
    bool temp = dirty[set][i];
    for (int j = i; j > 0; j--) {
        tagArray[set][j] = tagArray[set][j - 1];
        dirty[set][j] = dirty[set][j - 1];
    }
    tagArray[set][0] = tag;
    dirty[set][0] = temp;
}

void UpdateFIFO(long long int add) {
    int set = ((unsigned long long int)add / BLOCK_SIZE) % NUM_SETS;
    long long int tag = add / BLOCK_SIZE;
    if (dirty[set][ASSOC - 1] == true)
        Write++;
    for (int i = ASSOC - 1; i > 0; i--) {
        tagArray[set][i] = tagArray[set][i - 1];
        dirty[set][i] = dirty[set][i - 1];
    }
    tagArray[set][0] = tag;
    dirty[set][0] = false;
}

void Simulate_access(char op, long long int add) {
    int set = ((unsigned long long int)add / BLOCK_SIZE) % NUM_SETS;
    long long int tag = add / BLOCK_SIZE;
    for (int i = 0; i < ASSOC; i++) {
        if (tag == tagArray[set][i]) {
            Hit++;
            if (Replacement == 0) {
                UpdateLRU(add);
                if (op == 'W' && WriteBack == 1) {
                    dirty[set][0] = true;
                }
            } else {
                if (op == 'W' && WriteBack == 1) {
                    dirty[set][i] = true;
                }
            }
            if (op == 'W' && WriteBack == 0) {
                Write++;
            }
            return;
        }
    }
    Miss++;
    Read++;
    if (Replacement == 0) {
        if (dirty[set][ASSOC - 1] == true) {
            Write++;
        }
        for (int i = ASSOC - 1; i > 0; i--) {
            tagArray[set][i] = tagArray[set][i - 1];
            dirty[set][i] = dirty[set][i - 1];
        }
        tagArray[set][0] = tag;
        dirty[set][0] = false;
        if (op == 'W' && WriteBack == 1) {
            dirty[set][0] = true;
        }
        if (op == 'W' && WriteBack == 0) {
            Write++;
        }
    } else {
        UpdateFIFO(add);
        if (op == 'W' && WriteBack == 1) {
            dirty[set][0] = true;
        }
        if (op == 'W' && WriteBack == 0) {
            Write++;
        }
    }
}

int main(int arg, char **args) {
    if (arg != 6) {
        printf("Invalid Number of Arguments.");
        return 1;
    }

    CACHE_SIZE = atoi(args[1]);
    ASSOC = atoi(args[2]);
    Replacement = atoi(args[3]);
    WriteBack = atoi(args[4]);
    char *tracefile = args[5];

    FILE *file = fopen(tracefile, "r");
    if (file == NULL) {
        printf("Error: Could not open the trace file.\n");
        return 1;
    }

    NUM_SETS = CACHE_SIZE / (BLOCK_SIZE * ASSOC);

    tagArray = (long long int **)malloc(NUM_SETS * sizeof(long long int *));
    dirty = (bool **)malloc(NUM_SETS * sizeof(bool *));

    for (int i = 0; i < NUM_SETS; i++) {
        tagArray[i] = (long long int *)malloc(ASSOC * sizeof(long long int));
        dirty[i] = (bool *)malloc(ASSOC * sizeof(bool));
    }

    for (int i = 0; i < NUM_SETS; i++) {
        for (int j = 0; j < ASSOC; j++) {
            tagArray[i][j] = -1;
            dirty[i][j] = false;
        }
    }

    char op;
    long long int add;
    while (fscanf(file, " %c %llx\n", &op, &add) != EOF) {
        Simulate_access(op, add);
    }

    fclose(file);

    double Miss_ratio = (double)Miss / (Miss + Hit);
    printf("Miss Ratio %lf\n", Miss_ratio);
    printf("write %d\n", Write);
    printf("read %d\n", Read);

    for (int i = 0; i < NUM_SETS; i++) {
        free(tagArray[i]);
        free(dirty[i]);
    }
    free(tagArray);
    free(dirty);
    return 0;
}
