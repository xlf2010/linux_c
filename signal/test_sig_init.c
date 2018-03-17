#include<stdio.h>
#include<unistd.h>
#include<signal.h>
struct sigaction sa;

void init_process(int signo){
	printf("receive signal : %d ....\n",signo);
	int i,j;
	for(i=0;i<5;i++){
		printf("handling sig init signal, time cost : %d\n",i);
		for(j=1;j<=32;j++){
			printf("%d",sigismember(&sa.sa_mask,j));
		}
		printf("\n");
		sleep(1);
	}
}

int main(int argc,char** argv){
	//signal(SIGINT,init_process);
	//pause();
	//signal(SIGINT,init_process);
	//signal(SIGILL,init_process);
	struct sigaction old;
	
	//init the block signal while function sighandler_t was running
	sigemptyset(&sa.sa_mask);
	// block signal
	sigaddset(&sa.sa_mask,SIGQUIT);
//	sigaddset(&sa.sa_mask,SIGINT);
	// set handle function
	sa.sa_handler=init_process;
//	sa.sa_flags=0;
	//nodefer means restart process handler
	sa.sa_flags=SA_NODEFER;
	//register signal handler
	sigaction(SIGINT,&sa,&old);

	pause();

	printf("old contain sigquit signal:%d\n",sigismember(&old.sa_mask,SIGQUIT));

//	printf("pause return:%d\n",pause());
	return 0;
}
