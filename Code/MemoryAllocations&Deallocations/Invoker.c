#include "Analyze.h"	// for get_running_ratio()
#include <stdio.h>	// for fprintf(), printf()
#include <stdlib.h>	// for srand()

int main(int argc, char **argv) {
    if (argc != 2) {
    	fprintf(stderr, "Usage: %s <number-of-iterations>\n", argv[0]);
    	return 1;
    }

    int seed = atoi(argv[1]);
    // set the seed
    srand(seed);
    printf("[Invoker] With seed: %d\n", seed);
    // invoke the function in Analyze.c
    float running_ratio = get_running_ratio();
    printf("[Invoker] Running ratio: %f\n\n", running_ratio);
    return 0;
}
