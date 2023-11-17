#include "../headers/Conversion.h"

/* ---------- FONCTIONS FACULTATIVES ---------- */

void afficher_4x4(byte **s)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            printf("%02x ", s[i][j]);
        printf("\n");
    }
}

void afficher_column(byte *column)
{
    for (int i = 0; i < 4; i++)
        printf("%02x \n", column[i]);
}

/* -------------------------------------------- */

byte **fromHexToMatrice(char *hex)
{
    if (hex == NULL)
    {
        puts("ERROR : 'fromHexToMatrice' > le parametre 'hex' pointe sur NULL");
        exit(1);
    }
    if (strlen(hex) != 32)
    {
        puts("ERROR : 'fromHexToMatrice' > La taille de la cle doit etre de 16 octets, soit 32 symboles hexadecimaux");
        printf("Actuellement, '%s' fait %lu caractère\n", hex, strlen(hex));
        exit(1);
    }

    byte **s = calloc(4, sizeof(byte *));
    for (int i = 0; i < 4; i++)
        s[i] = calloc(4, sizeof(byte));

    int index = 0, i = 0, ligne = 0, colonne = 0;
    float tmp = 0.0;
    char tmp_hex[2];
    while (index < 32)
    {
        tmp_hex[0] = hex[index];
        tmp_hex[1] = hex[index + 1];
        tmp = ceilf((float)index / (float)2.0);
        colonne = (int)(tmp / 4);
        ligne = i % 4;
        s[ligne][colonne] = (byte)strtol(tmp_hex, NULL, 16);
        index += 2;
        i++;
    }

    return s;
}

char *fromMatriceToHex(byte **matrice)
{
    char *hex = calloc(33, sizeof(char));
    char tmp[3];
    int cpt = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            sprintf(tmp, "%02x", matrice[j][i]);
	    hex[cpt] = tmp[0];
            hex[cpt+1] = tmp[1];
	    cpt += 2;
        }
    }
    hex[32] = '\0';
    return hex;
}
