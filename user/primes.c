#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(){
    int nums[35];
    int num_cnt = -1;
    for(int i = 2;i<=35;++i){
        nums[++num_cnt] = i;
    }

    while(num_cnt >= 0){
        int p[2];
        if(pipe(p) < 0){
            fprintf(2,"Unable to create pipe\n");
            exit(1);
        }
        int prime = nums[0];   
        printf("prime %d\n", prime);
        
        int pid = fork();

        if(pid == 0){   //child
            num_cnt = -1;
            int tmp;
            while(read(p[0], &tmp, sizeof(int))>0){
                if(tmp == 0)    break;
                nums[++num_cnt] = tmp;
            }
            close(p[0]);
        }
        else{   //parent
            int status;
            close(p[0]);
            for(int i=1; i<=num_cnt; ++i){
                if(nums[i]%prime != 0){
                    write(p[1], &nums[i], sizeof(int));
                }
            }
            int tmp = 0;
            write(p[1], &tmp, sizeof(int));
            close(p[1]);
            wait(&status);
            break;
        }
    }
    exit(0);
}