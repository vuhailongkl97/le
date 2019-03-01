#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int write_to_file()
{
	char c ; 
	int counter = 0 ;
	FILE *fp ;
	if( (fp = fopen("data.txt" , "r+"))  ==  NULL ) 
	{
		perror("fopen");
		return -1;
	}
	// dung o cuoi 
	fseek(fp , 0 , SEEK_END);
	// loai bo khoang trong 
	do {
		c = getc(fp);
		fseek(fp , -2 , SEEK_CUR);
	}
	while ( (c > '9') ||  (c < '0'));
	
	// tim toan bo so 
	do {
		c = getc(fp);
		fseek(fp , -2 , SEEK_CUR);
	}
	while ( (c <= '9') &&  (c >= '0'));

	fseek( fp , 2 , SEEK_CUR);

	// da tim duoc vi tri thich hop
	while ( (c = getc(fp) ) != EOF) 
	{
		if (c < '0' || c > '9') break;
		counter = counter*10 + (c -'0'); 
	}
	counter ++;
	printf("after change : %d\n" , counter ) ; 

	fprintf(fp ,"\n%ld %d\n" , (long int)getpid() , counter); 
	fclose(fp);
}
// gui 17 la dang co nguoi ghi  
volatile int ready_to_write  = 0; 
void handler_17(int n)
{
	printf("%d :someone writting \n" , getpid());
	ready_to_write = 0;
	while(!ready_to_write);
}
	
// da co ai do ghi xong roi  
void handler_chld(int n)
{
	
	printf("%d:someone complete\n" , getpid());
	ready_to_write = 1 ; 
}
int main()
{
	// child write before parent write 
	// child signal over SIGCHLD when done to Parent 

	int pid;
	signal(17 , handler_17);
	signal(SIGCHLD , handler_chld);

	if (( pid = fork())  <0 )
	{
		perror("fork");
		return -1;
	}
	else if( pid == 0 )
	{
		//child
		printf("child : %d\n" , (int) getpid());
		kill( getppid() , 17);
		write_to_file();		
		kill( getppid() , 18);
	}
	else {
		// parent 
		int n ; 
		printf("parent : %d\n" , (int) getpid());
		kill( pid , 17);
		write_to_file();		
		kill( pid , 18);

		wait(&n);
			
	}
	return 0;
}

