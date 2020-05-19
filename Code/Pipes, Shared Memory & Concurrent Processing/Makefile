programs = LowAlpha HighAlpha Math Starter CharacterReader

CFLAGS = -std=gnu99 -g -Wall
CFLAGS += -Wextra -Wpedantic -Werror -Wfatal-errors 

LDLIBS = -lrt

CC=gcc

build:$(programs)

Starter: Starter.c
HighAlpha: HighAlpha.c
LowAlpha: LowAlpha.c
Math: Math.c
CharacterReader: CharacterReader.c


clean:
	rm -f *.o *~ $(programs)
