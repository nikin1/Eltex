CC := gcc
CFLAGS := -Wall -g

SOURCES := *.c
PROGRAM1 := server
PROGRAM2 := client
#  $< - all dependcy
# $@ and $^  - the target and dependencies
.PHONY: test all clean 

all: clean $(PROGRAM1) $(PROGRAM2)

SERVER: $(PROGRAM1).c
	$(CC) $^ -o $@ 

CLIENT: $(PROGRAM2).c
	$(CC) $^ -o $@ 

clean:
	rm -f $(PROGRAM1) $(PROGRAM2)

	
