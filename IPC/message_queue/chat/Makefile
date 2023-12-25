CC := gcc
CFLAGS := -Wall -g
LIBS := -lncurses
TARGET := program
SHARED_FILES := display.c  list.c system_v_functions.c
#  $< - all dependcy
# $@ and $^  - the target and dependencies
.PHONY: all clean

all: server client

server: $(SHARED_FILES) server.c
	$(CC) $^ -o $@ $(LIBS)

client: $(SHARED_FILES) client.c
	$(CC) $^ -o $@ $(LIBS)

clean:
	rm -f server client


