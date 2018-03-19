#include <stdio.h>
#include <omp.h>
int main (int argc, char *argv[])
{
    int num = 0; // Watch out for race conditions!
    int thread_num, nthreads = 16;

/* #ifdef is a condition statement. The code in between #ifdef and #endif 
will only be executed if the condition is true. In this case, the print statement is only
executed if the _OPENMP is currently defined. 
 */

    #ifdef _OPENMP
    omp_set_num_threads(nthreads);
    #endif
    printf("Testing OpenMP, you should see each thread print...\n");

	/* This is a C preproccessor directive that assigns some code to be a
parallell region. A parallel region is a region where multiple threads can run at once

 */
    #pragma omp parallel
    {
        
        /* This directve defines the code inside  to be a critical section. 
A critical section, unlike a parallel section, only allows one thread to execute the 
code inside the section at once.

*/
        #pragma omp critical
        {

		thread_num = omp_get_thread_num();
/* #ifdef is a condition statement. The code in between #ifdef and #endif 
will only be executed if the condition is true. In this case, the print statement is only
executed if the _OPENMP is currently defined. 
 */
            #ifdef _OPENMP
            num += 1; // THREAD SAFE INCREMENT
            printf("This thread = %d, num = %d\n", thread_num, num);
            #endif
        }
    }
    return 0;
}
