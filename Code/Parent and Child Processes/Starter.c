#include <stdio.h>	// for fprintf(), printf(), fopen(), fclose()
#include <sys/wait.h> //for waitpid(), WEXITSTATUS() 
#include <unistd.h> // for fork(), getpid()
#include <signal.h> // for WIFEXITED()

int main(int argc,char **argv)
{

    if (argc != 2) {
    	printf("Incorrect number of inputs! \n");
    	return 1;
    }
    char buf[256];
	int c;
    FILE *fp = fopen(argv[1],"r"); //create pointer to the input file location, and opens in read mode

	if(fp == NULL)
	{
      perror("Error in opening file");
      return(-1);
    }
	do
	{
	  fgets(buf,sizeof(buf),fp); //gets the characters from the file location (*fp) and stores it to the array *buf which is 256 characters long
	}while((c=fgetc(fp))!=EOF);


    fclose(fp); //close document stream
    int loc=0, status, result1=0, result2=0, result3=0, result4=0, u;

    pid_t pid1,pid2,pid3; //initiate process identifier

    while(buf[loc]>0) //loop through all characters in array until reaching empty space
    {
		//if((buf[loc] != 13) && (buf[loc] != 10))
		//{
			pid1 = fork(); //first fork() to initiate the HighAlpa program
			if(pid1>0)
			{
				/*parent process initiated*/
				waitpid(pid1,&status,0); //parent waits for the first child process (high alpha) to complete
				if(WIFEXITED(status))
				{
					result1 += WEXITSTATUS(status); //increments counter if result return is passing
					if(WEXITSTATUS(status)==0) //checks if the HighAlpha program status returned as 0 or 1: if 0 then print result returned false and else 1 result returned true
						{
							printf("Starter: Child process %d returned false\n", pid1);
						}
						else if(WEXITSTATUS(status)==1)
						{
							printf("Starter: Child process %d returned true\n", pid1);
						}
				}
				if(WIFEXITED(status) != 0 && !WEXITSTATUS(status))
				{
					pid2=fork(); //second fork() to initiate the LowAlpa program
					if(pid2>0)
					{
						waitpid(pid2,&status,0); //parent waits for the second child process (low alpha) to complete
						if(WIFEXITED(status))
						{
							result2 += WEXITSTATUS(status);
							if(WEXITSTATUS(status)==0) //checks if the LowAlpha program status returned as 0 or 1: if 0 then print result returned false and else 1 result returned true
							{
								printf("Starter: Child process %d returned false\n", pid2);
							}
							else if(WEXITSTATUS(status)==1)
							{
								printf("Starter: Child process %d returned true\n", pid2);
							}
						}
						if(WIFEXITED(status)!=0 && !WEXITSTATUS(status))
						{
							pid3=fork(); //third fork() to initiate the Math program
							if(pid3>0)
							{
								waitpid(pid3,&status,0); //parent waits for the third child process (math) to complete
								if(WIFEXITED(status))
								{
									result3 += WEXITSTATUS(status);
									if(WEXITSTATUS(status)==0) //checks if the Math program status returned as 0 or 1: if 0 then print result returned false and else 1 result returned true
									{
										printf("Starter: Child process %d returned false\n", pid3);
									}
									else if(WEXITSTATUS(status)==1)
									{
										printf("Starter: Child process %d returned true\n", pid3);
									}
								}
								if(WIFEXITED(status)!=0 && !WEXITSTATUS(status)) //All other characters (i.e. NOT HighAlpha/LowAlpha/Math) will count as Other
								{
                                    if((buf[loc] != 13) && (buf[loc] != 10))
                                    {
									    result4++;
									    if(WEXITSTATUS(status)==0)
									    {
										    printf("Starter: Child process %d returned false\n", pid3);
									    }
									    else if(WEXITSTATUS(status)==1)
									    {
										    printf("Starter: Child process %d returned true\n", pid3);
									    }
                                    }
                                    else
                                    { 
                                        u++; //other character, nonessential
                                    }
								}
							}
							else
							{
								/*3rd child process runs Math*/
								printf("Starter: Forked process with ID %d.\n",getpid());
								printf("Starter: Waiting for process %d.\n",getpid());
								execlp("./Math","Math",&buf[loc], NULL);
							}
						}
					}
					else
					{
						/*2nd child process runs LowAlpha */
						printf("Starter: Forked process with ID %d.\n",getpid());
						printf("Starter: Waiting for process %d.\n",getpid());
						execlp("./LowAlpha","LowAlpha",&buf[loc], NULL);
					}
				}
			}
		   else
			{
				/* 1st child process runs HighAlpha  */
				printf("Starter: Forked process with ID %d.\n",getpid());
				printf("Starter: Waiting for process %d.\n",getpid());
				execlp("./HighAlpha","HighAlpha",&buf[loc], NULL);
			}
			loc++; //increment counter to parse array
		//}else
        //{
            //NULL;
        //}
	}
	
    if(pid1>0)
    {
		/* print final results to bash*/
        printf("Low alpha:\t%d\n",result2);
        printf("High alpha:\t%d\n",result1);
        printf("Math:\t\t%d\n",result3);
        printf("Other:\t\t%d\n",result4);
		
		/* print final results to file */
		FILE *finalresult = fopen("FinalResult.txt","w"); //create pointer to output the final result to a file, and opens in write mode
		if(finalresult == NULL)
		{
			perror("Error in opening file");
			return(-1);
		}
		fprintf(finalresult,"Low alpha:\t%d\n",result2);
        fprintf(finalresult,"High alpha:\t%d\n",result1);
        fprintf(finalresult,"Math:\t\t%d\n",result3);
        fprintf(finalresult,"Other:\t\t%d\n",result4);
		
		fclose(finalresult); //close file
	}
	return 0;
}
