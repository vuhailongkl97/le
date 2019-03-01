#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

pthread_t ntid;
typedef struct {
	char name[15] ; 
	int sdt;	
	int sdt2;	
}my_struct;

my_struct  struct_data ;
void setData(my_struct *m , char *name , int sdt){

    //(m->name) = (char *) malloc( sizeof( char ) *10);
	strcpy((m->name) , name );
	m->sdt = sdt;
	m->sdt2 = sdt-100;
	
}
void printData(my_struct m){

	printf("\n---------\n");
	printf("name %s\n" , m.name);
	printf("sdt %d\n" , m.sdt);
	printf("sdt2 %d\n" , m.sdt2);
	printf("\n---------\n");
}
void cleanup(void *argv)
{
	printf("cleanup run for %s " , (char *)argv);
}

void printids(const char *s ) 
{
	pid_t pid;
	pthread_t tid ;

	pid = getpid();

	tid = pthread_self();

	printf(" %s  ,  pid %d  , thread id : %lu  , hex( 0x%lx )\n\n" , s , pid ,  \
			(unsigned long )tid , (unsigned long) tid);

}

void *fn_thread()
{
	printids("new thread : ");
	//setData(&struct_data , "long1" , 123);
	//printData(struct_data);
	pthread_cleanup_push(cleanup , "thread 1 111\n");

	pthread_exit((void *)&struct_data);

	pthread_cleanup_pop(0);
}

int main()
{
	int res_val = 0;
	my_struct *struct_data2 ;	
	if( pthread_create(&ntid , NULL , fn_thread , NULL)  !=  0 ) 
	{
		perror("pthread_create : ");
		return -1;
	}
	sleep(1);

	if ( pthread_join( ntid , (void *)&struct_data2) != 0 ) 
	{
		perror("pthread_join : ");
		return -1;
	}
	printf("thread 1 exit  \n");

	printids("thread main");
	printData(struct_data);
	printData(*struct_data2);
	return 0;
}
