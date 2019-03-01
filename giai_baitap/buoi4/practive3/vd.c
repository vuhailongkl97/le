#include <stdio.h>
 #include <unistd.h>
 #include <stdlib.h>
 #include <fcntl.h>
 #include <sys/wait.h>
 #include <sys/types.h>

 int runAs(int gid, int uid) {
   int child = fork();
   int rs;
   int fd;
   if (child == 0) {
    if(setgid(gid) != 0)
	{
		perror("uid");
		return -1;
	}
    if( setuid(uid) != 0){
		perror("uid");
		return -1;
	}
	if( (fd = creat("newfile.txt" , 755)) < 0 ) 
	{
		perror("creat");
		return -2;
}
	return 0;

	//execl("/usr/bin/touch" ,"touch" , "newfile.txt" , (char*)0);
   } else if (child > 0) {
    wait(&rs);
   } else {

   }
 }


 int main(int argc, char *argv[] )
 {
     char *env[3];

     runAs(1001, 1001);
     return 0;
 }
