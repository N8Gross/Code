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

int main(int argc,char **argv)
{

    if (argc != 2) {
    	printf("Incorrect number of inputs! \n");
    	return 1;
    }

	int pipefds[2], status,charcount=0;
	char pipeArray[16],charArray[60000];
	pid_t pid;

	if(pipe(pipefds) == -1)
	{
        //if pipe fails, exit with error
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	//prints the write end file descriptor to the pipeArray string, will be sent into CharacterReader for use
    sprintf(pipeArray,"%d",pipefds[1]);

	pid=fork();

	if(pid == -1)
	{
		//if pid = -1, fork failed.
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if(pid==0)
	{
		//child process
		close(pipefds[0]); //close read end since child does not need it
		execlp("./CharacterReader","CharacterReader",pipeArray,argv[1],NULL);
		printf("This should not print!");

	}
	else
	{
		//parent process

		waitpid(pid,&status,0); //waits for child program CharacterReader to complete
		close(pipefds[1]); //close write end since parent does not need it
		read(pipefds[0],charArray,60000); //Starter reads from the pipe and inputs into the charArray
		close(pipefds[0]); //close the read end of the pipe

		for(int j=0;charArray[j]!= '\0'; j++)
		{
            if(charArray[j]!='\0' && charArray[j]!='\n' && charArray[j]!='\r')
            {
				//counts all characters in the string except NULL, \r, \n
				//to be used in final calculations of the "Other" characters
                charcount++;
            }
            else
            {
				//Starter adds NULL to end of string
                charArray[j]='\0';
                charArray[j+1]=-1;
            }

		}

		pid_t pid1,pid2;
        int status1,status2;
		int result1[32],result2[32],result3[32]; // arrays to hold the final result from the child processes

		char SHM_HighAlpha[15] = "SHM_HighAlpha"; //shared memory name for HighAlpha
		char SHM_LowAlpha[15] = "SHM_LowAlpha"; //shared memory name for LowAlpha
		char SHM_Math[15] = "SHM_Math"; //shared memory name for Math

        int shm_fd1 = shm_open(SHM_LowAlpha, O_CREAT | O_RDWR, 0666); //open shared memory for HighAlpha for read/write
	    int shm_fd2 = shm_open(SHM_HighAlpha, O_CREAT | O_RDWR, 0666); //open shared memory for LowAlpha for read/write
	    int shm_fd3 = shm_open(SHM_Math, O_CREAT | O_RDWR, 0666); //open shared memory for Math for read/write

		//set respective shared memory to 32 bytes in size
	    ftruncate(shm_fd1, 32);
		ftruncate(shm_fd2, 32);
		ftruncate(shm_fd3, 32);

        //maps the shared memory
		mmap(0,32,PROT_READ,MAP_SHARED,shm_fd1,0);
		mmap(0,32,PROT_READ,MAP_SHARED,shm_fd2,0);
		mmap(0,32,PROT_READ,MAP_SHARED,shm_fd3,0);

		//print out the shared memory information
		printf(".\n.\nStarter[%d]: Created shared memory %s with FD: %d\n",getpid(),SHM_LowAlpha,shm_fd1);
		printf("Starter[%d]: Created shared memory %s with FD: %d\n",getpid(),SHM_HighAlpha,shm_fd2);
		printf("Starter[%d]: Created shared memory %s with FD: %d\n",getpid(),SHM_Math,shm_fd3);

		//fork 2x to create 4 concurrent processes, execlp for each process to run to be defined below
        pid1=fork();
        pid2=fork();

		if(pid1 == -1 || pid2 ==-1)
		{
            //if pid1 OR pid2 = -1, fork failed... exit
            perror("fork");
            exit(EXIT_FAILURE);
		}
        if(pid1 == 0 && pid2 == 0) //for this state, run Math program
        {
            //child process 1
            execlp("./Math","Math",SHM_Math,charArray,NULL);
        }
        else if(pid1==0 && pid2 >0) //for this state, run HighAlpha program
        {
            //child process 2
            execlp("./HighAlpha","HighAlpha",SHM_HighAlpha,charArray,NULL);
        }
        else if(pid1>0 && pid2==0) //for this state, run LowAlpha program
        {
            //child process 3
            execlp("./LowAlpha","LowAlpha",SHM_LowAlpha,charArray,NULL);
        }
        else if(pid1>0 && pid2 >0)
        {
            //parent

			//for loop to check if the child process has completed, and will hold for up to 10 seconds before outputting final counts
            for(int i=0;i<10;i++)
            {
                pid_t pid1_waitstatus = waitpid(pid1,&status1,WNOHANG|WUNTRACED);
                pid_t pid2_waitstatus = waitpid(pid2,&status2,WNOHANG|WUNTRACED);

                if(pid1_waitstatus==-1 && pid2_waitstatus==-1)
                {
                    printf("waitpid error");
                    exit(EXIT_FAILURE);
                }
                else if(pid1_waitstatus==0 && pid2_waitstatus==0)
                {
					//child is still running, sleep 1 second
                    sleep(1);
                }
                else if(pid1_waitstatus == pid1 && pid2_waitstatus == pid2)
                {
					//read from respective shared memories and output result to array
                    read(shm_fd1,result1,32);
                    read(shm_fd2,result2,32);
                    read(shm_fd3,result3,32);

					//unlink the shared memories
					shm_unlink(SHM_HighAlpha);
                    shm_unlink(SHM_LowAlpha);
                    shm_unlink(SHM_Math);

					//calculates the final count for all other characters (excluding \n and \r)
                    int other=charcount-(*result1+*result2+*result3);

					//print out all results
                    printf("LowAlpha:   %d\n",*result1);
                    printf("HighAlpha:  %d\n",*result2);
                    printf("Math:       %d\n",*result3);
                    printf("Other:      %d\n",other);

                    printf(".\n.\n");

                    return 0;
                    exit(EXIT_SUCCESS);
                }
            }
        }
    }
}
