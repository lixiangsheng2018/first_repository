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
	char buffer[30] = {0};
	int i = 0;
	struct iovec *v = (struct iovec*)malloc(3*sizeof(struct iovec));
	if(!v){
		printf("Not enough memoty !\n");
		return;
	}
	v[0].iov_base = buffer;
	v[1].iov_base = buffer + 10;
	v[2].iov_base = buffer + 20;
	v[0].iov_len = v[1].iov_len = v[2].iov_len = 10;
	for(;;){
		size = read(0, v[0].iov_base, 10);/*从标准输入读取数据到buffer*/
		if(size > 0){
			v[0].iov_len = size;
			writev(s,v,1);
			v[0].iov_len = v[1].iov_len = v[2].iov_len = 10;
			size = readv(s,v,3);
			for(i=0;i<3;i++){
				if(v[i].iov_len > 0){
					write(1,v[i].iov_base,v[i].iov_len);
				}
			}
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
