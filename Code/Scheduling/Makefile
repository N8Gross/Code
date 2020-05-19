programs = scheduler

CFLAGS = -std=gnu99 -g -Wall
CFLAGS += -Wextra -Wpedantic -Werror -Wfatal-errors 

LDLIBS = -lrt

CC=gcc

build:$(programs)

scheduler: scheduler.c


clean:
	rm -f *.o *~ $(programs)
