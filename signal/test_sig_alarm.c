#include<stdio.h>
#include<unistd.h>
#include<signal.h>

int main(int argc,char** argv){

	int count=0;
	
	//declare a alram set
	sigset_t alarm_set,unhandle_set;
	//initial alaram set
	sigemptyset(&alarm_set);
	//add SIGALRAM to set
	sigaddset(&alarm_set,SIGALRM);
	printf("after invoke sigaddset SIGALARM signal in alarm_set flag:%d\n",sigismember(&alarm_set,SIGALRM));
	//block alarm signal
	sigprocmask(SIG_BLOCK,&alarm_set,NULL);
	alarm(5);
	for(count=0;count<10;count++){
		printf("count=%d\n",count);
		sleep(1);
	}
	//get unhandle signale
	sigpending(&unhandle_set);
	printf("after invoke sigpending SIGALARM signal in alarm_set flag:%d\n",sigismember(&unhandle_set,SIGALRM));
	//unblock alarm signal
	printf("unblock alram signal \n");
	sigprocmask(SIG_UNBLOCK,&alarm_set,NULL);
	
	return 0;
}
