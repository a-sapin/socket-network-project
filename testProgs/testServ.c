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
	int clients_awaited = 3;
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

	int clientsDSArray[clients_awaited];
	int clientsJoined = 0;

	while (clientsJoined<clients_awaited)
	{
		int dSClient = accept(dSock, (struct sockaddr *) &adClient, &lgA);

		if (dSClient!=-1)
		{
			//TESTIFICATE


			inet_ntop(AF_INET, &(adClient.sin_addr), str, INET_ADDRSTRLEN);
			int temp = ntohs(adClient.sin_port);
			printf("Client %s:%d just connected!\n", str, temp);
			printf("Socket descriptor : %d\n", dSClient);
			//Adding socket-to-client to array
			clientsDSArray[clientsJoined] = dSClient;
			clientsJoined++;
		}
		else perror("Accept problem");
	}

	printf("All awaited clients are logged in!\n");


	for (int i = 1; i < clientsJoined; ++i)
  	{
  		int msgToSend = 5;

		printf("Sending 5 to client %d \n", clientsDSArray[clientsJoined]);
    	int sd = send(clientsDSArray[clientsJoined], &msgToSend, sizeof(msgToSend), 0);
    	perror("Send ");
  	}



	

}