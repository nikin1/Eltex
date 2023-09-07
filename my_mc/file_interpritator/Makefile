CC := gcc
CFLAGS := -Wall -g

SOURCES := *.c
HEADERS := *.h
PROGRAM := file_interpritator

#  $< - all dependcy
# $@ and $^  - the target and dependencies
.PHONY: all clean

all: clean $(PROGRAM)

$(PROGRAM): $(SOURCES)
	$(CC) $(SOURCES) -o $@ 


test1:
	./$(PROGRAM) "ls -l"

test2:
	./$(PROGRAM) "ls -l | wc -l"

test3:
	./$(PROGRAM) "ls -l | grep a"

test4:
	./$(PROGRAM) "ls -la| grep a"

clean:
	rm -f $(PROGRAM)
