#include<stdio.h>
static int add(int a,int b){
	return a+b;
}

int main(int argc,char *argv[]){
	int c=add(1,2);
	printf("c=%d",c);
	return 0;
}
