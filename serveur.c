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

    //Server main only takes a single argument : path to the graph file.
    if (argc > 2 || argc < 2) 
    {
        printf("ERR-101 : Invalid number of arguments\n");
        printf("Server program requires only 1 argument (path to graph file)\n");
        return -101;
    }

    //Creates a char[] that represents a String for the graph file path
    char pathToFile[50];
    strcpy(pathToFile, "");
    strcat(pathToFile, argv[1]);

    int** tab;
    
    //Access in READMODE to the file specified by pathToFile variable at launch
    FILE* file = fopen(pathToFile, "r");
    if (file == NULL) {
        printf("ERR-102 : Specified path is incorrect or does not lead to any file\n");
        printf("Please double check the input matches the file path syntax such as folder/graph.txt\n");
        return -102;
    } 

    else 
    {
        char* line = NULL;
        size_t len = 0;
        ssize_t read;
        int vertices = 0;

        // Read content of chosen file using getread()
        // Continue to scroll through the file as long as :
        //        - File hasn't ended (getline = -1)
        //        - Graph descriptor hasn't been reached yet (find a line that starts with 'p')
        
        bool startOfFileFound = false;

        //Read file line-by-line and loop to ignore initial lines
        while (startOfFileFound == false)
        {
            read = getline(&line, &len, file);

            //If read fails or file is finished before 'p' is found at start of line
            if (read == -1)
            {
                printf("ERR-103 : Unexpected end of file.\n");
                printf("Please make sure the chosen file is of the right format.\n");
                return -103;
            }

            //Stop looping if all lines have been skipped as intended
            if (line[0] == 'p') startOfFileFound = true;
        }

        // Fetch amount of vertices
        sscanf(line, "p edge %i", &vertices);
        printf("Found graph has %i vertices\n", vertices);
        vertices += 1;

        // Two dimensional matrix links a specific leaf with its neighbours (using 0's and 1's)
        int** voisins = (int **) malloc (vertices * sizeof(int*));

        for(int i = 0; i < vertices; i++) {
            voisins[i] = (int *) malloc (vertices * sizeof(int));
        }

        for(int i = 0; i < vertices; i++) {
            for(int j = 0; j < vertices; j++) {
                voisins[i][j] = 0;
            }
        }

        while ((read = getline(&line, &len, file)) != -1) {

                int s1 = 0;
                int s2 = 0;

                sscanf(line, " e %i %i", &s1, &s2);

                voisins[s1][s2] = 1;
                voisins[s2][s1] = 1;


        int ds = socket(PF_INET, SOCK_STREAM, 0);
        if (ds == 0){
                perror("ERR : Opening a socket went wrong.");
                exit(1);
        }

        struct sockaddr_in ad;
        ad.sin_family = AF_INET;
        ad.sin_addr.s_addr = INADDR_ANY;
        ad.sin_port = htons((short) 1099);

        int res;
        res = bind(ds, (struct sockaddr*)&ad, sizeof(ad));
        if (res == -1) {
            printf("Err : Binding process failed %i \n", res);
            exit(1);
        }

        int resListen = listen(ds, vertices);
        if (resListen == -1) {
            printf("ERR : Socket server failed to start listening\n");
            exit(1);
        }

        struct sockaddr_in sockClient;
        socklen_t lgAdr;

        int clientsAcceptes=0;
        while (clientsAcceptes<vertices) {

            int dsClient = accept(ds, (struct sockaddr*) &sockClient, &lgAdr);
            clientsAcceptes++;

            char str[INET_ADDRSTRLEN];

            int port_client = ntohs(sockClient.sin_port);
            printf("Client %i just connected\n",port_client);

            //Checking out the matrix
            /*int id = port_client - 2000;
            printf("Display neighbours of process %i : ", id);
            for(int i = 0; i < vertices; i++){
                printf("%i ", voisins[id][i]);
            }*/

            printf("\n \n");

            int messageSent;
            messageSent=-2;

            //TEST VALUES to connect two clients together
            if (port_client==2235)
            {
                messageSent=-1;
            }

            if (port_client==2248)
            {
                messageSent=2235;
            }

            printf("Attempting to send %d to client %i\n", messageSent, port_client);
            int sd = send(dsClient, &messageSent, sizeof(int), 0);


            if (sd == -1) {
                printf("The sent message didn't get through \n");
            }

        }

        printf("Graph has been built, closing server socket\n");
        close(ds);

        }
    }

    return 0;
}
