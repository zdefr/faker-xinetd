#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(){
	char mid[100];
	char pro[10],met[10],dir[50];
	char buf[50];
	int count;
	sscanf("%s %s %s",met,dir,pro,3);
	do{
		fgets(mid,100,stdin);
	}while(strcmp(mid,"\r\n")!=0);
	
	printf("%s 200 OK\r\n",pro);
	printf("Content-Type:test/plain;charset:iso-8859-1\r\n");
	printf("\r\n");
	int dw = open("/home/zde/dir/tcp/service.c",O_RDONLY);
	do{
		count = read(dw,(void*)&buf,50);
		write(fileno(stdout),(void*)&buf,count);
	}while(count==50);
	fflush(stdout);
}
