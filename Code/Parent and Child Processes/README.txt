README
======

This package includes the following files.

|-- Starter.c [This is the driver program which accepts an input .txt file and contains the parent process]
|-- HighAlpha.c [HighAlpha program to determine if character input is N-Z (or n-z)]
|-- LowAlpha.c [LowAlpha program to determine if character input is A-M (or a-m)]
|-- Math.c [Math program to determine if character input is any one of the following math symbols: +  -  *  / ]
|-- Makefile 
|-- input.txt [Sample input that can be modified to send in a single character line]
|-- README.txt [This file]

======

Answers to HW2 Questions:

1. WEXITSTATUS() returns 8 bits
2. WEXITSTATUS() requires header:  #include <sys/wait.h>
3. fork() returns the value of 0 to the child process
4. fork() can fail due to a lack of memory resources
5. We are performing sequential processing in the program Starter

======

This program will create a final output file FinalResult.txt containing the resulting character counts.