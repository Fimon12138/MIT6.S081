#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include"user/user.h"

int
main(int argc, char *argv[]) {
  char *command = argv[1];
  char *argvList[argc+1];
  for (int i=1; i<argc; i++) {
    argvList[i-1] = argv[i];
  }
  int p = argc-1;

  char buf[MAXARG];
  char *ptr = buf;
  while (read(0, ptr, 1) != 0) {
    if (*ptr != '\n') {
      ptr++;
    }
    else {
      *ptr = 0;
      argvList[p] = buf;
      argvList[p+1] = 0;
      if (fork() == 0) {
        exec(command, argvList);
        printf("exec error\n");
        exit(1);
      } else {
        wait(0);
      }
      ptr = buf;
    }
  }
  exit(0);
}