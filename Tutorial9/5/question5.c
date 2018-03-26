#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>

int numberOfThreads;
int n = 100000000;

double yValue(double x){

	return exp(x) * cos(x)*sin(x)*sqrt(5*x+6.0);
}


int main(int argc, char * argv[]){


	double dx;
	FILE *fp;

	fp = fopen("calculations.txt","w");//clear out text file
	fclose(fp);


	if (argc!=2){
		printf("please 1 argument\n");
		exit(1);
	}

	numberOfThreads = atoi(argv[1]);
	printf("number of threads set to: %i\n",numberOfThreads);

	dx = 1.0/(n+1);
#ifdef _OPENMP
	omp_set_num_threads(numberOfThreads);

#endif


	printf("starting all these threads!\n");

#pragma omp parallel for	// creates thread for each iteration of the for loop
	for(int i = 0; i<n;i++){

		double x = i*dx;
		double y = yValue(x);

		if(i%1000000==0){
			printf("writing log for index %i\n",i);
			#pragma omp critical (writing) // marks as critical section for mutex
			{
			fp = fopen("calculations.txt","a");
			fprintf(fp,"index: %i\t\tx : %f\t\ty: %f\n",i,x,y);
			fclose(fp);
		}
		}
}
}
