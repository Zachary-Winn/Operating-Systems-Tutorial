#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(){

    pid_t pid1, pid2;
    int status;
    pid1 = fork();

    if(pid1 == 0){
        //child
        sleep(1);
        printf("child process\n");
    }
    else{
        //parent
        pid2 =wait(&status);
        printf("parent process\n");
    }


    return 0;
}