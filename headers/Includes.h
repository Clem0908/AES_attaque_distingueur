#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef uint8_t byte;

#define NB_TOUR 4           // Nombre de tours que l'on veut effectuer pour l'AES
#define MESSAGE_LENGTH 32   // Taille des données à récupérer dans les fichiers (clé et message clair)
#define NB_CLAIRCHIFFRE 256 // Nombre de couple (clair, chiffré) utilisé pour l'attaque par saturation de l'AES
#define MAX_BUFFER_LENGTH 8449 // Taille max du buffer de lecture pour les clairs choisis ((32*2+1+1)*256) 

typedef enum boolean
{
    False = 0,
    True = 1
} BOOLEAN;
