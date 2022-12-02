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



    if (argc > 3) 
    {
        printf("ERR-101 : Invalid number of arguments\n");
        printf("first argument : client port; second : target port\n");
        return -101;
    }


    int ds = socket(PF_INET, SOCK_STREAM, 0);
    if (ds == -1){
            perror("ERR : Socket creation went wrong.");
            perror("cause:");
            exit(1);
    }

    struct sockaddr_in ad;
    ad.sin_family = AF_INET;
    ad.sin_addr.s_addr = INADDR_ANY;

    //SOCKET PORT IS SET TO BE THE FIRST ARGUMENT THAT THIS PROGRAM WAS CALLED WITH
    if (argc > 1)
    {
            printf("Création de socket %d", atoi(argv[1]));
            if(atoi(argv[1]) != -1) ad.sin_port = htons((short) atoi(argv[1]));
    }

    if (argc == 1)
    {
        printf("Création de socket 4000");
        ad.sin_port = htons(4000);
    }

    //Bind socket
    int res;
    res = bind(ds, (struct sockaddr*)&ad, sizeof(ad));
    if (res == -1) {
        printf("ERR : Binding the socket went wrong.\n");
        perror("cause:");
        exit(1);
    }




    if (argc == 3)
    {
    struct sockaddr_in sockServ;
    sockServ.sin_family = AF_INET;
    sockServ.sin_port = htons((short) atoi(argv[2]));
    socklen_t lgAdr = sizeof(struct sockaddr_in);

    int resConnexion = connect(ds, (struct sockaddr *)&sockServ, lgAdr);
    if (resConnexion == -1) {
        printf("ERR : Connection to other process went wrong\n");
        perror("cause:");
        exit(1);
        }
    else printf("Connection successful!");
    }
    else
    {
        int resListen = listen(ds, 1000); //Go into listening mode
        perror("err :");
        if (resListen == -1) perror("ERR : Socket failed to listen ");

        struct sockaddr_in sockClient;
        socklen_t lgAdr;
        int dsClient = accept(ds, (struct sockaddr*) &sockClient, &lgAdr);
        if (dsClient == -1) perror("FAILURE :");
        else printf("Successful connexion!");
    }
    


    

    close(ds);
    return 0;
}
