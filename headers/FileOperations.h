#pragma once

#include <fcntl.h>
#include <unistd.h>

#include "../headers/Conversion.h"

/**
 * @brief Fonction d'ouverture de fichier
 *
 * @param file Nom de fichier à ouvrir
 * @return fd Descripteur du fichier ouvert
 */
int openFile(const char *const file);

/**
 * @brief Fonction de lecture d'un fichier
 *
 * @param fd Descripteur du fichier ouvert
 * @return buffer Tampon de lecture
 */
char *readFile(const int fd);

void readFileCTR(const int fd, char *buffer);

char *readChosenFile(const int fd);

/**
 * @brief Fonction de lecture de fichier,
 * renvoie la clé sous forme de matrice 4x4.
 *
 * @param file Nom de fichier à ouvrir
 * @return Matrice de 4x4 octets
 */
byte **getHexFromFile(char *file);

void creerFichierCouplesClairChiffre();

char **arrangeBufferChosen(char *buffer);

void removeNewLinesAndAddCipher(char **fill_state, char *buffer, int fd, byte **key);
