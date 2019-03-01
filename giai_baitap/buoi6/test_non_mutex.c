#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>


// code test kiem tra pthread_mutex_lock , unlock , trylock 
// chu y trylock dung de kiem tra mutex do co dang bi lock hay khong ( co the su dung de tranh deadlock)
// trylock : neu chua lock thi lock lai . neu da lock roi thi return false ngay lap tuc ma ko blocking giong pthread_mutex_lock

//fn_thread 1 su dung try lock de kiem tra mut da bi lock hay chua 
//fn-thread 2 lock truoc khi fn_thread1 lock ( su dung sleep 1  trong fn_thread1  )   . 


pthread_t ntid;
pthread_t ntid2;
pthread_mutex_t mut;

int n = 100;
void *fn_thread()
{
	sleep(1);
	if( pthread_mutex_trylock(&mut) ==0)
	{
		pthread_mutex_lock(&mut);
		printf("lockking ok\n");
		printf("new thread 1\n");
		while(n==100);
		printf("ok thread exit()\n");
		pthread_mutex_unlock(&mut);
	}
	else{
		printf("other thread is locked\n\n");
	}
	//pthread_mutex_lock(&mut);	
	pthread_exit(NULL);
}

void *fn_thread2()
{
	pthread_mutex_lock(&mut);
	sleep(1);
	n--;
	printf("ban vua nhap %d" , n);
	
	pthread_mutex_unlock(&mut);
	pthread_exit(NULL);
}

int main()
{
	int res_val = 0;
	pthread_mutex_init(&mut, NULL) ;

	if( pthread_create(&ntid , NULL , fn_thread , NULL)  !=  0 ) 
	{
		perror("pthread_create : ");
		return -1;
	}
	if( pthread_create(&ntid2 , NULL , fn_thread2 , NULL)  !=  0 ) 
	{
		perror("pthread_create : ");
		return -1;
	}
	if ( pthread_join( ntid , NULL) != 0 ) 
	{
		perror("pthread_join : ");
		return -1;
	}
	if ( pthread_join( ntid2 ,NULL ) != 0 ) 
	{
		perror("pthread_join : ");
		return -1;
	}

	pthread_mutex_destroy(&mut);
	return 0;
}
