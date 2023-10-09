#include "kernel/types.h"
#include "kernel/stat.h"
#include"user/user.h"

int
main(int argc, char *argv[]) {
  char *ERR_NO_ARGUMENT = "ERROR: missing argument: number of ticks";

  if (argc < 2) {
    write(1, ERR_NO_ARGUMENT, strlen(ERR_NO_ARGUMENT));
    exit(1);
  }
  int ticks = atoi(argv[1]);
  sleep(ticks);
  exit(0);
}