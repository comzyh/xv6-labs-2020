#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"
const int BUFSIZE = 512;
int
main(int argc, char *argv[])
{
  if(argc < 2) {
    fprintf(2, "xargs need at least one argument");
    exit(1);
  }
  char *executable = argv[1];
  char *args[MAXARG];
  for(int i = 1; i < argc; i++) {
    args[i - 1] = argv[i];
  }
  int argbase = argc - 1; // the begining index of args from stdin.
  char buf[BUFSIZE + 1];
  char *start = buf;    // start of current line
  char *end = buf;      // end of read buffer
  char *end_line = buf; // on going pointer to line end
  int eof = 0;
  while(end_line < end || !eof) {
    while(end_line < end && *end_line != '\n') {
      end_line++;
    }
    if(end_line == end) {
      // didn't find new line, then move chunk to the front
      int len = end - start;
      memmove(buf, start, len);
      end_line -= start - buf;
      start = buf;
      end = start + len;
      // read a new chunk
      int readsize = read(0, end, BUFSIZE - (end - buf));
      end += readsize;
      *end = '\0';
      if(readsize == 0) {
        eof = 1;
      }
      continue; // and find the line end again
    }
    // then string [cur, end) is one line
    char *p = start;
    int cnt = 0;
    while(p < end_line && *p != ' ' && *p != '\n') {
      args[argbase + cnt++] = p;
      while(p < end_line && *p != ' ' && *p != '\n') {
        p++;
      }
      while(*p == ' ') {
        *p++ = 0; // fill space with '\0'
      }
    }
    *p = 0;
    args[argbase + cnt++] = 0;
    if(fork() == 0) {
      exec(executable, args);
    } else {
      wait((int *)0);
    }
    start = end_line + 1;
    end_line = start;
  }
  close(0);
  exit(0);
}