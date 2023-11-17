#include "../headers/Cadencement_cle.h"
#include "../headers/Substitution.h"
#include "../headers/XOR_operations.h"

// Tableau utilisé pour les rounds, c'est avec ça qu'on va faire un XOR
const byte rcon[11] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

void rotWord(byte *key_column)
{
    if (key_column == NULL)
    {
        puts("ERREUR : 'rotWord' > le parametre pointe sur NULL");
        exit(1);
    }

    byte tmp = key_column[0];
    key_column[0] = key_column[1];
    key_column[1] = key_column[2];
    key_column[2] = key_column[3];
    key_column[3] = tmp;
}

void subWord(byte *key_column)
{
    if (key_column == NULL)
    {
        puts("ERREUR : 'rotWord' > le parametre pointe sur NULL");
        exit(1);
    }

    for (int i = 0; i < 4; i++)
        key_column[i] = sbox_en[key_column[i]];
}

void key_schedule_round(byte **precedent_key, byte **following_key, int round)
{
    // CALCUL DE LA PREMIÈRE COLONNE DE LA NOUVELLE CLÉ (Clé suivante)
    // Utilisation de rotWord() et subWord()
    // -> Obligé de faire une nouvelle colonne (allocation)
    byte *new_column = calloc(4, sizeof(byte));
    for (int i = 0; i < 4; i++)
        new_column[i] = precedent_key[i][3];

    rotWord(new_column);
    subWord(new_column);

    // Recupération de la 1ère colonne de la précedente clé
    // Pour pouvoir XOR avec la nouvelle colonne
    byte *column_key = calloc(4, sizeof(byte));
    for (int i = 0; i < 4; i++)
        column_key[i] = precedent_key[i][0];

    XOR_columns(new_column, column_key);

    // Création de la colonne round et XOR avec la nouvelle clé
    byte *round_column = calloc(4, sizeof(byte));
    round_column[0] = rcon[round];
    round_column[1] = 0;
    round_column[2] = 0;
    round_column[3] = 0;

    XOR_columns(new_column, round_column);

    // Début du remplissage de la nouvelle clé
    for (int i = 0; i < 4; i++)
        following_key[i][0] = new_column[i];

    // XOR avec la colonne de l'ancienne clé avec la nouvelle colonne
    // Suite de remplissage de la nouvelle clé
    for (int i = 1; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            column_key[j] = precedent_key[j][i];

        XOR_columns(new_column, column_key);

        for (int j = 0; j < 4; j++)
            following_key[j][i] = new_column[j];
    }

    free(new_column);
    free(column_key);
    free(round_column);
}

byte **key_schedule(byte **key)
{

    // ALLOCATION MEM DE MATRICE [4][20]
    byte **all_key = calloc(4, sizeof(byte *));
    for (int i = 0; i < 4; i++)
        all_key[i] = calloc(4 * (NB_TOUR + 1), sizeof(byte));

    // mettre key dans all_key
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            all_key[i][j] = key[i][j];
    }

    // ALLOCATION POUR LA PROCHAINE CLÉ (celle du tour suivant)
    byte **following_key = calloc(4, sizeof(byte *));
    for (int i = 0; i < 4; i++)
        following_key[i] = calloc(4, sizeof(byte));

    // Calcul des clés suivantes sur 4 tour ici
    for (int round = 1; round <= NB_TOUR; round++)
    {
        key_schedule_round(key, following_key, round);

        // mettre following_key dans all_key
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
                all_key[i][round * 4 + j] = following_key[i][j];
        }
        key = following_key;
    }

    for (int i = 0; i < 4; ++i)
        free(following_key[i]);

    free(following_key);
    return all_key;
}

void key_schedule_inverse(byte **key)
{
    // allocation pour les colonnes
    byte *new_column = calloc(4, sizeof(byte));
    byte *column_tmp = calloc(4, sizeof(byte));

    for (int round = 4; round > 0; round--)
    {
        for (int j = 3; j > 0; j--)
        {
            for (int i = 0; i < 4; i++)
                new_column[i] = key[i][j];

            for (int i = 0; i < 4; i++)
                column_tmp[i] = key[i][j - 1];

            XOR_columns(new_column, column_tmp);

            for (int i = 0; i < 4; i++)
                key[i][j] = new_column[i];
        }

        for (int i = 0; i < 4; i++)
            new_column[i] = key[i][0];

        column_tmp[0] = rcon[round];
        column_tmp[1] = 0;
        column_tmp[2] = 0;
        column_tmp[3] = 0;

        XOR_columns(new_column, column_tmp);

        for (int i = 0; i < 4; i++)
            column_tmp[i] = key[i][3];

        rotWord(column_tmp);
        subWord(column_tmp);

        XOR_columns(new_column, column_tmp);

        for (int i = 0; i < 4; i++)
            key[i][0] = new_column[i];
    }

    free(new_column);
    free(column_tmp);
}