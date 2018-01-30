#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
// test zombie process
int main(int args,char *argv[]){
	pid_t pid = fork();
	if(pid<0){
		perror("fork child process fail!");
		return 1;
	}
	if (pid>0){
		// parent code
//		while(1)
		sleep(20);
		waitpid()
		
	}else{
		//child code
		//do nothing	
//		while(1) sleep(10);
	}
	return 0;
}
