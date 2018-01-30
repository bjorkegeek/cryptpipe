CFLAGS=-O3 -Wall
LIBS=-lcrypt

cryptpipe: cryptpipe.c
	$(CC) $(CFLAGS) $(LIBS) -o $@ $<

clean:
	@rm -rf cryptpipe

.PHONY: clean

