#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
int main(){
	
	struct stat buf;
	int ch = stat("/home/zde/faker-xinetd/bin/page/index.html",&buf);
	if(ch<0){
		printf("faild\n");
	}
	printf("%d\n",buf.st_mode);
	if(buf.st_mode & S_IFREG){
		printf("ok\n");
	}
	printf("%o\n",buf.st_mode & S_IFREG);

}
