CC := gcc
CFLAGS := -Wall -g

TARGET := program
SOURCES := *.c
PROGRAM1 := pipe_example
PROGRAM2 := mkfifo_example
#  $< - all dependcy
# $@ and $^  - the target and dependencies
.PHONY: test1 test2 all clean

all: mkfifo_example pipe_example

PROGRAM1: $(PROGRAM1).c
	gcc $^ -o $@ 

PROGRAM2: $(PROGRAM2).c
	gcc $^ -o $@ 

test1: $(PROGRAM1)
	./$(PROGRAM1) Hello world!

test2: $(PROGRAM2)
	./$(PROGRAM2) Hello world!
	
clean:
	rm -f $(PROGRAM1) $(PROGRAM2)
	rm -f myfifo