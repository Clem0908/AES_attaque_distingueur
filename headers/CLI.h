#pragma once

#include "../headers/Attaque.h"
#include "../headers/CTR.h"

/**
 * @brief Traitement du paramètre entré par l'utilisateur.
 *
 * @param argv L'ensemble des paramètres entrés par l'utilisateur
 */
void cmd(const int argc, char **const argv);

/**
 * @brief Execution du chiffrement AES.
 *
 */
void executeChiffrement();

/**
 * @brief Execution du déchiffrement AES.
 *
 */
void executeDechiffrement();

/**
 * @brief Cette attaque nécessite 3 fichiers contenant chacun 256 couples clair-chiffré.
 *
 */
void attaque_intersection();

/**
 * @brief Attaque utilisant la récursivité pour tester toutes les combinaisons possibles des octets candidats.
 *
 */
void attaque_recursion();

/** Fonction de lecture de fichier,
 *  renvoie la clé sous forme de matrice 4x4.
 *
 *  @param char *file : nom de fichier à ouvrir
 *  @return byte **key4x4 : matrice de la clé
 */
byte **getKeyFromFile(char *file);
