#include <stdio.h>	// for fprintf(), printf(), fopen(), fclose()
#include <sys/wait.h> //for waitpid(), WEXITSTATUS()
#include <unistd.h> // for fork(), getpid()
#include <stdlib.h> // for exit()

int main(int argc,char **argv)
{

	if (argc != 3) {
    	printf("Incorrect number of inputs! \n");
    	return 1;
    }

	int fd;
	int c=0;
	fd = atoi(argv[1]);

	//create pointer to the input file location, and opens in read mode
    FILE *fp = fopen(argv[2],"r");


	if(fp == NULL)
	{
      perror("Error in opening file");
      return(-1);
    }
	while((c = fgetc(fp)) != EOF)
	{
		if((c != '\r') && (c != '\n'))
		{
			//writes character through write end of pipe to Starter
			//will not include the \r and \n characters
            write(fd,&c,1);
		}
	}

	close(fd); //close write end of pipe
	fclose(fp); //close document stream

	return 0;
}
