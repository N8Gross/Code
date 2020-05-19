# variable assignment for the program we’re building:
programs = LowAlpha HighAlpha Math Starter

# flags for compiling:
CFLAGS = -std=c11 -g -Wall
# Optional stricter flags:
CFLAGS += -Wextra -Wpedantic -Werror -Wfatal-errors

# Include the math library when linking:
#LDFLAGS = -lm

CC=gcc

build:$(programs)

Starter: Starter.c
HighAlpha: HighAlpha.c
LowAlpha: LowAlpha.c
Math: Math.c


clean:
	rm -f *.o *~ $(programs) FinalResult.txt
