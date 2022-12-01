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

int main(int argc, char *argv[]) {

    int ds = socket(PF_INET, SOCK_STREAM, 0);
    int ds2 = socket(PF_INET, SOCK_STREAM, 0);
    if (ds == -1){
            perror("ERR : Socket creation went wrong.");
            exit(1);
    }

    struct sockaddr_in ad;
    ad.sin_family = AF_INET;
    ad.sin_addr.s_addr = INADDR_ANY;

    struct sockaddr_in ad2;
    ad2.sin_family = AF_INET;
    ad2.sin_addr.s_addr = INADDR_ANY;

    //SOCKET PORT IS SET TO BE THE FIRST ARGUMENT THAT THIS PROGRAM WAS CALLED WITH
    if(atoi(argv[1]) != -1) {
        ad.sin_port = htons((short) atoi(argv[1]));
        ad2.sin_port = htons((short) atoi(argv[1])*5);
    }

    //Bind socket
    int res;
    res = bind(ds, (struct sockaddr*)&ad, sizeof(ad));
    if (res == -1) {
        printf("ERR : Binding the socket went wrong.\n");
        exit(1);
    }
    res = bind(ds2, (struct sockaddr*)&ad2, sizeof(ad2));
    if (res == -1) {
        printf("ERR : Binding socket2 went wrong.\n");
        exit(1);
    }

    //1099 is the dedicated server port we chose.
    struct sockaddr_in sockServ;
    sockServ.sin_family = AF_INET;
    sockServ.sin_port = htons((short)1099);
    socklen_t lgAdr = sizeof(struct sockaddr_in);

    int resConnexion = connect(ds, (struct sockaddr *)&sockServ, lgAdr);
    if (resConnexion == -1) {
        printf("ERR : Connection to server went wrong. Make sure server is started\n");
        exit(1);
    }


    bool keepGoing = true;
    int errorTolerance = 1000;

    int errorCount=0;

    while(keepGoing)
    {
        //Shut down if program bugs
        if (errorCount>errorTolerance) return 0;

        int serverMessage = 0;

        int err = recv(ds, &serverMessage, sizeof(int), 0);
        if (err == -1) 
        {
            //printf("ERR : Generic message reception error\n");
        }
        else
        {
            //If no error proceed here
        //printf("Message reçu %i \n", serverMessage);

        //If receivedMessage is lower than 0, it means its a special code
        if (serverMessage<=0)
        {
            switch (serverMessage)
            {
                //Code -1 means get ready to accept another connection
                case -1:
                    printf("Server sent code -1, will attempt listen()\n");
                    int resListen = listen(ds, 1000); //Go into listening mode
                    if (resListen != -1) printf("ERR : Socket failed to listen.");
                    struct sockaddr_in sockClient;
                    socklen_t lgAdr;
                        int dsClient = accept(ds, (struct sockaddr*) &sockClient, &lgAdr);
                        if(dsClient!= -1) printf("Accepting connection!");
                        else printf("Accepting the connection encountered a problem.");
                        sleep(4);
                        dsClient = accept(ds, (struct sockaddr*) &sockClient, &lgAdr);
                        if(dsClient!= -1) printf("Accepting connection!");
                        close(ds);
                        close(ds2);
                        exit(0);
                break;

                //Code -2 means close down
                case -2:
                    printf("Server sent code -2, required this socket to close\n");
                    close(ds);
                    close(ds2);
                    keepGoing=false;
                    exit(0);
                break;
                
                default:
            }
        }       //If received message is a positive integer it's a port that this client must attempt to connect to
        else
        {
            printf("Server sent code %i, will attempt requesting connection to socket at that port\n", serverMessage);
            struct sockaddr_in sockNeighbour;
            sockNeighbour.sin_family = AF_INET;
            sockNeighbour.sin_port = htons((short)serverMessage);
            socklen_t lgAdr = sizeof(struct sockaddr_in);

            int resConnexion = connect(ds2, (struct sockaddr *)&sockNeighbour, lgAdr);
            if (resConnexion == -1) 
            {
                printf("ERR : Connection to other client %i socket went wrong. ERR %d\n", serverMessage, errno);
                perror("ERR ");
            }
            else printf("Connection to friend %i successful!", serverMessage);

                sleep(5);
                close(ds);
                close(ds2);
                exit(0);
        }   
        }

        

    }

    close(ds);
    return 0;
}
