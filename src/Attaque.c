#include "../headers/Attaque.h"
#include "../headers/Errors.h"

byte **creerMatriceZero()
{
    byte **matrice = (byte **)calloc(4, sizeof(byte *));
    for (int i = 0; i < 4; i++)
        matrice[i] = (byte *)calloc(4, sizeof(byte));

    return matrice;
}

byte **copieMatrice(byte **originale)
{
    byte **matrice = (byte **)calloc(4, sizeof(byte *));
    for (int i = 0; i < 4; i++)
        matrice[i] = (byte *)calloc(4, sizeof(byte));

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            matrice[i][j] = originale[i][j];
    }

    return matrice;
}

BOOLEAN areMatricesEqual(byte **matrice1, byte **matrice2)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            if (matrice1[i][j] != matrice2[i][j])
                return False;
    }
    return True;
}

CLAIR_CHIFFRE *creerCouplesClaireChiffre_recursion()
{
    CLAIR_CHIFFRE *etats = (CLAIR_CHIFFRE *)calloc(NB_CLAIRCHIFFRE, sizeof(CLAIR_CHIFFRE));
    if (!etats)
        memoryError(__func__);

    byte **key = getHexFromFile("./in/key.txt");

    // Ouverture du fichier
    const int fd = openFile("./in/clear_cipher_couples_0.txt");
    char *buffer = readChosenFile(fd);
    if (!buffer)
        readError(__func__);

    char **fill_state = arrangeBufferChosen(buffer);

    removeNewLinesAndAddCipher(fill_state, buffer, fd, key);

    for (int i = 0; i < NB_CLAIRCHIFFRE; i++)
    {
        etats[i].clair = fromHexToMatrice(fill_state[i]);
        etats[i].chiffre = fromHexToMatrice(fill_state[i]);
        chiffrement(etats[i].chiffre, key);
    }

	close(fd);

    return etats;
}

CLAIR_CHIFFRE **creerCouplesClaireChiffre_intersection()
{
    CLAIR_CHIFFRE **etats = (CLAIR_CHIFFRE **)calloc(NB_CLAIRCHIFFRE * 3, sizeof(CLAIR_CHIFFRE *));
    if (!etats)
        memoryError(__func__);
    for (int i = 0; i < 3; i++)
    {
        etats[i] = (CLAIR_CHIFFRE *)calloc(NB_CLAIRCHIFFRE, sizeof(CLAIR_CHIFFRE));
        if (!etats[i])
            memoryError(__func__);
    }

    byte **key = getHexFromFile("./in/key.txt");

    // Ouverture des fichiers
    const int fd0 = openFile("./in/clear_cipher_couples_0.txt");
    char *buffer0 = readChosenFile(fd0);
    if (!buffer0)
        readError(__func__);
    const int fd1 = openFile("./in/clear_cipher_couples_1.txt");
    char *buffer1 = readChosenFile(fd1);
    if (!buffer1)
        readError(__func__);
    const int fd2 = openFile("./in/clear_cipher_couples_2.txt");
    char *buffer2 = readChosenFile(fd2);
    if (!buffer2)
        readError(__func__);

    char **fill_state_0 = arrangeBufferChosen(buffer0);
    char **fill_state_1 = arrangeBufferChosen(buffer1);
    char **fill_state_2 = arrangeBufferChosen(buffer2);

    removeNewLinesAndAddCipher(fill_state_0, buffer0, fd0, key);
    removeNewLinesAndAddCipher(fill_state_1, buffer1, fd1, key);
    removeNewLinesAndAddCipher(fill_state_2, buffer2, fd2, key);

    for (int i = 0; i < NB_CLAIRCHIFFRE; i++)
    {
        etats[0][i].clair = fromHexToMatrice(fill_state_0[i]);
        etats[0][i].chiffre = fromHexToMatrice(fill_state_0[i]);
        chiffrement(etats[0][i].chiffre, key);
        etats[1][i].clair = fromHexToMatrice(fill_state_1[i]);
        etats[1][i].chiffre = fromHexToMatrice(fill_state_1[i]);
        chiffrement(etats[1][i].chiffre, key);
        etats[2][i].clair = fromHexToMatrice(fill_state_2[i]);
        etats[2][i].chiffre = fromHexToMatrice(fill_state_2[i]);
        chiffrement(etats[2][i].chiffre, key);
    }

    close(fd0);
    close(fd1);
    close(fd2);

    return etats;
}

void partialReverseAES(byte *const octet_etats, const byte key, const CLAIR_CHIFFRE *const etats, const int j, const int k)
{
    for (int i = 0; i < NB_CLAIRCHIFFRE; i++) // Parcours tous les chiffrés
    {
        octet_etats[i] = etats[i].chiffre[j][k] ^ key;
        octet_etats[i] = sbox_inverse[octet_etats[i]];
    }
}

BOOLEAN checkIntegral(const byte *const octet_etats)
{
    byte tmp = 0;
    for (int i = 0; i < NB_CLAIRCHIFFRE; i++) // Vérifie si on retrouve la propriété de l'integral
        tmp ^= octet_etats[i];
    if (tmp == 0)
        return True;
    else
        return False;
}

