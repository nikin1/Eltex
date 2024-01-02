CC := gcc
CFLAGS := -Wall -g
LIBS := -lncurses
TARGET := program
SOURCES := *.c

#  $< - all dependcy
# $@ and $^  - the target and dependencies
.PHONY: all clean

all: chat

chat: $(SOURCES)
	$(CC) $^ -o $@ $(LIBS)

clean:
	rm -f chat


