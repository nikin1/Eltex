CC := gcc
CFLAGS := -Wall -g

SOURCES := *.c
PROGRAM1 := server
PROGRAM2 := client_tcp
PROGRAM3 := client_udp
#  $< - all dependcy
# $@ and $^  - the target and dependencies
.PHONY: test all clean 

all: clean $(PROGRAM1) $(PROGRAM2) $(PROGRAM3)

SERVER: $(PROGRAM1).c
	gcc $^ -o $@ 

CLIENT_TCP: $(PROGRAM2).c  
	gcc $^ -o $@ 


CLIENT_UDP: $(PROGRAM3).c  
	gcc $^ -o $@ 



clean:
	rm -f $(PROGRAM1) $(PROGRAM2) $(PROGRAM3)


