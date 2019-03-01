// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <string.h> 
#include <pthread.h>
#include <sys/wait.h>

#define PORT 8080 
#define nthread 1 

pthread_t t[nthread]; 

void *thread(void *argv)
{
int pid;

struct sockaddr_in address; 
int sock = 0, valread; 
struct sockaddr_in serv_addr; 
char hello[20] ; 
char buffer[1024] = {0}; 

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return (void*)NULL;
		//return -1; 
	} 

	memset(&serv_addr, '0', sizeof(serv_addr)); 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	   
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return (void*)NULL;
	//	return -1; 
	} 
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		//return -1;
		return (void*)NULL; 
	} 
	if ( (pid = fork()) < 0)  	
	{
		perror("fork");
		return (void*)NULL;
	}
	//parent
	else if( pid > 0 )
	{
		while(1)
		{

			printf("\nenter  message : ");
			scanf("%s",hello);
			//sprintf(hello , "hello from client %d" , *(int *)argv);
			send(sock , hello , strlen(hello) , 0 ); 
			//printf("Hello message sent\n"); 
		}
		wait(&pid);
	}
	// child 
	else {
		while(1)
			{	
			valread = read( sock , buffer, 1024); 
			printf("\n co tin nhan moi\n");
			printf("\n%s\n",buffer ); 
			}
	}
	
	
	return (void*)NULL;
}

int main(int argc, char const *argv[]) 
{

	int *arg = (int*)malloc(sizeof(int)*(nthread));

	for (int i = 0 ; i < nthread ; i++)
	{
		arg[i]	= i;
		pthread_create(&t[i] , NULL , thread ,&arg[i] );
	}

	for (int j = 0 ; j< nthread ; j++)
	{
		pthread_join(t[j] , NULL );
	}
	free(arg);

	return 0; 
}
