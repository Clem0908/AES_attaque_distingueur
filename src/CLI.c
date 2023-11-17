#include "../headers/CLI.h"

void liberer_liste_chainee(POTENTIAL_KEY_BYTE *const l)
{
    if (l == NULL)
        return;
    liberer_liste_chainee(l->next);
    free(l);
}

void executeChiffrement()
{
    byte **m = getHexFromFile("./in/plaintext.txt");
    byte **k = getHexFromFile("./in/key.txt");
    chiffrement(m, k);
    char *result = fromMatriceToHex(m);
    printf("Résultat du chiffrement : %s\n", result);
    int fd = openFile("./in/ciphertext.txt");
    write(fd, result, 32);
    close(fd);

    // Libération de la mémoire
    for (int i = 0; i < 4; i++)
    {
        free(m[i]);
        free(k[i]);
    }
    free(m);
    free(k);
    free(result);
}

void executeDechiffrement()
{
    byte **c = getHexFromFile("./in/ciphertext.txt");
    byte **k = getHexFromFile("./in/key.txt");
    dechiffrement(c, k);
    char *result = fromMatriceToHex(c);
    printf("Résultat du déchiffrement : %s\n", result);
    int fd = openFile("./in/plaintext.txt");
    write(fd, result, 32);
    close(fd);

    for (int i = 0; i < 4; i++)
    {
        free(c[i]);
        free(k[i]);
    }
    free(c);
    free(k);
    free(result);
}

void executeCTR()
{
    const int fd = openFile("./in/plaintext.txt");
    // char *plaintext = readFile(fd);

    char *plaintext = calloc(1024 + 1, sizeof(char));
    readFileCTR(fd, plaintext);
    char *result = CTREncryption(plaintext);
    printf("Résultat du chiffrement : %s\n", result);
    int fd1 = openFile("./in/ciphertext.txt");
    write(fd1, result, 32);
    close(fd1);
    close(fd);
    free(plaintext);
    freeIV();
    free(result);
}

void attaque_intersection()
{
    CLAIR_CHIFFRE **etats = creerCouplesClaireChiffre_intersection();
    POTENTIAL_KEY_BYTE **potentialKey_0 = keyPotentialBytes(etats[0]);
    POTENTIAL_KEY_BYTE **potentialKey_1 = keyPotentialBytes(etats[1]);
    POTENTIAL_KEY_BYTE **potentialKey_2 = keyPotentialBytes(etats[2]);
    POTENTIAL_KEY_BYTE **subKeyBytes = findSubKey_intersetion(potentialKey_0, potentialKey_1, potentialKey_2);
    byte **sub_key = creerMatriceZero();

    int k = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            sub_key[i][j] = subKeyBytes[k]->octet;
            k++;
        }
    }

    key_schedule_inverse(sub_key);
    char *masterKey = fromMatriceToHex(sub_key);
    printf("Voici la clé qui a été trouvée : %s\n", masterKey);

    /* Libération de mémoire */
    for (int i = 0; i < NB_CLAIRCHIFFRE; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            free(etats[0][i].clair[j]);
            free(etats[0][i].chiffre[j]);
            free(etats[1][i].clair[j]);
            free(etats[1][i].chiffre[j]);
            free(etats[2][i].clair[j]);
            free(etats[2][i].chiffre[j]);
        }
    }

    for (int i = 0; i < 16; i++)
    {
        liberer_liste_chainee(potentialKey_0[i]);
        liberer_liste_chainee(potentialKey_1[i]);
        liberer_liste_chainee(potentialKey_2[i]);
        liberer_liste_chainee(subKeyBytes[i]);
    }

    for (int i = 0; i < 4; i++)
        free(sub_key[i]);

    free(potentialKey_0);
    free(potentialKey_1);
    free(potentialKey_2);
    free(subKeyBytes);
    free(sub_key);
    free(etats);
}

void attaque_recursion()
{
    CLAIR_CHIFFRE *etats = creerCouplesClaireChiffre_recursion();
    POTENTIAL_KEY_BYTE **potentialKey = keyPotentialBytes(etats);
    byte **sub_key = creerMatriceZero();
    findSubKey_recursion(etats, potentialKey, sub_key, 0);

    /* Libération de mémoire */
    for (int i = 0; i < NB_CLAIRCHIFFRE; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            free(etats[i].clair[j]);
            free(etats[i].chiffre[j]);
        }
    }

    for (int i = 0; i < 16; i++)
        liberer_liste_chainee(potentialKey[i]);

    for (int i = 0; i < 4; i++)
        free(sub_key[i]);

    free(potentialKey);
    free(sub_key);
    free(etats);
}

void description_commande()
{
    puts("\nE: Arguments non entrés lors de l'exécution");
    puts("Usage : ./bin/AES chiffrer | dechiffrer");
    puts("Usage : ./bin/AES attaquer [recursion]");
}

void cmd(const int argc, char **const argv)
{
    if (argc < 2)
    {
        description_commande();
        exit(EXIT_FAILURE);
    }

    else if (strncmp(argv[1], "chiffrer", 9) == 0)
    {
        printf("\n-----Chiffrement-----\n");
        executeChiffrement();
    }

    else if (strncmp(argv[1], "dechiffrer", 11) == 0)
    {
        printf("\n-----Dechiffrement-----\n");
        executeDechiffrement();
    }

    else if (strncmp(argv[1], "chiffrer_CTR", 13) == 0)
    {
        printf("\n-----Chiffrement CTR-----\n");
        executeCTR();
    }

    else if (strncmp(argv[1], "attaquer", 9) == 0)
    {
        printf("\n-----Attaque-----\n");
        if (argc == 3 && strncmp(argv[2], "recursion", 10) == 0)
        {
            printf("\nCréation de couples clair-chiffré (1/1)...\n");
            system("./create_clear_cipher_couples.sh 0");

            attaque_recursion();

            printf("\nNettoyage de fichiers de couples\n");
            system("rm ./in/clear_cipher_couples_0.txt");
        }
        else
        {
            printf("\nCréation de couples clair-chiffré (1/3)...\n");
            system("./create_clear_cipher_couples.sh 0");
            printf("Création de couples clair-chiffré (2/3)...\n");
            system("./create_clear_cipher_couples.sh 1");
            printf("Création de couples clair-chiffré (3/3)...\n");
            system("./create_clear_cipher_couples.sh 2");

            attaque_intersection();

            printf("\nNettoyage de fichiers de couples\n");
            system("rm ./in/clear_cipher_couples_0.txt");
            system("rm ./in/clear_cipher_couples_1.txt");
            system("rm ./in/clear_cipher_couples_2.txt");
        }
    }

    else
    {
        description_commande();
        exit(EXIT_FAILURE);
    }
}
