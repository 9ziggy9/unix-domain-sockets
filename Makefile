CC=gcc
CFLAGS=-Wall -Wextra -pedantic -Wconversion -Wunreachable-code -Wswitch-enum
EXE_HOST=host
EXE_CLIENT=client
LIB_CONNCT=./uconnect.o
BINS =./host ./client ./mem_pool_host ./mem_pool_client ./opts ./mem_pool

all: $(EXE_HOST) $(EXE_CLIENT)

mem_pool: mem_pool_host mem_pool_client

mem_pool_host: mem_pool_host.c
	$(CC) $(CFLAGS) -o $@ $<

mem_pool_client: mem_pool_client.c
	$(CC) $(CFLAGS) -o $@ $<

$(LIB_CONNCT): uconnect.c uconnect.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(EXE_CLIENT): client.c $(LIB_CONNCT)
	$(CC) $(CFLAGS) -o $@ $< $(LIB_CONNCT)

$(EXE_HOST): host.c $(LIB_CONNCT)
	$(CC) $(CFLAGS) -o $@ $< $(LIB_CONNCT)

run_host:
	$(EXE_HOST)
	@echo HOSTING WITH PID: $!

run_client:
	$(EXE_CLIENT)

opts: opts.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(EXE_HOST) $(EXE_CLIENT) $(BINS) $(LIB_CONNCT)
