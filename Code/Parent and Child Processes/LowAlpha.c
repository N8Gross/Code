#include <stdio.h>	// for fprintf(), printf()
#include <unistd.h> // for getpid()
#include <stdlib.h> //for exit()

int main(int argc, char *argv[])
{
    int input = *argv[1];
    if (argc != 2) {
    	printf("Incorrect number of inputs! \n");
    	exit(-1);
    }

	printf("LowAlpha[%d] : Received coded value %d.\n", getpid(), input); //print out the value received and process id

	if((input >= 'n' && input <= 'z') || (input >= 'N' && input <= 'Z'))
	{
		/* if statement is true, program exits and returns 1 to Starter */
	    printf("LowAlpha[%d] : Result is true.\n", getpid());
		exit(1);
	}else
	{
		/* if statement is false, program exits and returns 0 to Starter */
	    printf("LowAlpha[%d] : Result is false.\n", getpid());
		exit(0);
	}
	return 0;
}
