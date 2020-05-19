README
======

This package includes the following files.

|-- Coordinator.java [This is the driver program which will be used to create an instance of the Circular buffer, and create and wait for one thread of producer and one or more threads of and consumer.] (Skeleton Code provided)
|-- Producer.java [A producer generates a single number between 0 and 99 (both inclusive) randomly and inserts into the buffer, and reports the location and the time the number was inserted into the buffer] (Skeleton Code provided)
|-- Consumer.java [Each instance of a consumer reads a number from the buffer and reports the location and the time the number was read from the buffer.]
|-- Buffer.java [Creates a circular buffer whose size is passed as an argument to it.]
|-- Makefile [For compiling, and cleaning]
|-- README.txt [This file]


Questions:

1. The problem of producer consumer is solved using a. Mutex Locks. - 1 point
	a. Mutex Locks <<<
	b. Semaphores

2. What two functions defined in Java are used for synchronization between producer and consumers in your program? Answer: wait() and notify() - 2 points

3. In which function do you override the body to define the new body of a thread in java? Answer: In the run() function - 1 point

4. Which function is used to wait for a thread to finish and come back to calling program i.e. for a thread to die? Answer: join() - 1 point
