# Projet Attaque intégrale contre une version réduite de l’AES :
Le chiffrement par blocs AES est depuis 2001 le standard de chiffrement en
cryptographie symétrique. Le but de ce projet est d’étudier une technique de cryptanalyse
classique, connue sous le nom d’attaque intégrale ou attaque par saturation et d’implémenter
cette attaque contre une version réduite de l’AES.

#### Étudiants :
- Clément LYONNET
- Éloïse MERCADO GARCIA
- Vincent XAVIER

M1 INFORMATIQUE (2022) UVSQ

## Comment faire compiler et executer notre projet : 
### Chiffrement et Déchiffrement :
- Il faut tout d'abord avoir la clé et le message à chiffrer ou déchiffrer.
- Mettre la clé dans le ficher texte `key.txt` et le message dans `plaintext.txt` (en hexadécimal) qui se trouvent dans le dossier `in`.
- Taper en ligne de commande sur un terminal : **make chiffrer** (pour chiffrer) ou **make dechiffrer** (pour déchiffrer)
- Le résultat s'affichera sur le terminal.
- Le message et la clé doivent être d'une taille de 128 bits (16 octets).

### Attaque
- L'attaque se déroule en 3 étapes :
    - Génération d'une clé aléatoire.
    - Génération des 256 couples clairs-chiffrés (pour la récursion) ou de 256 x 3 couples clairs-chiffrés (pour l'intersection)
    - Exécution de l'attaque en elle-même (par Récursion ou par Intersection)
##### Récursion
- Taper en ligne de commande sur un terminal : **make attaquer_recursion**
- La clé retrouvée sera affichée sur le terminal.
##### Intersection
- Taper en ligne de commande sur un terminal : **make attaquer_intersection**
- La clé retrouvée sera affichée sur le terminal.

### CTR : (chiffrement)
Nous avons mis en place un CTR (CounTeR) pour les messages plus longs que 128 bits (32 symboles hexadécimaux). Il comporte un IV (Initialization Vector) qui est généré aléatoirement.
- Il faut mettre la clé dans `key.txt` et le message dans `plaintext.txt` qui se trouvent dans le dossier `in`.
- Taper en ligne de commande sur un terminal : **make chiffrer_CTR**.
- Le message peut-être de n'importe quelle taille mais la clé doit être de 128 bits.
- Le chiffré est affiché sur le terminal ainsi que le IV (pour permettre le déchiffrement).

## Executer le programme une fois compilé
- **./bin/AES chiffrer** : effectue le chiffrement.
- **./bin/AES dechiffrer** : effectue le déchiffrement.
- **./bin/AES chiffrer_CTR** : effectue le chiffrement en utilisant le mode d'opération CTR.
- **./bin/AES attaquer** : effectue l'attaque en se servant de l'intersection.
- **./bin/AES attaquer recursion** : effectue l'attaque en se servant de la récursion.

## Précisions sur le code :
Nous avons renommé le type `uint8_t` en `byte` et créé un type `boolean` pour plus de lisibilité dans le code.

## Représentation de l'AES :
Nous avons représenté un état de l'AES comme une matrice de 4x4 `byte` (qui se traduit par le type `byte **`).

### Choix de l'implémentation :
Nous avons décidé d'utiliser le type `uint8_t` (byte) car il nous permet de plus facilement représenter l’octet. Les opérations comme le XOR ou le parcours de tableau (S-box) est plus facile de mettre en place avec `byte`. Nous avons également décidé de représenter un état sous la forme d'une matrice 4x4 `byte **` car les opérations sur l'AES se font principalement sur les octets. Il suffit donc de faire les opérations sur des cases de notre matrice. Nous avons décidé de représenter en tableau de pointeur car un état de l'AES est représenté comme un tableau de 4 par 4. Il nous ait donc naturellement venu de le représenter tel quel (`byte **s`).

### Explication du MixColumns :
Nous avons trouvé sur un site (https://www.davidwong.fr/blockbreakers/aes.html) et sur Wikipédia (https://en.wikipedia.org/wiki/Rijndael_MixColumns) des tables qui facilitent l'opération MixColumns. Nous utilisons des tables précalculées pour faire la multiplication par 2 et 3 pour le chiffrement et 9, 11, 13, et 14 pour le déchiffrement. Nous avons donc 6 tables de tailles 256. 
Il suffit de chercher à l'indice de la valeur de l'octet dans le tableau correspondant pour obtenir la multiplication correspondante. 

## Attaque
### Structures 
- Nous avons un struct `clair_chiffre` qui représente un message de 128 bits (16 octets) qui est notre matrice 4x4 associée avec son chiffré (par l'algorithme de l'AES), une matrice 4x4 également. Elle a donc comme attribut 2 `byte **` pour le message et son chiffré associé.

- Nous avons un struct `potential_key_byte` qui est une liste chainée d'octet. Elle contient un attribut `byte` représentant l'octet potentiel et d'un pointeur qui pointe sur un autre `potential_key_byte`. Cette structure est utile lorsqu'on ne sait pas de quelle taille sera cette liste lors de l'execution du programme. De plus, cela permet d'utiliser juste ce qu'il faut en terme de mémoire.

### Déroulement
Pour l'attaque, nous générons aléatoirement une clé de 128 bits. Nous l'utilisons ensuite pour avoir nos 256 couples clairs chiffrés avec la constante (000---00) que nous stockons sous forme de tableau `CLAIR_CHIFFRE **`.
Nous obtenons une liste d'octets qui sont les octets potentiels de la sous-clé que nous stockons également dans un tableau de liste d'octets de taille 16 `POTENTIAL_KEY_BYTE **`. Il y a 2 façons pour obtenir la clé, soit par récursion, soit par intersection.

#### Récursion
Nous utilisons toutes les combinaisons possibles à partir des octets potentiels trouvés pour obtenir une sous-clé potentielle. Nous remontons pour trouver la clé maître avec le cadencement de clé inverse et nous chiffrons les messages clairs initiaux (il y en a 256). Nous comparons ensuite les chiffrés obtenus avec les chiffrés initiaux. S'ils sont tous égaux, nous avons trouvé la clé. Nous continuons à tester les autres clés par récursion jusqu'à trouver la vraie clé maître.

#### Intersection
Pour obtenir la clé, nous faisons la même opération 2 fois mais avec une constante différente (111---11 et 222---22). Nous obtenons donc 256 x 3 couples clairs-chiffrés et 3 ensembles d'octets potentiels pour la sous-clé. Nous procédons ensuite à l'intersection de ces 3 ensembles et nous trouvons ainsi la sous-clé. Nous remontons avec le cadencement de clé inverse pour obtenir la clé maître.




