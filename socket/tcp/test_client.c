#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<pthread.h>
#include<signal.h>
#include<errno.h>
#define MSG_MAX_LEN 100
#define SERVER_IP "10.0.2.15"
#define SERVER_PORT 80

void thread_sigint_handler(int signo){
	printf("thread id : %lu received a pthread kill signal:%d,exit read message.\n",pthread_self(),signo);
}

void* read_msg(void *arg){
	struct sigaction sig;
	sigemptyset(&sig.sa_mask);
	sigaddset(&sig.sa_mask,SIGINT);
	sig.sa_handler=thread_sigint_handler;
	sigaction(SIGINT,&sig,NULL);
	int fd=*(int *)arg;
	char msg[MSG_MAX_LEN];
	int size=sizeof(msg);
	while(1){
		memset(msg,'\0',size);
		int len=read(fd,msg,size);
		if(len==0){
			printf("socket close,");
			break;
		}else if(len==-1){
			printf("read error occurred, error msg : %s \n",strerror(errno));
			break;
		}
		printf("message read from server:%s\n",msg);
	}
	return NULL;
}

int main(int argc,char **argv){
	char msg[MSG_MAX_LEN];
	struct sockaddr_in server_addr;
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr(SERVER_IP);
	server_addr.sin_port=htons(SERVER_PORT);

	int sockfd=socket(AF_INET,SOCK_STREAM,0);
			
	if(connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr))!=0){
		perror("connect to server error");
		return 1;
	}
	//start a thread for reading message from server
	pthread_t tid;                            
	pthread_attr_t attr;
	pthread_attr_init(&attr);
//	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	int ret=pthread_create(&tid,&attr,read_msg,(void *)&sockfd);
	if(ret!=0){
		perror("create msg read error..");
		return 2;
	}
	memset(msg,'\0',sizeof(msg));	
	while(read(STDIN_FILENO,msg,MSG_MAX_LEN)){
		int i=0;
		if(strcmp(msg,"bye\n")==0) {
			pthread_kill(tid,SIGINT);
			break;
		}
		write(sockfd,msg,strlen(msg));
		memset(msg,'\0',sizeof(msg));
	}
	pthread_join(tid,NULL);
	close(sockfd);
}
