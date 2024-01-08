CFLAGS=-O3 -Wall
LIBS=-lcrypt

cryptpipe: cryptpipe.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	@rm -rf cryptpipe

.PHONY: clean

