#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

int main()
{
	struct in_addr ip;
	char addr1[] = "192.168.1.1";
	char addr2[] = "255.255.255.255";
	char addr3[] = "192.16.1";
	char *str=NULL, *str2 = NULL;
	int err = 0;
	
	err = inet_aton(addr1,&ip);
	if (err){
		printf("inet_aton:string %s value:0x%x\n",addr1,ip.s_addr);
	}else{
		printf("inet_aton err!\n");
	}
	
	/*inet_addr是inet_aton的缩减版，由于-1同时可以理解为非法地址255.255.255.255
	 *所以不能使用这个函数转换IP地址为255.255.255.255
	*/
	ip.s_addr = inet_addr(addr1);
	if(err != -1){
		printf("inet_addr:string %s value:0x%x\n",addr1,ip.s_addr);
	}else{
		printf("inet_addr err!\n");
	}
	ip.s_addr = inet_addr(addr2);
	if(ip.s_addr != -1){
		printf("inet_addr:string %s value:0x%x\n",addr2,ip.s_addr);
	}else{
		printf("inet_addr err!\n");
	}
	
	/*inet_ntoa为不可重入函数
	 *inet_ntoa:ip:0xffffffff latter:255.255.255.255 pre:255.255.255.255
	 */
	ip.s_addr = 192<<24|168<<16|1<<8|1;
	str = inet_ntoa(ip);
	ip.s_addr = 255<<24|255<<16|255<<8|255;
	str2 = inet_ntoa(ip);
	printf("inet_ntoa:ip:0x%x latter:%s pre:%s \n",ip.s_addr,str2,str);
	
	/*inet_addr不能转换字段不全的字符串
	 *inet_addr:string 192.168.1.1 value:0x10010c0
	 *inet_ntoa:ip:0x10010c0 str:192.16.0.1
	*/
	ip.s_addr = inet_addr(addr3);
	if(err != -1){
		printf("inet_addr:string %s value:0x%x\n",addr1,ip.s_addr);
	}else{
		printf("inet_addr err!\n");
	}
	str = inet_ntoa(ip);
	printf("inet_ntoa:ip:0x%x str:%s \n",ip.s_addr,str);
	
	
	
	return 0;
}