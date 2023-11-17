#pragma once

#include "../headers/AES.h"
#include "../headers/FileOperations.h"

/**
 * @brief Représente un couple clair-chiffré.
 *
 * @param clair Matrice 4x4 du message clair
 * @param chiffre Matrice 4x4 du chiffré correspondant au clair.
 */
typedef struct clair_chiffre
{
    byte **clair;
    byte **chiffre;
} CLAIR_CHIFFRE;

/**
 * @brief Maillon d'une liste chainée contenant les valeurs potentielles d'UN octet de la sous-clé.
 *
 * @param octet Une valeur potentiel d'un octet de la sous-clé
 * @param next Pointeur vers le prochain maillon de la chaine
 */
typedef struct potential_key_byte
{
    byte octet;
    struct potential_key_byte *next;
} POTENTIAL_KEY_BYTE;

/**
 * @brief Créer une matrice 4x4 remplit de valeur zéro.
 *
 * @return Matruce 4x4 de zéro
 */
byte **creerMatriceZero();

/**
 * @brief Copie une matrice d'octet 4x4.
 *
 * @param originale La matrice à copier
 * @return Renvoie une copie de la amtrice en paramètre
 */
byte **copieMatrice(byte **originale);

/**
 * @brief Test l'égalité entre deux matrices.
 *
 * @param matrice1 L'une des matrices comparées
 * @param matrice2 L'autre matrice comparée
 * @return vrai si elles sont égales, faux sinon
 */
BOOLEAN areMatricesEqual(byte **matrice1, byte **matrice2);

CLAIR_CHIFFRE *creerCouplesClaireChiffre_recursion();

/**
 * @brief Créer NB_CLAIRCHIFFRE couples clair-chiffré, dont seul le premier octet varie. Les autres sont à 0.
 *
 * @return Liste des couples clair-chiffré
 */
CLAIR_CHIFFRE **creerCouplesClaireChiffre_intersection();

/**
 * @brief Retrouve la sous-clé octet par octet, selon l'attaque par saturation.
 *
 * @param etats Liste des couples clair-chiffré
 * @return Liste d'octet potentiels
 */
POTENTIAL_KEY_BYTE **keyPotentialBytes(const CLAIR_CHIFFRE *const etats);

/**
 * @brief Trouve la véritable sous-clé à partir des octets potentiels, en se servant de la récursivité.
 *
 * @param etats Liste des couples clair-chiffré
 * @param potentialKey Ensemble des octets potentiels de la sous-clé
 * @param sub_key Matrice 4x4 représentant la dernière sous-clé. Ce paramètre est directement modifié par la fonction
 * @param indice La position de l'octet à tester dans la matrice
 */
void findSubKey_recursion(CLAIR_CHIFFRE *etats, POTENTIAL_KEY_BYTE **potentialKey, byte **sub_key, const int indice);

/**
 * @brief Trouve la véritable sous-clé à partir des octets potentiels, en faisant l'intersection des listes d'octets potentiels.
 *
 * @param potentialKey_0 Première ensemble des octets potentiels de la sous-clé
 * @param potentialKey_1 Deuxième ensemble des octets potentiels de la sous-clé
 * @param potentialKey_2 Troisième ensemble des octets potentiels de la sous-clé
 * @return Renvoie le résultat des intersections
 */
POTENTIAL_KEY_BYTE **findSubKey_intersetion(POTENTIAL_KEY_BYTE **potentialKey_0, POTENTIAL_KEY_BYTE **potentialKey_1, POTENTIAL_KEY_BYTE **potentialKey_2);

/**
 * @brief Test la clé en paramètre pour savoir si elle a potentiellement été utilisée lors du chiffrement des 256 états.
 *
 * @param etats Ensemble des couples clair-chiffré utilisé pour l'attaque
 * @param master_key La clé qui doit être testée
 * @return Vrai si la clé a potentiellement été utilisée, faux sinon
 */
BOOLEAN testMasterKey(CLAIR_CHIFFRE *etats, byte **master_key);
