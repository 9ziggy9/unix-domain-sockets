CC=gcc
CFLAGS=-Wall -Wextra -pedantic -Wconversion -Wunreachable-code -Wswitch-enum
EXE_HOST=./host
EXE_CLIENT=./client

all: $(EXE_HOST) $(EXE_CLIENT) run

$(EXE_CLIENT): client.c
	$(CC) $(CFLAGS) -o $@ $<

$(EXE_HOST): host.c
	$(CC) $(CFLAGS) -o $@ $<

run_host:
	$(EXE_HOST)
	@echo HOSTING WITH PID: $!

run_client:
	$(EXE_CLIENT)

clean:
	rm -rf $(EXE_HOST) $(EXE_CLIENT)
