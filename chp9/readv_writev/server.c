#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/uio.h>
#include <unistd.h>

#define PORT 9999
#define BACKLOG 2

void process_conn_server(int s)
{
	char buffer[30];
	ssize_t size =0;
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
		size = readv(s,v, 3);
		if(size == 0 ){
			return;
		}
		sprintf(v[0].iov_base, "%d ", size);
		sprintf(v[1].iov_base, "bytes alt");
		sprintf(v[2].iov_base, "ogether\n");
		v[0].iov_len = strlen(v[0].iov_base);
		v[1].iov_len = strlen(v[1].iov_base);
		v[2].iov_len = strlen(v[2].iov_base);
		writev(s,v,3);
	}
	
}

int main(int argc,char **argv)
{
	int ss,sc;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int err;
	pid_t pid;

	/*建立一个流式套接字*/
	ss = socket(AF_INET, SOCK_STREAM, 0);
	if(ss < 0){
		printf("socket error\n");
		return -1;
	}

	/*设置服务端地址*/
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	/*本地地址*/
	server_addr.sin_port = htons(PORT);

	/*绑定套接字到套接字描述符*/
	err = bind(ss,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(err < 0){
		printf("bind error\n");
		return -1;
	}

	/*设置侦听*/
	err = listen(ss, BACKLOG);
	if(err < 0){
		printf("listen error\n");
		return -1;
	}

	for(;;){
		socklen_t addrlen = sizeof(struct sockaddr);
		sc = accept(ss,(struct sockaddr*)&client_addr,&addrlen);
		if(sc < 0){
			continue;
		}

		/*建立一个新的进程处理连接*/
		pid = fork();
		if(pid ==0){
			close(ss);
			process_conn_server(sc);
		}else{
			close(sc);
		}
	}
	
}


