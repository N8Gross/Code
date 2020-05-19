#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strtok

void arrangeArrival(int numrow,char *CSVtable[][4]);
void arrangePriority(int numrow,char *CSVtable[][4]);
void SJF_scheduler(int n,char *CSV[][4],int finalResult[][4]);
void PBS_scheduler(int n,char *CSV[][4],int finalResult[][4]);
void Swap(char ** a,char ** b);
void printGANTT(int row,int GANTT[][3]);
void printResults(int n, int finalResult[][4]);
void arrangeProcess(int numrow,int CSVtable[][4]);
void RR_scheduler(int n,int TimeQuantum, char *CSV[][4],int finalResult[][4]);

int main(int argc,char **argv)
{

	if (argc != 4) {
    	printf("Incorrect number of inputs! \n");
    	return 1;
    }

    char* SchType = argv[1];
    int TimeQuantum = atoi(argv[3]);
    char * CSVtable[20][4]; //array to parse input csv
    char * token;
    char string[1234]; //input string array
    int x=1,j1=1,j2=0,j3=0,j4=0;

    FILE *fp = fopen(argv[2],"r"); //open CSV file in read mode

    if(fp == NULL)
	{
      perror("Error in opening file");
      return(-1);
    }

	fread(string,1234,1,fp); //read file into a string array for evaluation

	fclose(fp);

    token=strtok(string,",\n"); //parse input string to the next comma to new line
    CSVtable[0][0]=token; //input first element to array
    int numrow=0;

    while(token != NULL)
    {
		//sorts the input file into their respective columns
        switch(x)
        {
            case 0:
                //column 1 input: process ID
                token=strtok(NULL,",\n"); //parse input string to the next comma or new line
                CSVtable[j1][x]=token; //input value to array
                j1++; //increment row
                x++;//increment column
                numrow++;//increment total number of rows
                break;
            case 1:
                //column 2 input: Priority
                token=strtok(NULL,",\n"); //parse input string to the next comma or new line
                CSVtable[j2][x]=token;
                j2++;
                x++;
                break;
            case 2:
                //column 3 input: Arrival time
                token=strtok(NULL,",\n"); //parse input string to the next comma or new line
                CSVtable[j3][x]=token;
                j3++;
                x++;
                break;
            case 3:
                //column 4 input: Burst time
                token=strtok(NULL,",\n"); //parse input string to the next comma or new line
                CSVtable[j4][x]=token;
                j4++;
                x=0; //row completed, reset x back to 0 for next row
                break;
        }
    }

    int finalResult[numrow][4]; //table to hold final results
    char *SJF= "SJF";
    char *PBS= "PBS";
    char *RR= "RR";


	// Compare the incoming string input request to the programmed scheduler types
	// If none match, send error message

    if(strcmp(SchType,SJF)==0)
    {
        SJF_scheduler(numrow,CSVtable,finalResult); //execute SJF
    }else if(strcmp(SchType,PBS)==0)
    {
        PBS_scheduler(numrow,CSVtable,finalResult); //execute PBS
    }else if(strcmp(SchType,RR)==0)
    {
        RR_scheduler(numrow,TimeQuantum,CSVtable,finalResult); //execute RR
    }
    else
    {
        printf("Incorrect scheduler input. Please retry.\n");
    }

    return 0;

}

void printResults(int n,int finalResult[][4])
{
    arrangeProcess(n,finalResult); //arrange final result table by process ID

	//Function prints out the final wait time and turnaround time for each process

    printf("Process ID | Waiting Time | Turnaround Time\n");
    for(int k=1;k<n;k++)
    {
        finalResult[k][2]=finalResult[k][3]+finalResult[k][1];
        printf("%5d      |%7d       |%9d\n",finalResult[k][0],finalResult[k][1],finalResult[k][2]);
    }
}

