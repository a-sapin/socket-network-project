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

int otherClientsDS[5000];
int dSock;
struct sockaddr_in ad;
socklen_t lgA = sizeof(struct sockaddr_in);



void *threadAccept(void *args)
{
	sleep(0.5);
	printf("\tGeneric thread guy! %d\n", * (int*)args);
	return NULL;
}

void *receiveAndConnect(void *args)
{
	int recvdMsg;
	int rec = recv(dSock, &recvdMsg, sizeof(int), 0);
	if (rec==-1)
	{
		perror("\tTHREAD RECEIVE - Reception issue");
	}
	printf("Receive value %d\n", recvdMsg);
}


int main(int argc, char *argv[]) 
{

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
				/*	int recvdMsg;
					int rec = recv(dSock, &recvdMsg, sizeof(int), 0);
					if (rec==-1)
					{
						perror("Reception issue");
					}
					printf("Receive value %d\n", recvdMsg);  */


	//Opening a second socket which will act as server
	int dSockServ = socket(PF_INET, SOCK_STREAM, 0);
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
  	sleep(20);
  	exit(0);
}
