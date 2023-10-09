#include "kernel/types.h"
#include "kernel/stat.h"
#include"user/user.h"

int
main(int argc, char *argv[]) {
  int status;

  int p[2];
  pipe(p);
  write(p[1], "a", 1);
  if (fork() == 0) {
    // child process
    int pid = getpid();
    char v[1];
    read(p[0], v, 1);
    printf("%d: received ping\n", pid);
    write(p[1], "a", 1);
    close(p[0]);
    close(p[1]);
    exit(0);
  } else {
    // parent process
    int pid = getpid();
    wait(&status);
    char v[1];
    read(p[0], v, 1);
    printf("%d: received pong\n", pid);
    close(p[0]);
    close(p[1]);
  }
  exit(0);
}