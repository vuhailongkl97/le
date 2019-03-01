// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <pthread.h> 
#define PORT 8080 
#define maxOnline 10

pthread_mutex_t mutex;
int nOnline = 0;
int user[maxOnline];
pthread_t t[maxOnline];
int server_fd, new_socket, valread; 
struct sockaddr_in address; 
int opt = 1; 
int addrlen = sizeof(address); 
char buffer[1024] = {0}; 
char hello[30]; 

static void *thread(void *arg)
{
		int my_id ;
		// mutex bao ve . neu co new user vao phai doi gan xong da roi moi dc ++
		my_id = *(int *)arg;	
		nOnline ++;
		pthread_mutex_unlock(&mutex);	

		//printf("\nmy_id %d ... \n" , my_id);
		while( strcmp(buffer, "d")) 
		{
			valread = read( user[my_id] , buffer, 1024); 
			printf("%s\n",buffer ); 
			sprintf(hello , "Server hello to %d" , my_id);
			for(int i = 0 ; i < nOnline ; i++)
				send(user[i] ,hello , strlen(hello) , 0 ); 
			//printf("sent %d\n", my_id); 
		}
	
	return (void *)NULL;
}
int main(int argc, char const *argv[]) 
{ 
	   
	pthread_mutex_init(&mutex , NULL);
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	   
	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												  &opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	   
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address,  
								 sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 5) < 0) 
		{ 
			perror("listen"); 
			exit(EXIT_FAILURE); 
		} 

	while(1)
	{
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
						   (socklen_t*)&addrlen))<0) 
		{ 
			perror("accept"); 
			exit(EXIT_FAILURE); 
		}

		pthread_mutex_lock(&mutex);	
		user[nOnline] = new_socket;
		if( nOnline < maxOnline)
		{
			pthread_create(&t[nOnline] , NULL , thread , &nOnline  );
		}
	}
	pthread_mutex_destroy(&mutex);
	return 0; 
}
