#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char* target_name){
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    return;
  }
  if(fstat(fd, &st) < 0){
    close(fd);
    return;
  }
  
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
        continue;
    if(strcmp(".", de.name) == 0 || strcmp("..", de.name) == 0)
        continue;

    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    memmove(p, de.name, DIRSIZ);

    struct stat sub_st;
    if(stat(buf, &sub_st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
    }
    if(sub_st.type == T_DIR){
        find(buf, target_name);
        continue;
    }
    if(strcmp(de.name, target_name) == 0){
        printf("%s\n", buf);
    }

  }
  close(fd);
  return;
}

int 
main(int argc, char *argv[]){
    if(argc != 3){
        fprintf(2, "Usage: find path ... [expression]\n");
        exit(1);
    }

    char* path = argv[1];
    char* target_name = argv[2];
    find(path, target_name);
    exit(0);
}