#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>
#include<pthread.h>

void handle_signal(int signo){
	printf("tid :%lu,got the signal:%d\n",pthread_self(),signo);
}

void* thread_fun(void *arg){
	struct sigaction sig;
	sigemptyset(&sig.sa_mask);
	sigaddset(&sig.sa_mask,SIGINT);
	sig.sa_handler=handle_signal;
	sigaction(SIGINT,&sig,NULL);
	while(1){
		pause();
	}
}


int main(int argc,char** argv){
	pthread_t t1,t2;
	pthread_create(&t1,NULL,thread_fun,NULL);
	pthread_create(&t2,NULL,thread_fun,NULL);
	sleep(5);
	pthread_kill(t1,SIGINT);
	pthread_kill(t2,SIGINT);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	return 0;
}
