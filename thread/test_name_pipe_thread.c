#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>
#include<signal.h>
#include<pthread.h>
//#include<bits/sigaction.h>
#define MSG_MAX_SIZE 100
char *cp_name="/tmp/sw_test.pipe";
char *sp_name="/tmp/sr_test.pipe";

void* handle_msg(){
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

void* send_msg(void* arg){
	char **argv=(char **)arg;
	int argc=atoi(*argv++);
	while (argc--)
		printf("start send msg arg is : %s\n",*argv++);
	
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

int main(int argc,char **argv){

	pthread_t handle_t,send_t;
	if(pthread_create(&handle_t,NULL,handle_msg,NULL)<0){
		perror("create handle msg thread error");
		return 1;
	}
	char *arg[3];
	arg[0]="2";
	arg[1]="dd";
	arg[2]="ee";
	if(pthread_create(&send_t,NULL,send_msg,arg)<0){
		perror("create send msg thread error");
		return 1;
	}
	pthread_join(handle_t,NULL);
	pthread_join(send_t,NULL);
	return 0;
}
