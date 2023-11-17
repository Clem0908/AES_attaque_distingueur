#pragma once

#include "../headers/Substitution.h"

/**
 * @brief Cadencement de clé.
 * Pour les 4 tours de l'AES, il faut 5 clés. (4*5)
 *
 * @param key la clé principale.
 * @return Renvoi de la matrice clé de dimension [4][20].
 **/
byte **key_schedule(byte **key);

/**
 * @brief Effectue la rotation d'octet sur une colonne de la clé.
 *
 * @param key_column La colonne sur laquelle la rotation doit d'effectuer.
 * @return Ne retourne rien : la colonne est directement modifiée via le parametre 'key_column'.
 */
void rotWord(byte *key_column);

/**
 * @brief Effectue la sbox octet par octet sur une colonne de la clé.
 * Utilisation de sbox_en
 *
 * @param key_column La colonne sur laquelle la sbox va être effectué. Le paramètre est directement modifié.
 */
void subWord(byte *key_column);

/**
 * @brief Cadencement de clé par tour.
 * La fonction key_schedule appelle cette fonction.
 *
 *
 * @param precedent_key la sous-clé à partir duquel on va faire la nouvelle sous-clé
 * @param following_key la nouvelle sous-clé. Le paramètre est directement modifié.
 * @param round on a besoin du tour pour un XOR
 * @return Ne retourne rien : la nouvelle colonne est dans following_key
 **/
void key_schedule_round(byte **precedent_key, byte **following_key, int round);

/**
 * @brief Cadencement de clé, inverse
 *
 * @param key key La clé du tour 4. Le paramètre est directement modifié.
 */
void key_schedule_inverse(byte **key);