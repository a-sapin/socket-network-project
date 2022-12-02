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
	ad.sin_addr.s_addr = INADDR_ANY;


	struct sockaddr_in adServ ;
	adServ.sin_family = AF_INET; 
	adServ.sin_port = htons(7777);

	int res = inet_pton(AF_INET, ”197.50.51.10”, &(adServ.sin_addr));
	socklen_t_lgA = sizeof(struct sockaddr_in);
	res = connect(dS, (struct sockaddr *) &adServ, lgA);

}