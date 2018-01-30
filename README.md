A simple utility to expose the [C crypt function](https://linux.die.net/man/3/crypt) to a parent process via pipes.

Pseudocode:

1. Read a line from stdin into `key`
2. Read a line from stdin into `salt`
3. Run `crypt(key, salt)`
4. Write the result to stdout or a single line containing only "!" on error
5. Goto 1

The process should terminate cleanly upon EOF on stdin


