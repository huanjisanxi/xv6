#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(){
    int p1[2], p2[2];
    char buffer[1];

    if(pipe(p1) < 0 || pipe(p2) < 0){
        fprintf(2, "Unable to create pipe");
        exit(1);
    }

    int pid = fork();
    if(pid == -1){
        fprintf(2, "Unable to create a child");
        exit(1);
    }

    int pid_num = getpid();
    if(pid == 0){   // child
        close(p1[1]);
        close(p2[0]);

        read(p1[0], buffer, sizeof(buffer));
        printf("%d: received ping\n", pid_num);
        close(p1[0]);

        write(p2[1], buffer, sizeof(buffer));
        close(p2[1]);
    }
    else{   // parent
        close(p1[0]);
        close(p2[1]);

        const char msg = 'H';
        write(p1[1], &msg, sizeof(msg));
        close(p1[1]);

        read(p2[0], buffer, sizeof(buffer));
        printf("%d: received pong\n", pid_num);
        close(p2[0]);
    }
    
    exit(0);
}