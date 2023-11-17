#pragma once

#include "Includes.h"

extern const byte sbox_en[256];
extern const byte sbox_inverse[256];

/**
 * @brief Substitue les octets selon l'algorithme AES.
 *
 * @param s Matrice de taille 4*4 contenant les octets à substitués. Le paramètre est directement modifié
 */
void subBytes(byte **s);

void subBytesReverse(byte **s);