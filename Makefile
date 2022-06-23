CC=clang
CFLAGS=-g -Wall -ldl
# -ldl to avoid the error undefined reference to dlsym
BINS=randomalloc batchalloc leakyalloc

all: $(BINS)

%alloc: %alloc.o allocator.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	$(RM) -rf $(BINS) *.o
