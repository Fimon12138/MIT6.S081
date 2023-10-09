#include "kernel/types.h"
#include "kernel/stat.h"
#include"user/user.h"

void childProcess(int leftPipeReader);

int
main(int argc, char *argv[]) {
  int starter[2];
  pipe(starter);
  if (fork() == 0) {
    close(starter[1]);
    childProcess(starter[0]);
  } else {
    for (int i=2; i<=35; i++) {
      write(starter[1], &i, 4);
    }
    close(starter[0]);
    close(starter[1]);
    wait(0);
  }
  exit(0);
}

__attribute__((noreturn))
void 
childProcess(int leftPipeReader) {
  int p;
  int status = read(leftPipeReader, &p, 4);
  if (status == 0) {
    close(leftPipeReader);
    exit(0);
  }
  printf("prime %d\n", p);

  int n;
  int rightPipe[2];
  pipe(rightPipe);
  int rightPipeReader = rightPipe[0];
  int rightPipeWriter = rightPipe[1];
  if (fork() == 0) {
    close(leftPipeReader);
    close(rightPipeWriter);
    childProcess(rightPipeReader);
  } else {
    while (read(leftPipeReader, &n, 4) != 0) {
      if (n % p != 0) {
        write(rightPipeWriter, &n, 4);
      }
    }
    close(leftPipeReader);
    close(rightPipeReader);
    close(rightPipeWriter);
    wait(0);
  }
  exit(0);
}