POTENTIAL_KEY_BYTE *insertByte(POTENTIAL_KEY_BYTE *liste, const byte octet)
{
    POTENTIAL_KEY_BYTE *maillon = (POTENTIAL_KEY_BYTE *)malloc(sizeof(POTENTIAL_KEY_BYTE));
    maillon->octet = octet;
    maillon->next = NULL;

    if (liste == NULL) // Dans le cas où la liste est vide
    {
        liste = maillon;
        return liste;
    }

    POTENTIAL_KEY_BYTE *tmp = liste;
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = maillon;
    return liste;
}

void afficherOctetPotentiel(POTENTIAL_KEY_BYTE **key_bytes)
{
    POTENTIAL_KEY_BYTE *tmp;
    for (int i = 0; i < 16; i++)
    {
        printf("Octet numéro %d : ", i);
        tmp = key_bytes[i];
        while (tmp != NULL)
        {
            printf("%02x ", tmp->octet);
            tmp = tmp->next;
        }
        puts("");
    }
}

POTENTIAL_KEY_BYTE **keyPotentialBytes(const CLAIR_CHIFFRE *const etats)
{
    POTENTIAL_KEY_BYTE **key_bytes = (POTENTIAL_KEY_BYTE **)calloc(16, sizeof(POTENTIAL_KEY_BYTE *));
    for (int i = 0; i < 16; i++)
        key_bytes[i] = NULL;

    byte *octet_etats = (byte *)calloc(NB_CLAIRCHIFFRE, sizeof(byte));

    int indice;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int key = 0; key < NB_CLAIRCHIFFRE; key++) // Test les 256 valeurs possible d'un ocet de la clé
            {
                partialReverseAES(octet_etats, key, etats, i, j); // 'octet_etats' est directement modifié
                if (checkIntegral(octet_etats) == True)
                {
                    indice = (i * 4) + j; // Fait correspondre la case de la matrice avec la case du tableau
                    key_bytes[indice] = insertByte(key_bytes[indice], key);
                }
            }
        }
    }

    free(octet_etats);

    return key_bytes;
}

void findSubKey_recursion(CLAIR_CHIFFRE *etats, POTENTIAL_KEY_BYTE **potentialKey, byte **sub_key, const int indice)
{
    POTENTIAL_KEY_BYTE *tmp = potentialKey[indice];
    int i, j;
    char *hexKey;
    while (tmp != NULL)
    {
        i = indice / 4;
        j = indice % 4;
        sub_key[i][j] = tmp->octet;
        if (indice == 15) // Lorsque la sous-clé a été construite en entière, nous la testons
        {
            byte **key_test = copieMatrice(sub_key);
            key_schedule_inverse(key_test); // 'key_test' contient maintenant une master key à tester
            if (testMasterKey(etats, key_test) == True)
            {
                hexKey = fromMatriceToHex(key_test);
                printf("Voici la clé qui a été trouvée : %s\n", hexKey);
                free(hexKey);
                return;
            }
        }
        else
            findSubKey_recursion(etats, potentialKey, sub_key, indice + 1);

        tmp = tmp->next;
    }
}

BOOLEAN testMasterKey(CLAIR_CHIFFRE *etats, byte **master_key)
{
    byte **plaintext;
    for (int i = 0; i < NB_CLAIRCHIFFRE; i++)
    {
        plaintext = copieMatrice(etats[i].clair);
        chiffrement(plaintext, master_key);
        if (areMatricesEqual(plaintext, etats[i].chiffre) == False)
            return False;
    }
    return True;
}

POTENTIAL_KEY_BYTE *intersection_2_liste(POTENTIAL_KEY_BYTE *l1, POTENTIAL_KEY_BYTE *l2)
{
    POTENTIAL_KEY_BYTE *inter = NULL; // Pointeur sur une liste chainée, contenant les octets en commun de l1 et l2
    POTENTIAL_KEY_BYTE *l1_tmp = l1;
    POTENTIAL_KEY_BYTE *l2_tmp = l2;
    while (l1_tmp != NULL && l2_tmp != NULL)
    {
        if (l1_tmp->octet < l2_tmp->octet)
            l1_tmp = l1_tmp->next;
        else if (l1_tmp->octet > l2_tmp->octet)
            l2_tmp = l2_tmp->next;
        else
        {
            inter = insertByte(inter, l1_tmp->octet);
            l1_tmp = l1_tmp->next;
            l2_tmp = l2_tmp->next;
        }
    }

    return inter;
}

POTENTIAL_KEY_BYTE **findSubKey_intersetion(POTENTIAL_KEY_BYTE **potentialKey_0, POTENTIAL_KEY_BYTE **potentialKey_1, POTENTIAL_KEY_BYTE **potentialKey_2)
{
    POTENTIAL_KEY_BYTE **intersect = (POTENTIAL_KEY_BYTE **)calloc(16, sizeof(POTENTIAL_KEY_BYTE *));
    for (int i = 0; i < 16; i++)
    {
        intersect[i] = intersection_2_liste(potentialKey_0[i], potentialKey_1[i]);
        intersect[i] = intersection_2_liste(intersect[i], potentialKey_2[i]);
    }

    // afficherOctetPotentiel(intersect);
    return intersect;
}