void printGANTT(int row,int GANTT[][3])
{
	//Function prints out the final GANTT chart results

    printf("\n------ Gantt Chart ------\n");
    printf("Start | Process ID | End\n");

    for(int i=0;i<row;i++)
    {
        if(i!=0 && GANTT[i][0] != GANTT[i-1][2])
        {
            printf("%3d   |    %s    |%3d\n",GANTT[i-1][2],"IDLE",GANTT[i][0]);
            printf("%3d   |%6d      |%3d\n",GANTT[i][0],GANTT[i][1],GANTT[i][2]);
        }else
            printf("%3d   |%6d      |%3d\n",GANTT[i][0],GANTT[i][1],GANTT[i][2]);
    }
}

void Swap(char ** a,char ** b)
{
	//swap for the arrangeArrival function
    char *temp =*a;
    *a=*b;
    *b=temp;
}

void swapProcess (int *a,int  *b)
{
	//swap for the arrangeProcess function
    int temp = *a;
    *a=*b;
    *b=temp;
}

void arrangeArrival(int numrow,char *CSVtable[][4])
{
	//Program will arrange incoming table by arrival time
    for(int i=0;i<numrow;i++)
    {
        for(int j=1;j<numrow-i-1;j++)
        {
            if(atoi(CSVtable[j][2])>atoi(CSVtable[j+1][2]))
            {
                for(int k=0;k<4;k++)
                {
                    Swap(&CSVtable[j][k],&CSVtable[j+1][k]);
                }
            }
        }
    }
}

void arrangeProcess(int numrow,int CSVtable[][4])
{
	//Program will arrange final output table by process ID
    for(int i=0;i<numrow;i++)
    {
        for(int j=1;j<numrow-i-1;j++)
        {
            if((CSVtable[j][0])>(CSVtable[j+1][0]))
            {
                for(int k=0;k<4;k++)
                {
                    swapProcess(&CSVtable[j][k],&CSVtable[j+1][k]);
                }
            }
        }
    }
}

