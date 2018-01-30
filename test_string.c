#include<stdio.h>
#include<string.h>
#include<unistd.h>
#define N 100
int main(int argc,char **argv){
	char input[N];
	read(STDIN_FILENO,input,N);
	write(STDOUT_FILENO,input,strlen(input));	
	printf("inputlen=%d,equal : %d",strlen(input),strcmp("bye\n",input));
	return 0;
}
