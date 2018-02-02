#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <crypt.h>

enum read_input_result {
  READ_INPUT_OK, READ_INPUT_TRUNCATED, READ_INPUT_EOF
};
  
struct input {
  const char* key;
  const char* salt;
  enum read_input_result result;
};



static void read_input(char* buf,
                       int bufsize,
                       struct input *ret)
{
  size_t readlen;
  int c;
  int line;
  ret->salt = ret->key = NULL;
  ret->key = buf;
  for (line = 0; line < 2; ++line) {
    if (NULL == fgets(buf, bufsize, stdin)) {
      ret->result = READ_INPUT_EOF;
      return;
    }
    readlen = strlen(buf);
    if (readlen == bufsize-1) {
      ret->salt = buf + readlen;
      if (*(buf + readlen - 1) == '\n') {
        ++line;
      }
      goto consume_eol;
    }
    if (*(buf + readlen - 1) != '\n') {
      ret->result = READ_INPUT_EOF;
      return;
    }
    *(buf + readlen - 1) = '\0';
    if (line == 0) {
      buf += readlen;
      bufsize -= readlen;
      ret->salt = buf;
    }
  }
  ret->result = READ_INPUT_OK;
  return;
consume_eol:
  for (; line<2; ++line) {
    do {
      c = fgetc(stdin);
      if (c == EOF) {
        ret->result = READ_INPUT_EOF;
        return;
      }
    } while (c != '\n');
  }
  ret->result = READ_INPUT_TRUNCATED;
}

int main() {
  char buf[2048];
  struct input input;
  const char* cryptret;
  struct crypt_data cryptbuf;

  while (1) {
    read_input(buf,sizeof(buf), &input);
    switch (input.result) {
      case READ_INPUT_OK:
        cryptret = crypt_r(input.key, input.salt, &cryptbuf);
        if (cryptret) {
          fputs(cryptret, stdout);
          fputc('\n', stdout);
          fflush(stdout);
          break;
        }
        /* deliberate continuation */
      case READ_INPUT_TRUNCATED:
        fputs("!\n", stdout);
        fflush(stdout);
        break;
      case READ_INPUT_EOF:
        exit(0);
    }
  } 
}
