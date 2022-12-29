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

	int dSock = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in ad;
	ad.sin_family = AF_INET;

	if (argc == 1)
	{
		ad.sin_addr.s_addr = INADDR_ANY;
	}
	if (argc == 2)
	{
		inet_pton(AF_INET, argv[1], &(ad.sin_addr));

	}
	ad.sin_port = htons((short)7777);


	int res = bind(dSock, (struct sockaddr*)&ad, sizeof(ad));
	if(res==-1) perror("BINDING ERR ");

	char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(ad.sin_addr), str, INET_ADDRSTRLEN);
	printf("Server is using IP %s \n", str);


	//Listening now
	int lis = listen(dSock, 1);
	if (lis==-1)
	{
		perror("LISTEN ERR ");
	}
	else
	{
		printf("Server is ready and bound!\n");
	}


	struct sockaddr_in adClient;
	socklen_t lgA = sizeof(struct sockaddr_in);
	printf("=Accept() starting now=\n");
	int dSClient = accept(dSock, (struct sockaddr *) &adClient, &lgA);

}