#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int  nFile = 2 , nNumber = 10000000;
// ghi nNumber so vao file 
void *write_to_file_mult(void *inf)
{
	FILE *fp;
	//inf  = {"namefile" };
	int i , j;
	char *namefile = *((char **)inf);
	puts(namefile);
	fp = fopen(namefile , "w");
		// tao random 
		for( j = 0 ; j <nNumber ; j ++) 
		{
			fprintf(fp, "%d" , rand()%10);
		}

	fclose(fp);
	return (void *)NULL;
}

void write_to_file_non_mult()
{
	FILE *fp[5] ; 
	char namefile[15];

	int i , j ; 
	for ( i = 0 ; i < nFile	 ; i ++) 
	{
		sprintf(namefile, "myfile%d.txt",i);
		printf("create %s\n" , namefile );
		fp[i] = fopen(namefile , "w");
		// tao random 
		for( j = 0 ; j <nNumber ; j ++) 
		{
			fprintf(fp[i], "%d" , rand()%10);
		}
	}
		
}
int main()
{
	
	// tao 10 file 	
	clock_t t;
	double measure_time = 0;
	pthread_t mthread[5];
	char *namefile[5]; 
	char buff[15]; 
	int i ; 
	for ( i = 0 ; i < nFile ; i ++) 
	{
		namefile[i] = (char *)malloc(sizeof(char) *15);
		sprintf(namefile[i], "myfile%d.txt",i);
	}
	
	t = clock();
	write_to_file_non_mult();
	t = clock() - t;
	measure_time  = (double)t/ (CLOCKS_PER_SEC);;
	printf("spend %f \n" ,measure_time);


	t = clock();
	for (i = 0 ; i < nFile ; i ++ )
			{
				pthread_create(&mthread[i],NULL ,  write_to_file_mult , (void *)&namefile[i]);
			}

	for (i = 0 ; i < nFile ; i ++ )
			{
				pthread_join(mthread[i] , (void**)NULL);
			}
			
	t = clock() - t;
	measure_time  = (double)t/ (CLOCKS_PER_SEC);;

	printf("spend %f \n" ,measure_time);
	for ( i = 0 ; i < nFile ; i ++) 
	{
		free(namefile[i]);
	}
	return 0;
}

