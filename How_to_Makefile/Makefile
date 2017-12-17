CC=gcc

test: test.o
	$(CC) -o $@ $^
test.o: test.c
	$(CC) -c -o $@ $^

.PHONY: clean
clean:
	-rm *.o
