#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define MAX_PRODUCT_NUM 10

typedef struct product {
	char pro_name[100];
	struct product *next;
}product_t;

volatile int size=0;
volatile int product_index=0;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_not_empty=PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_not_full=PTHREAD_COND_INITIALIZER;

void* produce(void* arg){
	while(1){
	sleep(1);
	pthread_mutex_lock(&mutex);
	// product link is fulled, wait for consumer
	if(size>=MAX_PRODUCT_NUM){
		printf("queue is full ,wait for consumer");
		pthread_cond_wait(&cond_not_full,&mutex);
	}
	product_t* pro =(product_t *) malloc(sizeof(product_t));
	sprintf(pro->pro_name,"product-%d",product_index++);
	printf("produce product : %s\n",pro->pro_name);
	product_t* head=*(product_t **)arg;
	product_t* tail=*((product_t **)arg+1);
	if(size==0){
		head=tail=pro;
		size++;
		pthread_cond_signal(&cond_not_empty);
	}else{
		tail->next=pro;
		tail=pro;
		size++;
	}
	*(product_t **)arg=head;
	*((product_t **)arg+1)=(void *)tail;
	pthread_mutex_unlock(&mutex);

	}
}

void* consume(void* arg){
//	sleep(10);
	while(1){
//	sleep(1);
	pthread_mutex_lock(&mutex);
	if(size<=0){
		// wait for produce
		printf("queue is empty, wait for producer");
		pthread_cond_wait(&cond_not_empty,&mutex);
	}	
	product_t* head=*(product_t **)arg;
	product_t* tail=*((product_t **)arg+1);
	product_t* t=head;
	head=head->next;
	if(size==MAX_PRODUCT_NUM){
		size--;
		pthread_cond_signal(&cond_not_full);
	}else{
		size--;
	}
	printf("consume data=%s\n",t->pro_name);
	free(t);
	*(product_t **)arg=head;
	*((product_t **)arg+1)=tail;
	pthread_mutex_unlock(&mutex);
	}
}

int main(int argc,char **argv){
	pthread_t prod_pt,cons_pt;
	int err;
	void* arg[2];
	product_t *head,*tail;
	arg[0]=head;
	arg[1]=tail;
	err=pthread_create(&prod_pt,NULL,produce,arg);
	err=pthread_create(&cons_pt,NULL,consume,arg);
	pthread_join(prod_pt,NULL);
	pthread_join(cons_pt,NULL);
	return 0;
}
