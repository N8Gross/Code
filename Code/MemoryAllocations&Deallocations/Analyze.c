#include "Analyze.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * Takes an array of integers and the number of elements in the array as
 * arguments.  Return the ratio of square numbers to nonsquare numbers
 */

float return_ratio(int *arr, int size)
{
	 float numRound=0.0,numSqrt=0.0,remainder=0.0,ratio=0.0;
	 int square=0, nonsquare=0;
	 int *p = &arr[0];
	 for(int i=0; i<size; i++)
	 {
		numSqrt= sqrt(p[i]);//take the square root of each number in the array
		numRound = round(numSqrt); // round that number to the nearest whole number
		remainder = numSqrt-numRound; //subtract to find the remainder, and evaluate if remainder=0. If statement is true, number is a perfect sqaure, if not then it is a non-sqaure
		 if(remainder == 0.0)
		 {
			 square++;
		 }
		 else
		 {
			 nonsquare++;
		 }
	 }
	 ratio = (float)square / nonsquare;
	 return(ratio);
}


/*
 * Takes a random number and maps it to the range [100 - 500).
 * Used to get the number of iterations in the range
 */
int get_iteration_count(int rand)
{
    return rand % (500-100) + 100;
}


/*
 * Takes a random number and maps it to the range [100 - 800).
 * Used to get the size of array within the range
 */
int get_arr_size(int rand)
{
    return rand % (800 - 100) + 100;
}


/*
 * Takes a random number and maps it to the range [1 - 1000).
 * Used to get the value of an element in the array within the range
 */
int get_arr_val(int rand)
{
    return rand % (1000 - 1) + 1;
}


/*
 * This is the method that you need to implement.
 * Implement the following steps in the given order.
 *
 *  1. Generate the number of iterations
 *      - Generate a random number. Map it to the range [100 - 500) using the
 *        utility function 'get_iteration_count'.
 *
 * 2. For each iteration:
 *
 *      2.a Generate an array size - Generate a random number and map it to
 *	    the range [100 - 800) using the utility function 'get_arr_size'.
 *
 *      2.b Allocate memory for the array in the heap.
 *
 *      2.c Populate the array with random numbers and map number to
 *	    the range [1 - 1000) using the get_arr_val utility function.
 *
 *      2.d Get the ratio of square numbers to nonsquare numbers.
 *
 *      2.e Find the average ratio among all of the arrays.
 */
float get_running_ratio()
{
    int randomNum1 = rand();
    int iterations = get_iteration_count(randomNum1); //calling get_iteration_count to generate the number of iterations to be triggered. ****Step #2****
	printf ("[Analyze] Number of iterations: %d\n", iterations);
	int **p=malloc(iterations*sizeof(int *)); //dynamically allocated rows for a 1D array of pointers
	float sum=0.0,finalAvg=0.0; //generate float for the sum and the final average to be returned at end of loop

	for(int i=0; i<iterations; i++)
	{
		int randomNum2 = rand();
		int arraySize = get_arr_size(randomNum2); // use get_arr_size to determine the array size for each iteration ***STEP #3****
		p[i]=malloc(sizeof(int)*arraySize); //allocated columns to hold the data

		for(int j=0;j<arraySize;j++)
		{
			int randomNum3 = rand();
			p[i][j] = get_arr_val(randomNum3); //populates the array with random numbers ****STEP #4****
		}

	sum += return_ratio(p[i],arraySize); //Adding up the results from calculating the ratio to get a final sum of all the ratios****STEP #6****

	free(p[i]); //free up the row space after use
	}
	
	free(p); 
	p=NULL;
	finalAvg = sum/iterations; //Calculates the final ratio, ***STEP #7***
	return(finalAvg); //Returns the final average to invoker ***STEP #8***
 }
