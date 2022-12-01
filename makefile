compile :
	gcc serveur.c -o serveur
	gcc launchProcess.c -o launchProcess
	gcc -g client.c -o client

run_server :
	./serveur fichiersGraphes/testGraph.txt

run_clients :
	./launchProcess testGraph.txt
