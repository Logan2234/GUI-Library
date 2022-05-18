ProjetC - Projet Interface Graphique
18 Mai 2022

EXECUTION DES TESTS

	Test du projet :
	- Pour exécuter les tests du projet depuis le terminal, il suffit de taper la commande depuis le fichier du projet:\\

		cd cmake && cmake .. && make {test} && cd .. && ./cmake/{test}

	Avec {test} un des tests présents dans le projet (frame, button, helloworld, two048, puzzle).

	Tests personnels :
	- Pour executer un test que nous avons créé il faut taper la commande suivante depuis le fichier du projet :

		cd cmake && cmake .. && make nos_tests && cd .. && ./cmake/nos_tests test{k}

	avec k un entier compris entre 1 et 8 inclus.
	
	- Pour exécuter les tests que nous avons créé depuis Clion, il faut d'abord décommenter la ligne 437 et noter le numéro du test que l'on souhaite tester 


FERMETURE DES TESTS
	- Pour fermer les tests, il suffit d'appuyer sur echap sauf pour le test frame où les évènements ne sont pas pris en compte et ainsi il faut le kill (en utilisant xkill et en appuyant dessus par exemple).


TESTS 
- button.c : Crée un button au milieu de la fenêtre avec "Click !" écrit dessus en noir au milieu du button.
- frame.c : Crée une frame au milieu de la fenêtre.
- hello_world.c : Crée un toplevel avec un button "CLick !" que l'on peut resize et déplacer.
- puzzle.c : Créé un puzzle avec comme photo klimt que l'on peut reconstituer. 
- nos_tests.c
	1 : Crée une fenêtre en full écran.
	2 : Test des fonctions : ei_widgetclass_register, ei_geometrymanager_register, ei_app_free pour les listes chaînées des geometry managers et des widget class.
	3 : Test des fonctions : ei_app_root_widget, ei_frame_configure. Affichage du text et d'images dans des frames.
	4 : Crée 2 buttons, 1 avec du texte et 1 avec une image.
	5 : Vérifie que les relations de parentés sont bien effectués.
	6 : Crée un toplevel sans bord pour tester le resize et le déplacement.
	7 : Création d'un toplevel dans un toplevel.


AUTEURS :
	Projet réalisé par :
	- Jorge LURI VANO
	- Logan WILLEM 
	- Nils DEPUILLE


