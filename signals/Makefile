CC := gcc
CFLAGS := -Wall -g

TARGET := program
SOURCES := *.c
PROGRAM1 := example_sigaction
PROGRAM2 := event_loop
#  $< - all dependcy
# $@ and $^  - the target and dependencies
.PHONY: test1 test2 all clean

all:  $(PROGRAM1) $(PROGRAM2) 

PROGRAM1: $(PROGRAM1).c
	gcc $^ -o $@ 

PROGRAM2: $(PROGRAM2).c
	gcc $^ -o $@ 

test1: $(PROGRAM1)
	#  In top you need to find this pid  -- kill -SIGUSR1 [pid]  
	./$(PROGRAM1)


test2: $(PROGRAM2)
	#  "kill -SIGUSR1 [pid]"
	./$(PROGRAM2) 

	
clean:
	rm -f $(PROGRAM1) $(PROGRAM2)


