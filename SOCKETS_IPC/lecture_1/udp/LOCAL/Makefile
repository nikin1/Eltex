CC := gcc
CFLAGS := -Wall -g

TARGET := program
SOURCES := *.c
PROGRAM1 := server_local
PROGRAM2 := client_local
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
	rm -rf /tmp/dgram_serv
	rm -rf /tmp/dgram_client
	
