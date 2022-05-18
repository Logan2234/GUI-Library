# Projet C

Pour exécuter les tests du projet depuis le terminal, il suffit de taper la commande depuis le fichier du projet:
cd cmake && cmake .. && make {test} && cd .. && ./cmake/{test} 
Avec {test} un des tests présent dans le projet (frame, button, hello_world, two048, puzzle)
Pour executer un test que nous avons créé il faut taper la commande suivante depuis le fichier du projet : 
cd cmake && cmake .. && make nos_tests && cd .. && ./cmake/nos_tests test{k}
avec k un entier compris entre 1 et 8 inclus.
Pour exécuter les tests que nous avons créé depuis Clion, il faut d'abord décommenter la ligne 437 et noter le numéro du test que l'on souhaite tester 

Pour fermer les tests, il suffit d'appuyer sur echap sauf pour le test frame où il faut le xkill.

Projet réalisé par :
Jorge LURI VANO
Logan WILLEM
Nils DEPUILLE