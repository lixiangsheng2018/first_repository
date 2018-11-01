#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9999

void process_conn_client(int s)
{
	ssize_t size = 0;
	char buffer[1024] = {0};
	
	for(;;){
		size = read(0, buffer, 1024);/*从标准输入读取数据到buffer*/
		if(size > 0){
			write(s,buffer,size);
			size = read(s,buffer,1024);
			write(1,buffer,size);
		}
	}
}

int main(int argc,char **argv)
{
	int s;
	struct sockaddr_in server_addr;
	
	s = socket(AF_INET, SOCK_STREAM, 0);
	if(s < 0){
		printf("socket error\n");
		return -1;
	}

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	/*本地地址*/
	server_addr.sin_port = htons(PORT);

	/*将输入的字符串转为IP地址类型*/
	inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
	connect(s,(struct sockaddr*)&server_addr,sizeof(struct sockaddr));
	process_conn_client(s);
	
	close(s);
	return 0;
}
