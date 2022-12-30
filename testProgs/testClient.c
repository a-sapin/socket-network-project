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
#include <stdbool.h>

int otherClientsDS[5000];
int dSock;
int dSockServ;

int n=-1;

struct sockaddr_in ad;
socklen_t lgA = sizeof(struct sockaddr_in);



void *threadAccept(void *args)
{
	sleep(0.5);
	struct sockaddr_in adNClient;
	int newDSClient = accept(dSockServ, (struct sockaddr *) &adNClient, &lgA);
	if (newDSClient==-1) perror("Accepting connection issues ");
	else
	{
		int newFriendN;
		int rec = recv(newDSClient, &newFriendN, sizeof(int), 0);
		perror("\tTHREAD RECEIVE FROM CLIENT ");
		printf("\tTHREAD - just accepted connection request by fellow client number %d !\n", newFriendN);

	}
	return NULL;
}

void *receiveAndConnect(void *args)
{
	bool continuity = true;

	while (continuity)
	{
		int recvdMsg = -1;
		int rec = -1;
		rec = recv(dSock, &recvdMsg, sizeof(int), 0);
		if (rec== -1)
		{
			perror("\tTHREAD RECEIVE - Reception issue");
			continuity = false;
		}
		printf("SERVER SENT MESSAGE : %d\n", recvdMsg);

		struct sockaddr_in adNeigh;
		adNeigh.sin_family = AF_INET;
		adNeigh.sin_addr.s_addr = INADDR_ANY;
		adNeigh.sin_port = htons(recvdMsg);
		//inet_pton(AF_INET, argv[1], &(adServ.sin_addr));

		int newS = socket(PF_INET, SOCK_STREAM, 0);
		struct sockaddr_in adNewS;
		adNewS.sin_family = AF_INET;
		adNewS.sin_addr.s_addr = INADDR_ANY;


		int res = connect(newS, (struct sockaddr *) &adNeigh, lgA);
		if (res==-1) perror("Connect problem ");
		else
		{
			printf("\tTHREAD - Successful connection to fellow client socket!\n");
			int a=0;
			while (otherClientsDS[a] != -7)
			{
				a++;
			}
			otherClientsDS[a] = res; //THis is the new socket descriptor that results from the connection

			//Sending a "business card"
			int greetMsg = n;
			res = send(newS, &greetMsg, sizeof(greetMsg), 0);

		}
		sleep(1);

	}
	
}


int main(int argc, char *argv[]) 
{
	for (int i=0; i<(sizeof(otherClientsDS)/sizeof(otherClientsDS[0])); i++)
	{
		otherClientsDS[i] = -7;
	}

	if (argc!= 2)
	{
		perror("You need exactly one parameter (IP address of server network).");
		exit(-1);
	}

	ad.sin_family = AF_INET;
	ad.sin_addr.s_addr = INADDR_ANY;
	dSock = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in adServ ;
	adServ.sin_family = AF_INET; 
	adServ.sin_port = htons(7777);
	inet_pton(AF_INET, argv[1], &(adServ.sin_addr));

	//int res = inet_pton(AF_INET, ”197.50.51.10”, &(adServ.sin_addr));
	//socklen_t_lgA = sizeof(struct sockaddr_in);
	int res = connect(dSock, (struct sockaddr *) &adServ, lgA);

	//Upon finish connect()
	
	//Code to receive msg
				int recvdMsg;
				int rec = recv(dSock, &recvdMsg, sizeof(int), 0);
				if (rec==-1)
				{
						perror("Reception issue");
						exit(-1);
				}
				printf("\t!!!!! Server told me that I'm #%d !!!!!!\n", recvdMsg);
				n = recvdMsg;


	//Opening a second socket which will act as server
	dSockServ = socket(PF_INET, SOCK_STREAM, 0);
	int bnd = bind(dSockServ, (struct sockaddr*)&ad, sizeof(ad));
	if(bnd==-1) perror("BINDING ERR ");
	
	//Listen
	int lis = listen(dSockServ, 100);
	if (lis==-1) perror("LISTEN ERR ");

	//Getting the PORT
	struct sockaddr_in ad_2soc;
    socklen_t len = sizeof(ad_2soc);
    if (getsockname(dSockServ, (struct sockaddr *)&ad_2soc, &len) == -1) perror("getsockname");
    else printf("Auxiliary 'server' client socket opened on port %d\n", ntohs(ad_2soc.sin_port));
    //

    //Sending the PORT number for AUXILIARY SERV SOCKET
    	int msgToSend = ntohs(ad_2soc.sin_port);
		printf("Sending my 2nd socket port to server!\n");
		int sd = send(dSock, &msgToSend, sizeof(msgToSend), 0);
		if (sd==-1) perror("Send ");

	//Starting thread to receive
	printf("Now starting a thread!\n");
	pthread_t id;
	pthread_t id2;
  	int parameter = 21;
  	pthread_create(&id, NULL, threadAccept, &parameter); //Starting thread using method threadAccept() with "parameter" input
  	pthread_create(&id2, NULL, receiveAndConnect, NULL);
  	sleep(30);
  	exit(0);
}
