#include <stdio.h>
#include <sys/wait.h> //for waitpid(), WEXITSTATUS()
#include <unistd.h> // for fork(), getpid()
#include <stdlib.h> // for exit()
#include <sys/mman.h> // for shm_open(), shm_close()
#include <sys/stat.h> //for mode constants
#include <fcntl.h>  // for O_* constants
#include <sys/types.h> //for ftruncate
#include <sys/stat.h>
#include <errno.h> // for error msg
#include <string.h> // for perror

int main(int argc, char *argv[])
{

    int i=0,counter=0,errnum;
    char input[60000],array[15];
    sprintf(array,"%s",argv[1]); //print the shared memory name to a character string
    sprintf(input,"%s",argv[2]); //print the input file text to the input array for analysis

    int shm_fd1 = shm_open(array, O_CREAT | O_RDWR, 0666); //open the HighAlpha shared memory file descriptor for writing
    if(shm_fd1<1) //check for errors opening fd
    {
      errnum = errno;
      fprintf(stderr, "Value of errno: %d\n", errno);
      perror("Error printed by perror");
      fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
    }

    if (argc != 3) {
    	printf("Incorrect number of inputs! \n");
    	exit(-1);
    }

    while(input[i] != '\0')
    {
        if((input[i] >= 'n' && input[i] <= 'z') || (input[i] >= 'N' && input[i] <= 'Z'))
        {
            counter++;
        }
        i++;
    }
    write(shm_fd1,&counter,10); //print out the final counter to the shared memory file descriptor
    write(shm_fd1,'\0',10); //print out NULL to  the shared memory file descriptor
    printf("HighAlpha[%d]: counted %d\n",getpid(),counter); //HighAlpha prints out its final result
    exit(0); //exit normally
    return 0;
}