void RR_scheduler(int n,int TimeQuantum, char *CSV[][4],int finalResult[][4])
{
    arrangeArrival(n,CSV); //arrange CSV by program arrival time

    int remTime[n]; //array for remaining time left of the process
    int arrTime[n]; //array for arrival time of each process
    int complete=0,time=0;
    int GANTT[1000][3]; //array to hold the GANTT chart results

    for (int i=1;i<n;i++)
    {
        remTime[i] = atoi(CSV[i][3]); //copy the initial burst time into remaining time array for evaluation
        arrTime[i]= atoi(CSV[i][2]); //copy the initial process arrival time into array for evaulation
        finalResult[i][3]=atoi(CSV[i][3]); //copy burst time to the final Result table
    }
	printf("\n\n");
	int row=0;
    int flag=0;

    while(complete != (n-1)) //runs while all process are not complete (n-1 processes to be considered since 1st line in CSV table is headers)
    {

        for(int j=1;j<n;j++) //loop through
        {
            if(arrTime[j]<=time) //check if arrival time is less than or equal to current time
            {
                if(arrTime[j]<=TimeQuantum) //check if arrival time is less than time quantum input
                {
                    if(remTime[j]>0) //check if the remaining time left in process is greater than 0
                    {
                        if(remTime[j]>TimeQuantum) //check if remaining time for process is greater than the time quantum input
                        {
                            if(time ==0 && flag == 0)
                            {
								//if this is the first process and the burst time is greater than time quantum, set start time to current time (0s) and end time to Time quantum;
                                GANTT[row][0]=time;
                                GANTT[row][1]=atoi(CSV[j][0]);
                                GANTT[row][2]=TimeQuantum;
                                time= time+TimeQuantum;
                                remTime[j] = remTime[j] - TimeQuantum; //subtract time quantum from the remaining run time of the process
                                arrTime[j] = arrTime[j] + TimeQuantum; //add time quantum to the arrival time to move to the back of the ready queue
                                row++; //increment GANTT array row
                                flag=1; //set flag to 1 to indicate first row is completed

                            }
                            else
                            {
                                GANTT[row][0]=time; //input current time for the start time of process to GANTT
                                GANTT[row][1]=atoi(CSV[j][0]); //input current process number to GANTT
                                time = time + TimeQuantum;//increment time
                                GANTT[row][2]=time; //input end time for the program
                                remTime[j] = remTime[j] - TimeQuantum; //update remaining time for process
                                arrTime[j] = arrTime[j] + TimeQuantum; //update arrival time for process to move to back of ready queue
                                row++;
                            }

                        }
                        else //remaining time left in process is less than the given time quantum, and process arrived within the 1st time quantum
                        {
                            if(time ==0 && flag == 0)
                            {
                                GANTT[row][0]=time;
                                GANTT[row][1]=atoi(CSV[j][0]);
                                GANTT[row][2]=remTime[j];
                                time= time+remTime[j];//update time to current time (0s) + the burst time of the program
                                remTime[j] = 0; //remaining time is 0
                                row++; //increment GANTT array row
                                flag=1;
								finalResult[j][1]=time - atoi(CSV[j][3])-atoi(CSV[j][2]); //input final wait time to the finalRest (wait time = current time - burst time - arrival time)
                                finalResult[j][0]=atoi(CSV[j][0]); //input process ID to final result table
								complete++; //note a process completed
                            }
                            else
                            {
                                complete++; //note a process completed
                                GANTT[row][0]=time;
                                GANTT[row][1]=atoi(CSV[j][0]);; //print current process id
                                time = time + remTime[j];
                                GANTT[row][2]=time;
                                finalResult[j][1]=time - atoi(CSV[j][3])-atoi(CSV[j][2]);
                                finalResult[j][0]=atoi(CSV[j][0]);
                                row++;
                                remTime[j]=0;
                                j=0; //reset j to 0
                            }
                        }
                    }
                }
                else if(arrTime[j]>TimeQuantum) //if the arrival time is greater than the time quantum, then execute comparisons to decide which program to execute
                {
                    for(int k=1;k<n;k++) //for loop to compare program arrival times
                    {
                        if(arrTime[k]<arrTime[j]) //check the current program arrival time to other programs, if any are less than current then execute that program
                        {
                            if(remTime[k]>0)
                            {
                                if(remTime[k]>TimeQuantum)
                                {
                                    printf("process id%d with remaining time %d\n",atoi(CSV[k][0]),remTime[k]);
                                    GANTT[row][0]=time;
                                    GANTT[row][1]=atoi(CSV[k][0]);
                                    time = time + TimeQuantum;
                                    GANTT[row][2]=time;remTime[k] = remTime[k] - TimeQuantum;
                                    arrTime[k] = arrTime[k] + TimeQuantum;
                                    row++;

                                }
                                else
                                {
                                    complete++;
                                    GANTT[row][0]=time;
                                    GANTT[row][1]=atoi(CSV[k][0]);
                                    time = time + remTime[k];
                                    GANTT[row][2]=time;
                                    finalResult[k][1] = time - atoi(CSV[k][3])- atoi(CSV[k][2]);
                                    finalResult[k][0]=atoi(CSV[k][0]);
                                    remTime[k]=0;
                                    row++;
                                    j=0;

                                }
                            }
                        }
                    }

                    if(remTime[j]>0) //if current process does have the lowest arrival time (top of ready queue) then perform evaulations
                    {
                        if(remTime[j]>TimeQuantum)
                        {
                            GANTT[row][0]=time;
                            GANTT[row][1]=atoi(CSV[j][0]);
                            time = time + TimeQuantum;
                            GANTT[row][2]=time;
                            remTime[j] = remTime[j] - TimeQuantum;
                            arrTime[j] = arrTime[j] + TimeQuantum;
                            row++;

                        }
                        else
                        {
                            complete++;
                            GANTT[row][0]=time;
                            GANTT[row][1]=atoi(CSV[j][0]);
                            time = time + remTime[j];
                            GANTT[row][2]=time;
                            finalResult[j][1]= time - atoi(CSV[j][3])-atoi(CSV[j][2]);
                            finalResult[j][0]= atoi(CSV[j][0]);
                            remTime[j]=0;
                            row++;
                            j=0;
                        }

                    }

                }
            }
            else if(arrTime[j] > time)
            {
				//accounts for idle period when all programs arrival > current time
                time++;
                j=0;
            }

        }

    }


    printf("\n--------------- Round Robin ---------------\n");
    printResults(n,finalResult);
    printGANTT(row,GANTT);

    int total_waittime=0,total_tat=0;
    float avg_waittime=0,avg_tat=0;
    for(int i=1;i<n;i++)
    {
        total_waittime += finalResult[i][1]; //add up wait times
        total_tat += finalResult[i][2]; //add up burst times for each process to calculate throughput
    }
    avg_waittime=(float)total_waittime/(n-1); //calculate average wait time
    printf("\nAverage Waiting Time:  %.3f\n",avg_waittime);

    avg_tat=(float)total_tat/(n-1); //calculate average turnaround time
    printf("Average Turnaround Time:  %.3f\n",avg_tat);
    printf("Throughput:  %.3f\n\n",(float)complete/time);
}

