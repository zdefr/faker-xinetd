#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

//写http头
void rehead(char* pro, char* num, char* desc, char* type, char* encod){
	printf("%s %s %s\r\n",pro,num,desc);
	printf("Content-Type:%s; charset:%s\r\n",type,encod);
	printf("\r\n");
}

//报错
void netperror(char* err, char* pro){
	rehead(pro,"200","ok","text/html","utf-8");
	printf("<html>\n");
	printf("<meta http-equiv=\"Content-Type\" content=\"test/html;charset=utf-8\"/>");
	printf("<head><title>error</title></head>");
	printf("<body><p>%s</p></body>",err);
	printf("</html>\n");
	fflush(stdout);
}

//读目录
void redir(char* dirpath, char* pro, char* dir){
	DIR *pdir = opendir(dirpath);
	if(pdir==NULL){
		perror("opendir");
		exit(1);
	}
	struct dirent *dirs=NULL;
	rehead(pro,"200","ok","text/html","utf-8");
	printf("<html>\n");
	printf("<meta http-equiv=\"Content-Type\" content=\"test/html;charset=utf-8\"/>");
	printf("<head><title>%s</title></head>",dir);
	printf("<body>");
	printf("<p>目录</p>");
	dirs=readdir(pdir);
	while(dirs!=NULL){
		printf("<p>%s</p>",dirs->d_name);
		dirs = readdir(pdir);
	}
	printf("</body>");
	printf("</html>\n");
	fflush(stdout);
}

//主函数
int main(){
	char mid[100];
	char pro[20],met[50],dir[50];
	char buf[50];
	int count;
	struct stat sbuf;

	//int ow =  open("/fhttp-log",O_WRONLY | O_APPEND | O_CREAT);	//写日志
	
	//读http请求
	fgets(mid,100,stdin);
	sscanf(mid,"%s %s %s",met,dir,pro);
	do{
		fgets(mid,100,stdin);
	//	write(ow,mid,100);
	}while(strcmp(mid,"\r\n")!=0);
	
	
	//文件是否存在
	if(strcmp(met,"/")==0){
		strcpy(dir,"/page/index.html");
	}
	if(stat(dir,&sbuf)<0){
		netperror("404\n没有这个目录",pro);
		exit(1);
	}

	//处理目录
	if(S_IFDIR){
		//pdir = opendir(met);
		redir(dir,pro,"dir");
		exit(1);
	}
/*	printf("%s 200 OK\r\n",pro);
	printf("Content-Type:test/plain;charset:iso-8859-1\r\n");
	printf("\r\n");
	int dw = open("/home/ubuntu/faker-xinetd/service/service.c",O_RDONLY);
	do{
		count = read(dw,(void*)buf,50);
		write(fileno(stdout),(void*)buf,count);
	}while(count==50);
	fflush(stdout);
*/

}
