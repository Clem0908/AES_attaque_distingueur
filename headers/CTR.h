#pragma once

#include "../headers/Errors.h"
#include "../headers/FileOperations.h"
#include "../headers/AES.h"
#include <pthread.h>

/*
 * Structure IV
 */
typedef struct IVStruct
{
	byte **IV;
	byte **cpyIV;
} IVSTRUCT;

/*
 * Structure des octets à CTR et leur chiffré
 */
typedef struct CTRBytesStruct
{
	byte ***mult_m_blocks;
	byte ***res;
} CTRBYTESSTRUCT;

/*
 * Structure paramètres pour les threads
 */
typedef struct CTRComputeStruct
{
	int it;
	CTRBYTESSTRUCT *ctrbs;
	byte **key;
} CTRCOMPUTESTRUCT;

/**
 * @brief Demande à l'utilisateur de choisir un IV particulier ou non.
 *
 * @return void
 */
void askIV();

/**
 * @brief Alloue et initialise la structure de IV avec des valeurs aléatoires avec rand().
 *
 */
void initializeIV();

/**
 * @brief Alloue et initialise la structure de IV avec des valeurs choisies par l'utilisateur.
 *
 */
void chosenIV();

/**
 * @brief Libère la mémoire allouée pour IVSTRUCT.
 *
 */
void freeIV();

/**
 * @brief Complète les messages de moins de 32 caractères avec des 0.
 *
 * @return Message complété, multiples de 32 (2 dimensions, à libérer)
 */
char **complete_plaintext(const char *const plaintext);

/**
 * @brief Libère la mémoire des multiples clairs.
 *
 */
void free_mult_plaintext(char **mult_plaintext);

/**
 * @brief Alloue et init les multiples de blocs du message entré.
 *
 * @return Les multiples (byte *) des blocs du message (byte **) donc (byte ***)
 */
CTRBYTESSTRUCT *initialize_ctrbs(char **mult_plaintext);

/**
 * @brief Libère la mémoire allouée pour la structure contenant les blocs à CTRer.
 *
 */
void free_ctrbs(CTRBYTESSTRUCT *ctrbs);

byte **XORBlock(byte **block1, byte **block2);

void *CTRCompute(void *ctrcs);

/**
 * @brief Routine principale pour le CTR, appelle les threads.
 *
 * @return Chiffré en mode CTR
 */
char *CTREncryption(char *plaintext);