void SJF_scheduler(int n,char *CSV[][4],int finalResult[][4])
{
    arrangeArrival(n,CSV);

    int remTime[n]; //array for remaining time left of the process
    int complete=0,time=0,minm=999,currProcess=0, end_time;
    int compProcess=0; //variable to note if a process just completed
    int GANTT[100][3]; //array to hold the GANTT chart results
    int check=0;

    for (int i=1;i<n;i++)
    {
        remTime[i] = atoi(CSV[i][3]); //copy burst times into remaining time table (per process)
		finalResult[i][3]=atoi(CSV[i][3]);//copy burst time into the final result table
    }
	printf("\n\n");

    int temp;
    int row=0;

    while(complete != n-1)
    {
        for(int j=1;j<n;j++)
        {
            //go through all process to check if arrival time is less than current time, if the process has the smallest burst time
            //and if the burst time is >0. This will preempt any in process program.
            if(((atoi(CSV[j][2])<=time)) && (remTime[j]<minm) && (remTime[j]>0))
            {
                minm = remTime[j]; //sets the new minimum to smallest burst time
                currProcess=j; //sets the current process selected
                check=1;
                temp=atoi(CSV[j][0]); //holds the temp process to be considered
            }
        }


        if(check==0)
        {
            //if no process fits criteria, increment time and continue
            time++;
            continue;
        }

        if(time==0)
        {
			//if time is 0s, set the first row
            GANTT[row][0]=time;
            GANTT[row][1]=temp;
        }

        if(GANTT[row][1] != temp)
        {
			//if the previous process being considered does not match the current process, set the new row start time and process
            GANTT[row][0]=time;
            GANTT[row][1]=temp;
        }

        remTime[currProcess] = remTime[currProcess] - 1;//subtract one 1sec from burst time

        minm=remTime[currProcess];//update the minimum time variable with new burst time value

        if(minm == 0)
        {
			//is minimum value equals 0 now, then reset minimum value to 999
            minm=999;
        }

        if(remTime[currProcess] == 0) //if the remaining time of the current process is 0, then go through process complete final updates
        {
            complete++; //increment complete counter
            check=0; //reset the check value
            compProcess=1; //update process complete flag
            end_time = time + 1; //not the final end time for the current process
            finalResult[currProcess][0]=temp; //update the process ID into the final result table
            finalResult[currProcess][1] = end_time - atoi(CSV[currProcess][3])-atoi(CSV[currProcess][2]); //update the wait time into the final result table

        }

        time++; //increment time

		/*check if the current process running match prev process
        if it doesn't equal, add the end time to previous process
        start new process
        */

        if(GANTT[row][1] != GANTT[row-1][1])
        {
			//if the previous process in GANTT chart doesn't match the current process
			//then update end time for the current process
            GANTT[row][2]=time;
            if(GANTT[row][1] != GANTT[row-1][1] && compProcess == 1)
            {
                GANTT[row-1][2]=end_time;
            }
            compProcess=0; //reset process complete flag
            row++; //increment GANTT array row
        }
        if(complete == n-1)
        {
			//if this is the last process to run, set the final end time
            GANTT[row-1][2]=end_time;
        }

    }


	//Final average and print lines are below, similar to RR setup and descriptions. (See RR_scheduling for details)

    printf("------------------- SJF -------------------\n");
    printResults(n,finalResult);
    printGANTT(row,GANTT);

    int total_waittime=0,total_tat=0;
    float avg_waittime=0,avg_tat=0;
    for(int i=1;i<n;i++)
    {
        total_waittime += finalResult[i][1];
        total_tat += finalResult[i][2];
    }
    avg_waittime=(float)total_waittime/(n-1);
    printf("\nAverage Waiting Time:  %.3f\n",avg_waittime);

    avg_tat=(float)total_tat/(n-1);
    printf("Average Turnaround Time:  %.3f\n",avg_tat);
    printf("Throughput:  %.3f\n\n",(float)complete/end_time);
}

