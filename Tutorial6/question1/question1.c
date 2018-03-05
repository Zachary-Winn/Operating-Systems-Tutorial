#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>

//this program demonstrates how orphan functions are created

int main(){

    pid_t pid1,pid2;

    pid1 = fork();


    if (pid1 > 0){
        pid2 = fork();
        }

   



    if(pid1 == 0){
        
        //child1
        sleep(1);
        FILE *f1;
        int c1;
        f1 = fopen("child1.txt","r");


        while(1) {
      c1 = fgetc(f1);
      if( feof(f1) ) {
         break ;
      }
      printf("%c", c1);
   }

   fclose(f1);

    return 0;
    }

    else if(pid2 == 0){
        //child2
        sleep(1);
        int c2;
        FILE *f2;
        f2 =fopen("child2.txt", "r");

        while(1) {
      c2 = fgetc(f2);
      if( feof(f2) ) {
         break ;
      }
      printf("%c", c2);
   }

   fclose(f2);
   
    return 0;
    }
     else if (pid1 > 0  && pid2 > 0 ) {
        // parent 
        // writes files

        FILE * f3;
        FILE * f4;

        f3 =fopen("child1.txt","w");
        f4 =fopen("child2.txt","w");        


        fputs("child 1", f3);
        fputs("child 2",f4);

        fclose(f3);
        fclose(f4);
    }

    return 0;
}