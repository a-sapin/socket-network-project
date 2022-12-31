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
	//FILEPATH ARGUMENT SECTION
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


	//OPENING AND READING THE FILE
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
        }









	int clients_awaited = vertices;
	int dSock = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in ad;
	ad.sin_family = AF_INET;
	ad.sin_addr.s_addr = INADDR_ANY;
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



			printf("Registered as client #%d!\n", clientsJoined);
			int sd = send(dSClient, &clientsJoined, sizeof(clientsJoined), 0);
			
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