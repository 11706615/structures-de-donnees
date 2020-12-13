# [Rapport de projet](https://github.com/11706615/structures-de-donnees/blob/master/R%C3%A9seau%20d%E2%80%99Aqueducs/Rapport/rapport.md)
La France a décidé de repenser son réseau d’aqueducs pour rendre l’approvisionnement des communes en eau plus efficace. Le chef du projet vous charge de calculer le réseau le plus court reliant toutes les villes, ainsi que sa taille. Pour ce faire, vous calculerez l’arbre couvrant de poids minimal sur un graphe dont les nœuds sont les villes ; les arêtes sont les connexions entre les villes, et le poids associé à chaque arête est la distance entre les villes en kilomètre.
- Adrien Simonnet
- Pierre Wu
## Structures de données
- `Vecteur` : Tableau faisant office d'un ensemble indicé fini d'objets quelconques. L'accès à un élément est constant.
- `Arrête` : Agrégat contenant le sommet d'origine, de destination et le coût d'une arrête. Le type des sommets est générique ce qui permet de supporter les versions 64 bits mais il doit être numérique et non signé.
- `Tas` : Ensemble générique d'éléments permettant l'insertion d'un élément et l'extraction du plus petit. Le type des éléments n'a pas d'importance mais il doit nécessairement fournir un opérateur de comparaison entre deux éléments. Par ailleurs la destruction, la construction par déplacement et l'assignation par déplacement ne doivent pas générer d'exceptions pour garantir l'intégrité de la structure.
## Algorithmes
Deux algorithmes ont été testés pour résoudre ce problème.
### Prim-Dijsktra
Le principe de cet algorithme est de parcourir les sommets. En partant d'un sommet quelconque, marquer ce sommet comme visité et ajouter les arrêtes partant de celui-ci dans un tas. Ensuite, il faut en extraire l'arrête de plus petit coût menant vers un sommet non visité, l'ajouter au résultat final et réitérr l'opération en partant du sommet vers lequel mène cette arrête.
#### Pré-conditions
- `Sommets` : Vecteurs de sommets quelconques.
- `Coût` : Fonction, lambda ou fonctor prenant en premier argument une référence vers le sommet d'origine et en deuxième argument une référence vers le sommet de destination et retournant le **coût** d'une arrête menant du premier sommet au deuxième. La nature des coûts n'a aucune importance mais ils doivent nécessairement être **comparables** entre eux pour que l'algorithme fonctionne.
#### Post-conditions
- Retourne un ensemble de n-1 arrêtes (avec n le nombre de sommets passés en argument) qui composent un arbre couvrant minimal entre ces sommets.
#### Compléxité
O(n * log(m)) avec n le nombre de sommets et m = (n * (n - 1)) / 2 le nombre d'arrêtes.
### Glouton
Le principe de cet algorithme est de parcourir les arrêtes. En partant de l'arrête la plus petite, si le sommet de départ et d'arrivée ne sont pas connexes (ils n'appartiennent pas à la même composante), ajouter cette arrête au résultat et mettre à jour la composante du sommet d'arrivée pour la faire correspondre à la composante du sommet d'origine.
#### Pré-conditions
- `Arrêtes` : Vecteur **trié par ordre croissant** d'arrêtes.
- `Taille` : Nombre de sommets du graphe. Ce nombre doit être supérieur à tout sommet (origine ou destination) stocké dans les arrêtes.
#### Post-conditions
- Retourne un ensemble de n-1 arrêtes (avec n le nombre de sommets passés en argument) qui composent un arbre couvrant minimal entre ces sommets.
#### Compléxité
O(m * log(m)) avec n le nombre de sommets et m = (n * (n - 1)) / 2 le nombre d'arrêtes.
## Meilleur algorithme
Pour le problème posé, il ne fait aucun doute que le premier algorithme doit être privilégié car cela revient à déterminer le plus court chemin d'un graphe complet. En effet il existe potentiellement un chemin entre toutes les villes.
## Tests effectués
Population minimale  | Villes | Temps de calcul (sec) | Longueur du réseau (km) | Graphe
:------------------: | :----: | :-------------------: | :---------------------: | :----:
0                    | 36201  | 295,5720658           | 90002                   | ![0 habitant et plus](./0.png "0 habitant et plus")
1000                 | 9490   | 11,4878384            | 41104                   | ![1 000 habitants et plus](./1000.png "1 000 habitants et plus")
10 000               | 889    | 0,10865               | 10671                   | ![10 000 habitants et plus](./10000.png "10 000 habitants et plus")
50 000               | 113    | 0,001961              | 4554                    | ![50 000 habitants et plus](./50000.png "50 000 habitants et plus")
100 000              | 39     | 0,000249              | 3497                    | ![100 000 habitants et plus](./100000.png "100 000 habitants et plus")
250 000              | 8      | 0,000035              | 1850                    | ![250 000 habitants et plus](./250000.png "250 000 habitants et plus")
