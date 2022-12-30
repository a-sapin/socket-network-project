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
#include <pthread.h>

int lifetime = 120; //Duration for the server program

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
	int lis = listen(dSock, 100);
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
			  		//int msgToSend = 5;
					//int sd = send(dSClient, &msgToSend, sizeof(msgToSend), 0);

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
	sleep(1);
	int clients2ndDS[clients_awaited];
	
	for (int i = 0; i < clientsJoined; ++i)
  	{
		//Code to SEND a message to every client  		
					/*int msgToSend = 5;
					printf("Sending 5 to client %d \n", clientsDSArray[i]);
			    	int sd = send(clientsDSArray[i], &msgToSend, sizeof(msgToSend), 0);
			    	perror("Send ");
			    	*/
  		int portAnswered;

  		int rec = recv(clientsDSArray[i], &portAnswered, sizeof(int), 0);
  		if (rec==-1) perror("RECEIVE :");
  		printf("Fetched answer %d from client#%d\n", portAnswered, i);
  		clients2ndDS[i] = portAnswered;

  		//TEMP CODE//
  			if(i>0)
  			{
  				int sd = send(clientsDSArray[i-1], &portAnswered, sizeof(portAnswered), 0);
  				printf("\tSending %d to client %d \n", portAnswered, i-1);
  			}




  	}

  	//SERVER STAYS ACTIVE (with all sockets) FOR ITS LIFETIME VARIABLE IN SECONDS
  		if (lifetime<0)
  		{
  			while(true)
  			{

  			}
  		}
  		else sleep(lifetime);
	


	

}