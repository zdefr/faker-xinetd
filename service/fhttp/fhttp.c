#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
void split(char *src,const char *separator,char dest[][60],int *num) {
      char *pNext;
      int count = 0;
      if (src == NULL || strlen(src) == 0)
         return;
      if (separator == NULL || strlen(separator) == 0)
         return;    
      pNext = strtok(src,separator);
      while(pNext != NULL) {
           strcpy(dest[count],pNext);
           ++count;
           pNext = strtok(NULL,separator);  
     }  
     *num = count;
}     

//写http头
void rehead(char* pro, char* num, char* desc, char* type, char* encod){
	printf("%s %s %s\r\n",pro,num,desc);
	printf("Content-Type:%s; charset:%s\r\n",type,encod);
	printf("\r\n");
	fflush(stdout);
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
		netperror("not found this dir",pro);
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

void rehtml(char* pro,char* dir){
	char buf[100];
	int count;
	int hw = open(dir,O_RDONLY);
	if(hw<0){
		netperror("miss dir",pro);
		exit(1);
	}
	rehead(pro,"200","ok","text/html","utf-8");
	do{
		count = read(hw,(void*)buf,100);
		write(fileno(stdout),(void*)buf,count);
	}while(count==100);
	fflush(stdout);
}

void repict(char* pro,char* dir){
	char buf[100];
	int count;
	int hw = open(dir,O_RDONLY);
	if(hw<0){
		netperror("miss dir",pro);
		exit(1);
	}
	rehead(pro,"200","ok","image/jpeg","base64");
	do{
		count = read(hw,(void*)buf,100);
		write(fileno(stdout),(void*)buf,count);
	}while(count==100);
}

//主函数
int main(){
	char mid[100];
	char pro[20],met[50],dir[50];
	char buf[50];
	char rdir[70];
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
	if(getcwd(rdir,70)==NULL){
		netperror("获取目录失败",pro);
		exit(1);
	}
	rdir[strlen(rdir)-8]='\0';
	strcat(rdir,"/bin");
	
	if(strcmp(dir,"/")==0){
		strcat(rdir,"/page/index.html");
	}else{
		strcat(rdir,dir);
	}
	
	//处理错误
	if(stat(rdir,&sbuf)<0){
		netperror(rdir,pro);
		exit(1);
	}
	
	//处理目录
	if(S_ISDIR(sbuf.st_mode)){
		//pdir = opendir(met);
		redir(rdir,pro,"dir");
		exit(1);
	}

	if(sbuf.st_mode & S_IFREG){
		char fdir[70];
		char tail[10][60];
		int num;
		strcpy(fdir,rdir);
		split(fdir,".",tail,&num);
		if(strcmp(tail[num-1],"html")==0){
			rehtml(pro,rdir);
			exit(1);
		}

		if(strcmp(tail[num-1],"jpg")==0){
			repict(pro,rdir);
			exit(1);
		}


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
