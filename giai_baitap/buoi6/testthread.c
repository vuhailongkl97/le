#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>

long  tcount = 0;

void * count_odd2(void  *k)
{
	long long **ke = (long long **)k;
	long long low =  (*(ke))[0];
	long long high = (*(ke))[1];
	int *ki = (int *) malloc(sizeof(int));
	*ki = 1997;
	printf("\n\nthread 1 get  %lld %lld %p %p\n\n" , low , high , ke , ki);
	return (void *)ki;
}
long count_odd_with_thread(int low , long long high)
{
		pthread_t t;
		int *kr;
		long long *low_high_argv = (long long *)malloc(sizeof(long long)*2);
		low_high_argv[0] = 100;
		low_high_argv[1] = 1;

		printf("\nlows = %lld , highs = %lld add %p\n", low_high_argv[0], low_high_argv[1], &low_high_argv);
		pthread_create(&t ,NULL , count_odd2, (void *)&low_high_argv);
		pthread_join(t , (void *)&kr);

		int *ff = (int *)kr;
		
		printf("\n\nget from thread %d  %p\n\n " , *ff , ff);

		return tcount;
}
int main()
{
	count_odd_with_thread(0,1000000);
	return 0;
}
