#include "kernel/types.h"
#include "kernel/stat.h"
#include"user/user.h"
#include "kernel/fs.h"

void find(const char *path, const char *filename);
int match(const char *path, const char *filename);

void 
main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(2, "ERROR: missing params: {path} {filename}\n");
    return;
  }
  const char *path = argv[1];
  const char *filename = argv[2];
  find(path, filename);
  exit(0);
}

void 
find(const char *path, const char *filename) {
  int fd;
  struct stat st;
  struct dirent de;
  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "ERROR: can't open %s\n", path);
    return;
  }
  if (fstat(fd, &st) < 0) {
    fprintf(2, "ERROR: can't stat %s\n", path);
    close(fd);
    return;
  }
  switch (st.type) {
  case T_FILE:
    if (match(path, filename) == 0) {
      fprintf(1, "%s\n", path);
    }
    break;
  case T_DIR:
    char buf[512], *p;
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
      fprintf(2, "ERROR: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf + strlen(path);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
        continue;
      }
      strcpy(p, de.name);
      p[DIRSIZ] = 0;
      find(buf, filename);
    }
    break;
  }
  close(fd);
}

int 
match(const char *path, const char *filename) {
  int l = strlen(path) - 1;
  while (l >= 0 && path[l] != '/') {
    l--;
  }
  const char *subFilename = path + l + 1;
  return strcmp(subFilename, filename);
}
