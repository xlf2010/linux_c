#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<errno.h>
#include<string.h>
#include<sys/epoll.h>

#define SERVER_PORT 80
#define MAX_CONNECTIONS 20

#define MAX_FD_LISTEN 1024
#define EPOLL_WAIT_TIMEOUT 10 * 1000

#define MAX_BUF 100

void print_error(const char * msg,int errno){
	printf("%s:%s\n",msg,strerror(errno));
	exit(1);
}

void handle_active_fd_in(){}

void handle_active_fds(int epoll_fd,int listenedfd,struct epoll_event* event){	
	if(event->events & EPOLLIN){
		if(event->data.fd==listenedfd){
			struct sockaddr_in client_addr;
			socklen_t client_sock_len;
			int connfd=accept(listenedfd,(struct sockaddr * )&client_addr,&client_sock_len);
			printf("client addr:%s,port:%d has connected....\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
			// add to epoll
			struct epoll_event ev;
			ev.events = EPOLLIN;
			ev.data.fd=connfd;
			epoll_ctl(epoll_fd,EPOLL_CTL_ADD,connfd,&ev);
		}else{
			char buf[MAX_BUF];
			memset(buf,'\0',sizeof(buf));
			int len=read(event->data.fd,buf,sizeof(buf[0])*MAX_BUF);
			// read from socket len =0 means client has close connection,delete it from epoll
			if(len<=0){
				epoll_ctl(epoll_fd,EPOLL_CTL_DEL,event->data.fd,NULL);
				close(event->data.fd);
				return;
			}
			printf("read from client : %s\n",buf);
			struct epoll_event ev;
			ev.events=EPOLLOUT;
			ev.data.fd=event->data.fd;
			epoll_ctl(epoll_fd,EPOLL_CTL_MOD,event->data.fd,&ev);
		}
	}else if(event->events & EPOLLOUT){
		//printf("socket fd:%d can write \n",event->data.fd);
		char *reply="got the msg,now reply\n";
		write(event->data.fd,reply,sizeof(char)*strlen(reply));
		struct epoll_event ev;
		ev.data.fd=event->data.fd;
		ev.events=EPOLLIN;
		epoll_ctl(epoll_fd,EPOLL_CTL_MOD,event->data.fd,&ev);
	}else{
		printf("not support epoll type\n");
	}
}

int main(int argc,char** argv){
	struct sockaddr_in server_addr;
	int listenedfd=socket(AF_INET,SOCK_STREAM,0);
	if(listenedfd<0){
		printf("create a socket fd fail,fail msg : %s\n",strerror(errno));
		return 1;
	}
	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(SERVER_PORT);

	int ret = bind(listenedfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
	if(ret==-1){
		printf("listened fd:%d,invoked bind error: errmsg:%s\n",listenedfd,strerror(errno));
		return 2;
	}
	//create epoll fd
	int epoll_fd=epoll_create(MAX_FD_LISTEN);
	//create epoll_event
	struct epoll_event ev,evs[MAX_FD_LISTEN];
	ev.events=EPOLLIN;
	ev.data.fd=listenedfd;
	//add event to epoll
	epoll_ctl(epoll_fd,EPOLL_CTL_ADD,listenedfd,&ev);

	ret = listen(listenedfd,MAX_CONNECTIONS);
	if(ret==-1){
	    printf("listened fd:%d,invoked listen error: errmsg:%s",listenedfd,strerror(errno));
        return 3;
    }   

	int i=0;
	while(1){
		int active_fd=epoll_wait(epoll_fd,evs,MAX_FD_LISTEN,EPOLL_WAIT_TIMEOUT);
		if(active_fd<=0){ 
//			printf("cannt find any active fd,continue...\n");
			continue;
		}
		for(i=0;i<active_fd;i++){
			handle_active_fds(epoll_fd,listenedfd,&evs[i]);
			//			if(evs[i].events & EPOLLIN){
//				if(evs[i].events.fd==listenedfd){
//					
//				}
//			}
		}
	}

	return 0;
}
