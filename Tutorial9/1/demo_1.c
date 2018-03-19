#include <stdio.h>
#include <omp.h>
int main (int argc, char *argv[])
{
    printf("This statement will always print.\n");
		/* #ifdef is a condition statement. The code in between #ifdef and #endif 
will only be executed if the condition is true. In this case, the print statement is only
executed if the _OPENMP is currently defined. 
 */
    #ifdef _OPENMP
    printf("This statement will only print when compiled with \"-fopenmp\"\n");
    #endif
    return 0;
}
