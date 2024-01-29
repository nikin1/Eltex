CC := gcc
CFLAGS := -Wall -g

SOURCES := *.c
PROGRAM1 := server_inet
PROGRAM2 := client_inet
#  $< - all dependcy
# $@ and $^  - the target and dependencies
.PHONY: test all clean 

all: clean $(PROGRAM1) $(PROGRAM2)

SERVER: $(PROGRAM1).c
	gcc $^ -o $@ 

CLIENT: $(PROGRAM2).c
	gcc $^ -o $@ 

clean:
	rm -f $(PROGRAM1) $(PROGRAM2)

	
