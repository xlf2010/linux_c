#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

static int cnt=0;
static pthread_mutex_t mutex_t;
void* count( void* arg){
	int i=0;
	int* val=(int *)arg;
	for(i=0;i<10000;i++){
		pthread_mutex_lock(&mutex_t);
		*val=cnt;
		printf("thread id=%u,count=%d\n",pthread_self(),(*val)+1);
		cnt=(*val)+1;
		pthread_mutex_unlock(&mutex_t);
		//	sleep(1);
	}
	pthread_exit(val);
	return NULL;
}

int main(int argc,char **argv){
	pthread_t t1,t2;
	int err;
	int res1,res2;
	pthread_mutex_init(&mutex_t,NULL);
	err=pthread_create(&t1,NULL,count,&res1);
	if(err!=0){
		perror("cann't create thread");
		return 1;
	}
	err=pthread_create(&t2,NULL,count,&res2);
	if(err!=0){
		perror("can't create thread");
		return 2;
	}
	void *ret1,*ret2;
	pthread_join(t1,&ret1);
	pthread_join(t2,&ret2);
	printf("res1=%d,res2=%d,ret1=%d,ret2=%d\n",res1,res2,*(int *)ret1,*(int *)ret2);
	return 0;
}
