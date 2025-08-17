#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define BUF_SIZE 512

int
main(int argc, char *argv[]){
    char buf[BUF_SIZE];
    char* p = buf;

    char* xargv[MAXARG] = {0};
    for(int i=1;i<argc;++i){
        xargv[i-1] = argv[i];
    }

    int xargc = argc - 1;
    
    while(read(0, p, sizeof(char)) > 0){
        if(*p == ' ' || *p == '\n' || *p == '\0'){
            int execcmd = (*p == '\n') || (*p == '\0');
            *p = '\0';
            xargv[xargc] = malloc(BUF_SIZE);
            strcpy(xargv[xargc], buf);
            xargc++;
            p = buf;
            
            if(execcmd){
                int pid = fork();
                if(pid < 0){
                    fprintf(2, "Unable to create child\n");
                    exit(1);
                }
                else if (pid == 0){
                    // printf("%s\n", argv[1]);
                    if(exec(argv[1], xargv) < 0){
                        fprintf(2, "exec fail\n");
                        exit(1);
                    }
                }
                else{
                    int status;
                    wait(&status);
                }
                // memset(buf, 0, sizeof(buf));
                xargc = argc - 1;
            }
        }
        else{
            p++;
        }
    }
}