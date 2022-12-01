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

    char path[40];
    strcpy(path, "fichiersGraphes/");

    strcat(path, argv[1]);

    FILE* file = fopen(path, "r");

    if (file == NULL) {
        perror("ERR : Graph not found\n");
        exit(1);
    } else {

        char* line = NULL;
        size_t len = 0;
        ssize_t read;
        int nb_sommet = 0;

        while (((read = getline(&line, &len, file)) != -1) && (line[0] != 'p')){

        }

        sscanf(line, "p edge %i", &nb_sommet);
        printf("Fetching vertices : %i \n", nb_sommet);

        char cmd[40];

        int port = 2000;

        for(int i = 0; i < nb_sommet; i++) {

            memset(cmd, 0, 40);
            sprintf(cmd, "./client %d &", port);
            printf("%s \n", cmd);
            port++;
            // Launching through CMD

            system(cmd);


        }

    }

    return 0;
}
