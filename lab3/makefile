CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99
LDFLAGS = -lm
TEST_INPUT = test_input.txt
TEST_OUTPUT = test_output.txt

.PHONY: all clean

all: line_invert

line_invert: main.o line_invert.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
main.o: main.c line_invert.h
	$(CC) $(CFLAGS) -c $< -o $@ -I /usr/include
line_invert.o: line_invert.c line_invert.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o line_invert

test: line_invert
	@echo "Running tests..."
	@./line_invert $(TEST_INPUT) test_actual_output.txt > test_actual_output.txt
	@diff -q $(TEST_OUTPUT) test_actual_output.txt > /dev/null && echo "\nTests passed" || echo "\nTests failed"
	@rm -f test_actual_output.txt
