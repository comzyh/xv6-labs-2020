#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void
find(const char *root, const char *target)
{
  int fd;
  if((fd = open(root, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", root);
    return;
  }
  struct dirent de; // directory entity
  struct stat st;   // stat struct
  char buf[512];    // full path
  char *p;          // the begining position of the filename in this level

  // add `/` after root
  strcpy(buf, root);
  p = buf + strlen(buf);
  *p++ = '/';

  while(read(fd, &de, sizeof(de)) == sizeof(de)) {
    if(de.inum == 0) // end of directory
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if(stat(buf, &st) < 0) {
      fprintf(2, "ls: cannot stat %s\n", buf);
      continue;
    }
    if(de.name[0] == '.' &&
       (de.name[1] == 0 ||                         // ignore .
        (de.name[1] == '.' && de.name[2] == 0))) { // ignore ..
      continue;
    }
    if(st.type == T_DIR) {
      find(buf, target);
    } else if(st.type == T_FILE) {
      if(strcmp(target, de.name) == 0) {
        printf("%s\n", buf);
      }
    }
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc < 3) {
    fprintf(2, "root path and filename to find must be specificed.");
  }
  struct stat st;
  const char *root = argv[1];
  const char *filename = argv[2];
  if(stat(root, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", root);
    exit(1);
  }
  if(st.type != T_DIR) {
    fprintf(2, "find: %s is not directory\n", root);
    exit(1);
  }
  find(root, filename);
  exit(0);
}