#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>

#define MSG_MAX_LEN 100
#define SERVER_IP "10.0.2.15"
#define SERVER_PORT 80

void read_msg(void *arg){
	int fd=*(int *)arg;
	char msg[MSG_MAX_LEN];
	int size=sizeof(msg);
	while(1){
		int len=read(fd,msg,size);
		if(len==0){
			printf("socket close,");
			break;
		}
	}
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
	memset(msg,'\0',sizeof(msg));
	while(read(STDIN_FILENO,msg,MSG_MAX_LEN)){
		int i=0;
//		write(sockfd,msg,strlen(msg));
		if(strcmp(msg,"bye\n")==0) {
			break;
		}
		write(sockfd,msg,strlen(msg));
		memset(msg,'\0',sizeof(msg));
	}
	close(sockfd);
	sleep(4);
}
