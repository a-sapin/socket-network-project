#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <curses.h>

int main(int argc, char *argv[]) 
{

	if (argc!= 2)
	{
		perror("You need exactly one parameter (IP address of server network).");
		exit(-1);
	}
	int dSock = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in ad;
	ad.sin_family = AF_INET;
	ad.sin_addr.s_addr = INADDR_ANY;
	socklen_t lgA = sizeof(struct sockaddr_in);


	struct sockaddr_in adServ ;
	adServ.sin_family = AF_INET; 
	adServ.sin_port = htons(7777);
	inet_pton(AF_INET, argv[1], &(adServ.sin_addr));

	//int res = inet_pton(AF_INET, ”197.50.51.10”, &(adServ.sin_addr));
	//socklen_t_lgA = sizeof(struct sockaddr_in);
	int res = connect(dSock, (struct sockaddr *) &adServ, lgA);

	//Upon finish connect()
	int recvdMsg;
	int rec = recv(dSock, &recvdMsg, sizeof(int), 0);
	if (rec==-1)
	{
		perror("Reception issue");
	}
	printf("Receive value %d\n", recvdMsg);
}