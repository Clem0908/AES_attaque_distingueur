#pragma once
#include "../headers/XOR_operations.h"
#include "../headers/Cadencement_cle.h"

/**
 * @brief Effectue l'opération shiftRows de l'AES sur la matrice en paramètre.
 *
 * @param matrice Matrice sur laquelle l'opération va être effectuée. Le résultat est stocké dans cette matrice
 */
void shiftRows(byte **matrice);

/**
 * @brief Effectue le mixColumns de l'AES sur la matrice en paramètre.
 *
 * @param matrice La matrice sur laquelle sera effectué le mixColumns, et dans laquelle le résultat sera directement stocké
 */
void mixColumns(byte **matrice);

/**
 * @brief Fonction principale du chiffrement AES.
 *
 * @param plaintext Le clair de 16 octets à chiffrer, il sera directement transformé en son chiffré correspondant
 * @param key La clé de 16 octets qui va servir au chiffrement
 */
void chiffrement(byte **plaintext, byte **key);

/**
 * @brief Fonction inverse du shitRows le l'AES.
 *
 * @param matrice Matrice sur laquelle l'opération va être effectuée. Le résultat est stocké dans cette matrice
 */
void shiftRowsReverse(byte **matrice);

/**
 * @brief Fonction inverse du mixColumns de l'AES.
 *
 * @param matrice La matrice sur laquelle sera effectué le mixColumns, et dans laquelle le résultat sera directement stocké
 */
void mixColumnsReverse(byte **matrice);

/**
 * @brief Fonction principale du déchiffrement AES.
 *
 * @param cipher Le chiffré de 16 octets à déchiffrer, il sera directement transformé en son clair correspondant
 * @param key La clé de 16 octets qui va servir au déchiffrement
 */
void dechiffrement(byte **cipher, byte **key);
