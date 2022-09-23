#include <sys/types.h> 
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/un.h>

#define SOCKET "srvr_sock.soc"
#define MSG_SIZE 256

char *srvr_ans = "OK";

static int sock_fd;

void sigint_handler(int sig) 
{
    close(sock_fd);
    unlink(SOCKET);
    exit(0);
}

int main(void)
{
	sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sock_fd == -1)
	{
		perror("Socket");
		return -1;
	}
	
	struct sockaddr addr;
	addr.sa_family = AF_UNIX;
	strcpy(addr.sa_data, SOCKET);
	
	if (bind(sock_fd, &addr, sizeof(addr)) == -1)
	{
		perror("Bind");
		return -1;
	}
	
	char msg[MSG_SIZE];
	struct sockaddr rcvr_name;
	int namelen = sizeof(rcvr_name);
	
	signal(SIGINT, sigint_handler);
	
	while(1)
	{
		int len = recvfrom(sock_fd, msg, MSG_SIZE - 1, 0, &rcvr_name, &namelen);
		sendto(sock_fd, srvr_ans, strlen(srvr_ans), 0, &rcvr_name, sizeof(rcvr_name));
		msg[len] = '\0';
        printf("Message from client: %s.\n", msg);
	}
	
	return 0;
}