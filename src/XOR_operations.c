#include "../headers/XOR_operations.h"

void XOR_columns(byte *column_1, byte *column_2)
{
    if (column_1 == NULL || column_2 == NULL)
    {
        puts("ERREUR : 'XOR_columns' > l'un des paramètres pointe sur NULL");
        exit(1);
    }

    for (int i = 0; i < 4; i++)
        column_1[i] = column_1[i] ^ column_2[i];
}

void addRoundKey(byte **s, byte **subKey)
{
    for (int i = 0; i < 4; i++)
        XOR_columns(s[i], subKey[i]);
}

int modulo(const int nb, const int mod)
{
    const int tmp = nb % mod;
    return tmp < 0 ? tmp + mod : tmp;
}