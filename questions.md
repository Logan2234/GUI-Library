# Questions à demander aux profs

- Si on alloue ``sizeof(ei_widget_t)`` dans ``allocfunc`` pour créer un nouveau widget, comment peut-on être sûr que l'on peut se servir de ce widget comme étant un frame widget par exemple (polymorphisme).

Réponse:

<br>

- Comment dessiner le rectangle bleu, il faut une liste chaînée de points. Faut-il la construire dans la fonction fill ?

Réponse:

<br>

- Dans ``ei_application.c``, pour ``ei_app_root_get`` par exemple, on a besoin du widget racine (qui est une frame). Pour cela il faut la créer dans ``ei_app_create``, sauf qu'en faisant comme ça, la fonction étant de type void, comment retourner ce widget racine pour qu'on puisse la récupérer à l'aide de ``ei_app_root_get`` ?

Réponse:

<br>

- A quoi sert le parametre ``surface`` de ``ei_map_rgba`` ?

Réponse:

<br>

- Différence entre ``widget->destructor`` et  ``releasefunc`` ?

Réponse:

<br>
