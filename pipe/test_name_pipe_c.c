#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>
#include<signal.h>
//#include<bits/sigaction.h>
#define MSG_MAX_SIZE 100
char *cp_name="/tmp/sw_test.pipe";
char *sp_name="/tmp/sr_test.pipe";

void handle_msg(){
	if(access(cp_name,R_OK)<0){
		perror("read client pipe file error");
		return;
	}
	int fd=open(cp_name,O_RDONLY);
	if(fd<0){
		perror("open pipe file fail");
		return ;
	}	
	char msg[MSG_MAX_SIZE];
	while(1){
		memset(msg,'\0',MSG_MAX_SIZE);
		int n=read(fd,msg,MSG_MAX_SIZE);
		if(n>0){
			printf("pid:%d say:\"%s\"\n",getpid(),msg);
			if(strcmp(msg,"bye\n")==0)break;	
		}
	}
//	printf("child process:%d end!!",getpid());
	close(fd);
}

void send_msg(){
	if(access(sp_name,W_OK)<0 && mkfifo(sp_name,O_CREAT|O_EXCL)<0){
		perror("create pipe file error");
		return ;
	}
	int fd=open(sp_name,O_WRONLY);
	if(fd<0){
		perror("open pipe file fail");
		return ;
	}
	char msg[MSG_MAX_SIZE];
	while(1){
		memset(msg,'\0',sizeof(msg));
		read(STDIN_FILENO,msg,MSG_MAX_SIZE);
		write(fd,msg,strlen(msg));
		if(strcmp("bye\n",msg)==0) break;
	}
//	wait(NULL);
	close(fd);
}

void wait_child(int signo){
	
	int statu;
	pid_t pid=waitpid(0,&statu,0);	
	printf("child process pid:%d has exit,clear it,status=%d",pid,statu);
}

int main(int argc,char **argv){

	// child process handle msg
	pid_t pid=fork();
	if(pid<0){
		perror("fork son process error!!");
	}else if(pid>0){
		send_msg();
	}else{
		struct sigaction new_act,old_act;
		new_act.sa_handler=wait_child;
		sigemptyset(&new_act.sa_mask);
		new_act.sa_flags=0;
		sigaction(SIGCHLD,&new_act,&old_act);
		handle_msg();
	}
	return 0;
}
