# Socket Network Project

**Author:** Arthur Sapin

This project was realized in 2022 as an example of a simplistic distributed algorithm, using sockets in C.
The main program parses a graph file written in the given format, and then creates 'child processes' that will have their own sockets to communicate with each other.
They are then bound and connected with each other following the pattern of the graph provided as parameter of program _serveur_.


# Programmation Répartie : Première Partie du Projet
*Arthur Sapin*
*Abla Amareff*

#### Déploiement
L'exécution du code du projet nécessite un environnement de développement/compilation du langage C, inclus par défaut dans la majorité des systèmes Linux.

 1. **Tester le projet à l'aide du graphe exemple**
 Vous pouvez tester compiler et exécuter les programmes à l'aide des commandes du fichier *makefile*.
- Ouvrez deux terminaux séparés.
- Dans le premier terminal, exécutez les commandes 

> make compile
> make run_server

 - Puis exécutez dans le second terminal :
> make run_clients

*Il est important de les exécuter dans cet ordre.*

 2. **Tester le projet avec un autre fichier de graphe**
 - Insérez le fichier de graphe.txt dans le dossier *fichiersGraphes*
 - Puis exécutez :

> ./serveur fichiersGraphes/nomdugraphe.txt
> ./launchProcess nomdugraphe.txt

