#pragma once

#include "Includes.h"
#include <string.h>
#include <math.h>

void afficher_4x4(byte **s);

void afficher_column(byte *column);

byte **fromHexToMatrice(char *hex);

char *fromMatriceToHex(byte **matrice);
