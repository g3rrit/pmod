#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdarg.h>

void error(char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  exit(-1);
}

int is_proc(char *p) {
  size_t len = strspn(p, "0123456789");
  if(len < strlen(p)) {
    return 0;
  }
  return 1;
}

void ps() {
  DIR *proc = opendir("/proc");
  if(!proc) {
    error("unable to open /proc");
  }

  char proc_exe[256] = { 0 };
  char proc_name[256] = { 0 };
  struct dirent *de;
  while((de = readdir(proc))) {
    if(!is_proc(de->d_name)) continue;

    memset(proc_exe, 0, 256);
    memset(proc_name, 0, 256);

    strcpy(proc_exe, "/proc/");
    strcat(proc_exe, de->d_name);
    strcat(proc_exe, "/exe");

    readlink(proc_exe, proc_name, 256);
    printf("pid: %s - name: %s\n", de->d_name, proc_name);
  }
  closedir(proc);
}


int main(int argc, char **argv) {

  //print running processes
  ps();

  return 0;
}
