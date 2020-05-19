README
======

This package includes the following files.

|-- Invoker.c [This is the driver program which will be used to invoke the get_running_ratio.]
|-- Analyze.c [Skeleton version of Analyze, in which you have to fill in the missing code.]
|-- Analyze.h [Header file declaring the function exposed from Analyze.c]
|-- README.txt [This file]

Use the provided Makefile as a template.

============================================

Answers to HW1 Questions:
1. True
2. False
3. The free() function is located in the #include <stdlib.h>
4. Two executables are generated: Analyzer.o and Invoker.o
5. You use the 'make' command to call the default target in Makefile

=============================================

Extra Comments:
The current state of Analyze.c will execute with 0 memory leaks. To intentionally induce a memory leak, comment out row 109 of the Analyze.c file prior to compiling.

The "Makefile" required "std=c11" instead of "std=c17"(line 5) to compile on local linux machine. Still having issues remoting into school computer lab to see if this would cause issues. 

=============================================

Valgrind Outputs:
1. Without Leaks
nate@ubuntu:~/Desktop/HW1$ valgrind --leak-check=yes ./HW1 3
==15697== Memcheck, a memory error detector
==15697== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==15697== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==15697== Command: ./HW1 3
==15697==
[Invoker] With seed: 3
[Analyze] Number of iterations: 446
[Invoker] Running ratio: 0.032039

==15697==
==15697== HEAP SUMMARY:
==15697==     in use at exit: 0 bytes in 0 blocks
==15697==   total heap usage: 448 allocs, 448 frees, 813,396 bytes allocated
==15697==
==15697== All heap blocks were freed -- no leaks are possible
==15697==
==15697== For counts of detected and suppressed errors, rerun with: -v
==15697== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)



2. With Leaks
nate@ubuntu:~/Desktop/HW1$ valgrind --leak-check=yes ./HW1 3
==15668== Memcheck, a memory error detector
==15668== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==15668== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
==15668== Command: ./HW1 3
==15668==
[Invoker] With seed: 3
[Analyze] Number of iterations: 446
[Invoker] Running ratio: 0.032039

==15668==
==15668== HEAP SUMMARY:
==15668==     in use at exit: 808,804 bytes in 446 blocks
==15668==   total heap usage: 448 allocs, 2 frees, 813,396 bytes allocated
==15668==
==15668== 808,804 bytes in 446 blocks are definitely lost in loss record 1 of 1
==15668==    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==15668==    by 0x108AEB: get_running_ratio (Analyze.c:116)
==15668==    by 0x108C35: main (Invoker.c:16)
==15668==
==15668== LEAK SUMMARY:
==15668==    definitely lost: 808,804 bytes in 446 blocks
==15668==    indirectly lost: 0 bytes in 0 blocks
==15668==      possibly lost: 0 bytes in 0 blocks
==15668==    still reachable: 0 bytes in 0 blocks
==15668==         suppressed: 0 bytes in 0 blocks
==15668==
==15668== For counts of detected and suppressed errors, rerun with: -v
==15668== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)

=============================================

Final Code Output:
nate@ubuntu:~/Desktop/HW1$ ./HW1 3
[Invoker] With seed: 3
[Analyze] Number of iterations: 446
[Invoker] Running ratio: 0.032039

nate@ubuntu:~/Desktop/HW1$ ./HW1 64
[Invoker] With seed: 64
[Analyze] Number of iterations: 396
[Invoker] Running ratio: 0.032327

nate@ubuntu:~/Desktop/HW1$ ./HW1 89
[Invoker] With seed: 89
[Analyze] Number of iterations: 229
[Invoker] Running ratio: 0.031507
