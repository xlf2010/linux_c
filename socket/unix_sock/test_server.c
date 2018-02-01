#include<stdio.h>
#include<unistd.h>
#include<sys/un.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string.h>
#include<errno.h>
#define MAX_BUF 100
#define SERVER_PORT 80
typedef struct{
	int fd;
	struct sockaddr_in *addr;
}msg_arg;

void* write_msg(void* arg){
	msg_arg *msg=(msg_arg *)arg;
	char buf[MAX_BUF];
	int fd=msg->fd;
	while(read(STDIN_FILENO,buf,MAX_BUF)){
		if(strcmp("bye\n",buf)==0)break;
		write(fd,buf,strlen(buf));
	}
	close(fd);
}

void* handle_msg(void* arg){
	msg_arg *msg=(msg_arg *)arg;
	char buf[MAX_BUF];
	int fd=msg->fd;
	int finished=0;
	while(finished==0){
		memset(buf,'\0',sizeof(buf));
		printf("client ip : %s,port:%d,say:\"",inet_ntoa(msg->addr->sin_addr),ntohs(msg->addr->sin_port));
		int n=-1;
		while((n=read(fd,buf,MAX_BUF))){
			if(n==-1){
				perror(strerror(errno));
				finished=1;
				break;
			}
			printf("%s",buf);
			if(strcmp("bye\r\n",buf)==0) {
				finished=1;
				break;
			}
			int i=0;
			for(i=0;i<strlen(buf);i++) printf("%d ",buf[i]);
			if(n!=MAX_BUF) break;
		}
		printf("\"\n");
		if (n==0) break;
	}
	close(fd);
	return (void *)0;
}

int main(int argc,char **argv){
	struct sockaddr_un server_addr;
	int listenfd;
	char buf[MAX_BUF];
	listenfd=socket(AF_UNIX,SOCK_STREAM,0);
	if(listenfd<0){
		perror("create socket fd error");
		return 1;
	}
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sun_family=AF_UNIX;
	strcpy(server_addr.sun_path,/tmp/test_unix_sock);

	int bind_res=bind(listenfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
	if(bind_res<0){
		perror("bind fd error");
		return 2;
	}	
	if(listen(listenfd,20)<0){
		perror("listening error");
		return 3;
	}
//	char *msg="got it";
	while(1){	
		struct sockaddr_in client_addr;
		int addrlen = sizeof(client_addr);
		int connfd=accept(listenfd,(struct sockaddr *)&client_addr,&addrlen);
		msg_arg arg;
		arg.fd=connfd;
		arg.addr=&client_addr;
		pthread_t tid;
		if(pthread_create(&tid,NULL,handle_msg,&arg)!=0){
			perror("create msg handle error ");
			continue;
		}
		pthread_detach(tid);
		//printf("connected , client ip:%s,port:%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		//int n=0;
		//printf("read from client : ");
		//while(n=read(connfd,buf,MAX_BUF)){
		//	printf("%s",buf);
		//}
		//printf("\n");
		//write(connfd,msg,strlen(msg));
		//close(connfd);
	}

	return 0;
}
