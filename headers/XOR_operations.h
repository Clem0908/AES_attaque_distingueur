#pragma once

#include "Includes.h"

/**
 * @brief Calcule le XOR case par case des deux paramètres.
 *
 * @param s La matrice intermédiaire au chiffrement du clair, dans laquelle sera stocké le résultat
 * @param subKey la sous-clé avec laquelle effectuer le XOR
 */
void addRoundKey(byte **s, byte **subKey);

/**
 * @brief Calcule le XOR case par case de deux colonnes (tableau de 4 cases).
 *
 * @param column_1 Tableau dans lequel sera stocké le résultat
 * @param column_2 Tableau avec lequel sera effectué avec le XOR
 */
void XOR_columns(byte *column_1, byte *column_2);

/**
 * @brief Fonction calculant le modulo d'un entier, en évitant le résultat '-1'
 *
 * @param nb L'entier dont on veut le résultat modulo 'mod'
 * @param mod Entier qui sert de modulo
 * @return Renvoie un entier compris entre 0 et mod-1 (Les deux bornes incluses)
 */
int modulo(const int nb, const int mod);
