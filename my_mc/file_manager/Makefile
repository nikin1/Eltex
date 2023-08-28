CC := gcc
CFLAGS := -Wall -g

SOURCES := *.c
HEADERS := *.h
PROGRAM := file_manager
ADD_LIBRARY := lncurses

#  $< - all dependcy
# $@ and $^  - the target and dependencies
.PHONY: all clean

all: clean $(PROGRAM)

$(PROGRAM): $(SOURCES)
	$(CC) $(SOURCES) $(HEADERS) -$(ADD_LIBRARY) -o $@ 



clean:
	rm -f $(PROGRAM)