void PBS_scheduler(int n,char *CSV[][4],int finalResult[][4])
{
    arrangeArrival(n,CSV); // Arrange in arrival order

    int remTime[n]; //array for remaining time left of the process
    int complete=0,time=0,currProcess=0, end_time;
    int prio=999;
    int compProcess=0;
    int GANTT[100][3];
	int temp,row=0;
	int check=0;

    for (int i=1;i<n;i++)
    {
        remTime[i] = atoi(CSV[i][3]); //copy the burst time to the remaining time array for evaluation
		finalResult[i][3]=atoi(CSV[i][3]); //update burst time into the final result table
    }

    printf("\n\n");

    //check all jobs for arrival time less than current time and which priority is the lowest

    while(complete != (n-1))
    {
        for(int j=1;j<n;j++)
        {
            if((atoi(CSV[j][2])<=time) && (atoi(CSV[j][1])<=prio) && (remTime[j]>0)) //check if the arrival time is less than current time, if priority is less than current priority, and if remaining time is greater than 0 for that process
            {
                prio=atoi(CSV[j][1]); //set the new priority
                currProcess=j; //update the current process for evaluation
                temp=atoi(CSV[j][0]);//update temp process ID
                check=1;
            }
        }

        if(check==0)
        {
            time++;
            continue;
        }

        if(time==0)
        {
			//if time is 0s, set the first row
            GANTT[row][0]=time;
            GANTT[row][1]=temp;
        }

        if(GANTT[row][1] != temp)
        {
			//if the previous process being considered does not match the current process, set the new row start time and process
            GANTT[row][0]=time;
            GANTT[row][1]=temp;
        }

        remTime[currProcess] = remTime[currProcess] - 1;//subtract one 1sec from burst time

        if(remTime[currProcess] == 0) //process completed all burst time and will set final flags
        {
            complete++; //increment process complete counter;
            compProcess=1; //set process complete flag to 1
            prio=999; //reset prio flag once program completes (non-preemptive)
            check=0;

            end_time = time +1; //update end time for process

            finalResult[currProcess][0]=temp; //update the process ID into the final result table
            finalResult[currProcess][1] = end_time - atoi(CSV[currProcess][3])-atoi(CSV[currProcess][2]); //update the wait time into the final result table

        }

        time++; //increment time

        /*check if the current process running match prev process
        if it doesn't equal, add the end time to previous process
        start new process
        */

        if(GANTT[row][1] != GANTT[row-1][1])
        {
            GANTT[row][2]=time;
            if(compProcess == 1)
            {
                GANTT[row-1][2]=end_time;
                compProcess=0;
            }
            row++;
        }
        if(complete == n-1)
        {
            GANTT[row-1][2]=end_time;
        }

    }

	//Final average and print lines are below, similar to RR setup and descriptions. (See RR_scheduling for details)

    printf("------------------- PBS -------------------\n");
    printResults(n,finalResult);
    printGANTT(row,GANTT);

    int total_waittime=0,total_tat=0;
    float avg_waittime=0,avg_tat=0;

    for(int i=1;i<n;i++)
    {
        total_waittime += finalResult[i][1];
        total_tat += finalResult[i][2];
    }

    avg_waittime=(float)total_waittime/(n-1);
    printf("\nAverage Waiting Time:  %.3f\n",avg_waittime);

    avg_tat=(float)total_tat/(n-1);
    printf("Average Turnaround Time:  %.3f\n",avg_tat);
    printf("Throughput:  %.3f\n\n",(float)complete/end_time);

}

