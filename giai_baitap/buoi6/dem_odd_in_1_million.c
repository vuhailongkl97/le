#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
long  count_odd(int low , long long  high)
{
	long long  i = 0 ; 
	long long count = 0;
	for (i = 0 ; i < high ; i ++)
	{
		count +=  ( i %2 == 0 ) ;
	}
	return count ;
}
void * count_odd2(void  *k)
{
	long long  i = 0 ; 
	//long long count = 0;
	 long long **ke = (long long **)k;
	 long long low = (*ke)[0];
	 long long high = (*ke)[1];
	 long long *result = (long long *)malloc(sizeof(long long));

	 printf("thread  get  %lld %lld %p , result = %d \n" , low , high , k , *result );
	for (i = low ; i < high ; i ++)
	{
		(*result) +=  ( i %2 == 0 ) ;
	}
	printf("\nresult thread %lld\n\n ", *result);
	printf("result address %p \n" , result);
	return (void *)result;
}
long count_odd_with_thread(int low , long long high)
{
	long  count ;	
	int i ;
	pthread_t t[3];
	i = count = 0;
	
	long long *low_high_argv1 = (long long *) malloc(sizeof(long long) *2);
	long long *low_high_argv2 = (long long *) malloc(sizeof(long long) *2);
	

	int nthread = 2  ; 

	long long *result[3] ;
	long long tcount = 0;

	// using nthread
	long long increase = high/nthread;

	low_high_argv1[0] = low;
	low_high_argv1[1] = increase;

	low_high_argv2[0] = increase +1;
	low_high_argv2[1] = high;
	
	pthread_create(&t[0] ,NULL , count_odd2, &low_high_argv1);
	pthread_create(&t[1] ,NULL , count_odd2, &low_high_argv2);
	

	for ( i = 0 ; i < nthread  ; i ++)
	{
		pthread_join(t[i] , (void *)&result[i]);
		tcount += *((long long *)result[i]);	
		printf("get result form thread %lld %p", tcount, result[i]);
	}
	free(result[0]);
	free(result[1]);

		
	free(low_high_argv1);
	free(low_high_argv2);
	//printf("\n\nket qua thread %lld\n\n"  , tcount);
	return tcount;
}
int main()
{
	clock_t t;
	double measure_time = 0;
	long long minx , maxx ; 
	minx = 0 ; 
	maxx = 100000000;
	t = clock();
	printf("count_odd with method simple %ld\n",count_odd(minx , maxx));
	t = clock() - t;

	measure_time  = (double)t/ (CLOCKS_PER_SEC);;
	printf("spend %f \n" ,measure_time);


	t = clock();
	printf("count_odd with method thread %ld\n",count_odd_with_thread(minx , maxx));
	t = clock() - t;

	measure_time  = (double)t/ (CLOCKS_PER_SEC);;
	printf("spend %f \n" ,measure_time);
	return 0;
}
