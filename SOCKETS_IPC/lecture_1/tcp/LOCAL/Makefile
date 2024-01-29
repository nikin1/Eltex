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

test_server: $(PROGRAM1)
	./$(PROGRAM1)

test_client:  $(PROGRAM2)
	./$(PROGRAM2)

clean:
	rm -f $(PROGRAM1) $(PROGRAM2)
	rm -f /tmp/stream_serv


