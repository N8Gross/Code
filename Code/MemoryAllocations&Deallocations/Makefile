# variable assignment for the program we’re building:
target = HW1

# flags for compiling:
CFLAGS = -std=c11 -ggdb -Wall
# Optional stricter flags:
CFLAGS += -Wextra -Wpedantic -Werror -Wfatal-errors

# Include the math library when linking:
LDFLAGS = -lm

CC=gcc

# Our target depends upon these .o files.  If the .o files change, then
# rebuild the target using the $(CC) command on the indented lines.
# The indented lines MUST begin with a tab, NOT spaces.
#
# As the first target, it runs when make is executed with no arguments.

$(target): Analyze.o Invoker.o
	$(CC) $^ $(LDFLAGS) -o $@

# Both of these *.o files depend on this header file.  The double-colon only
# adds dependencies, so the *.o files still depend upon the corresponding *.c
# files.  We don’t have to tell make how to build a *.o from a *.c; it knows.

Analyze.o Invoker.o:: Analyze.h

# This phony target (via “make clean”) is used to remove unneeded files.


clean:
	-rm -f *.o *.gch $(target)
