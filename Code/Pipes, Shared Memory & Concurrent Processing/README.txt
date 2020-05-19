README
======

This package includes the following files.

|-- Starter.c [This is the driver program which accepts an input .txt file and contains the parent process]
|-- HighAlpha.c [HighAlpha program to determine if character input is N-Z (or n-z), count up the results, and output those results to shared memory/shell]
|-- LowAlpha.c [LowAlpha program to determine if character input is A-M (or a-m), count up the results, and output those results to shared memory/shell]
|-- Math.c [Math program to determine if character input is any one of the following math symbols:( +  -  *  / ), count up the results, and output those results to shared memory/shell]
|-- CharacterReader.c [CharacterReader program to read the input file and write all characters to Starter (excluding \r and \n characters)
|-- Makefile 
|-- input1.txt [Sample input]
|-- input2.txt [Sample input]
|-- README.txt [This file]

======

Answers to HW3 Questions:

1. To create a pipe, use: int pipe(int pipefd[2]);   where pipefd[2] is a an array of size 2. 
	pipefd[0] denotes the read end
	pipefd[1] denotes the write end
2. To map files/devices to memory, use:  void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
3. To open a shared memory object, use:  int shm_open(const char *name, int oflag, mode_t mode);
	This function will return a file descriptor that is used to read/write to the shared memory. The shared memory name MUST stay the same across all processes utilizing that shared memory space.

======

This program will create a final output file FinalResult.txt containing the resulting character counts.