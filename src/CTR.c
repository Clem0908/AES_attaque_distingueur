#include "../headers/CTR.h"
#include "../headers/Errors.h"

IVSTRUCT *IV;

CTRBYTESSTRUCT *ctrbs;

int multiple;
char **ret;

//-1 non défni, 0 par défaut, 1 défini par utilisateur
int g_IV_state = -1;

void askIV()
{
	printf("\nVoulez-vous choisir un vecteur d'initialisation pour le mode CTR (IV) ? [o/N]: ");
	char *choix = malloc(sizeof(char));
	int check = scanf("%c", choix);
	if(check != 1)
		inputError(__func__);

	switch(*choix)
	{
		case 'o':
			chosenIV();
			break;
		case 'N':
			initializeIV();
			break;
		case '\n':
			initializeIV();
			break;
		default:
			initializeIV();
			break;
	}
}

void initializeIV()
{

	printf("\nVecteur d'initialisation par défaut choisi.\n");
	IV = (IVSTRUCT *)malloc(sizeof(IVSTRUCT));
	if (!IV)
		memoryError(__func__);

	IV->IV = (byte **)calloc(4, sizeof(byte *));
	IV->cpyIV = (byte **)calloc(4, sizeof(byte *));
	if (!IV->IV || !IV->cpyIV)
		memoryError(__func__);
	for (int i = 0; i < 4; i++)
	{
		IV->IV[i] = (byte *)calloc(4, sizeof(byte));
		IV->cpyIV[i] = (byte *)calloc(4, sizeof(byte));
		if (!IV->IV[i] || !IV->cpyIV[i])
			memoryError(__func__);
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			IV->IV[i][j] = (byte)random();
			IV->cpyIV[i][j] = IV->IV[i][j];
		}
	}
	g_IV_state = 0;
}

void chosenIV()
{
	IV = (IVSTRUCT *)malloc(sizeof(IVSTRUCT));
	if (!IV)
		memoryError(__func__);

	IV->IV = (byte **)calloc(4, sizeof(byte *));
	IV->cpyIV = (byte **)calloc(4, sizeof(byte *));
	if (!IV->IV || !IV->cpyIV)
		memoryError(__func__);
	for (int i = 0; i < 4; i++)
	{
		IV->IV[i] = (byte *)calloc(4, sizeof(byte));
		IV->cpyIV[i] = (byte *)calloc(4, sizeof(byte));
		if (!IV->IV[i] || !IV->cpyIV[i])
			memoryError(__func__);
	}

	printf("\nVeuillez entrer le vecteur d'initialisation choisi en hexadécimal (32 caractères): ");
	char *input = malloc(sizeof(char) * 32);
	if(!input)
		memoryError(__func__);

	int check = scanf("%s", input);
	if(check != 1)
		inputError(__func__);

	int cpt = 0;

	byte **matIV = malloc(sizeof(byte *) * 4);
	if(!matIV)
		memoryError(__func__);
	for(int i = 0; i < 4; i++)
	{
		matIV[i] = malloc(sizeof(byte) * 4);
		if(!matIV[i])
			memoryError(__func__);
	}

	matIV = fromHexToMatrice(input);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			IV->IV[i][j] = matIV[i][j];
			IV->cpyIV[i][j] = IV->IV[i][j];
			cpt++;
		}
	}

	g_IV_state = 1;
	free(input);
}

void freeIV()
{
	for (int i = 0; i < 4; i++)
	{
		free(IV->IV[i]);
		free(IV->cpyIV[i]);
	}
	free(IV->IV);
	free(IV->cpyIV);
	free(IV);
}

byte **XORBlock(byte **block1, byte **block2)
{
	if (!block1 || !block2)
		memoryError(__func__);

	byte **res = (byte **)calloc(4, sizeof(byte *));
	if (!res)
		memoryError(__func__);

	for (int i = 0; i < 4; i++)
	{
		res[i] = (byte *)calloc(4, sizeof(byte));
		if (!res[i])
			memoryError(__func__);
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			res[i][j] = block1[i][j] ^ block2[i][j];
		}
	}

	return res;
}

void *CTRCompute(void *arg)
{
	CTRCOMPUTESTRUCT *ctrcs;
	ctrcs = (CTRCOMPUTESTRUCT *)arg;
	int i = ctrcs->it;

	chiffrement(IV->cpyIV, ctrcs->key);
	ctrcs->ctrbs->res[i] = XORBlock(IV->cpyIV, ctrcs->ctrbs->mult_m_blocks[i]);
	for (int j = 0; j < 4; j++)
	{
		for (int k = 0; k < 4; k++)
		{
			IV->cpyIV[j][k] = IV->IV[j][k];
		}
	}


	int j = 3; int k = 3;
	while(IV->IV[j][k] == 255 && k != 0)
	{
		IV->cpyIV[j][k-1] += 1 + (1 * i);
		j--;
		k--;
	}
	
	IV->cpyIV[3][3] += 1 + (1 * i);

	char *buffer = fromMatriceToHex(ctrcs->ctrbs->res[i]);
	for (int j = 0; j < 32; j++)
	{
		ret[i][j] = buffer[j];
	}

	free(buffer);	
	pthread_exit(NULL);
}

char **complete_plaintext(const char *const plaintext)
{
	// strlen(plaintext): +1 car \0 en moins
	if (!plaintext)
		memoryError(__func__);
	char *new_plaintext = (char *)calloc(strlen(plaintext) + 1, sizeof(char));
	if (!new_plaintext)
		memoryError(__func__);

	new_plaintext = memcpy(new_plaintext, plaintext, strlen(plaintext));
	new_plaintext[strlen(plaintext)] = '\0';
	// Compléter le message clair pour qu'il soit mod 32
	int i = strlen(new_plaintext);
	int cpt = 1;

	while (i % 32)
	{
		// new_plaintext = reallocarray(new_plaintext, (i + 2), sizeof(char));
		new_plaintext = (char *)realloc(new_plaintext, sizeof(char) * (i + 2));
		if (!new_plaintext)
			memoryError(__func__);

		new_plaintext[i] = '0';
		new_plaintext[i + 1] = '\0';
		i++;
		cpt++;
	}

	multiple = i / 32;

	// Allocation mémoire pour les blocs de 32 du message
	char **mult_plaintext = (char **)calloc(multiple, sizeof(char *));
	if (!mult_plaintext)
		memoryError(__func__);
	for (int i = 0; i < multiple; i++)
	{
		mult_plaintext[i] = (char *)calloc(34, sizeof(char));
		if (!mult_plaintext[i])
			memoryError(__func__);
	}

	// Remplissage du tableau des mult de 32
	for (int i = 0; i < multiple; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			mult_plaintext[i][j] = new_plaintext[j + (i * 32)];
		}
		mult_plaintext[i][32] = '\0';
	}

	free(new_plaintext);

	return mult_plaintext;
}

void free_mult_plaintext(char **mult_plaintext)
{
	if (!mult_plaintext)
		memoryError(__func__);

	for (int i = 0; i < multiple; i++)
	{
		free(mult_plaintext[i]);
	}

	free(mult_plaintext);
}

CTRBYTESSTRUCT *initialize_ctrbs(char **mult_plaintext)
{
	if (!mult_plaintext)
		memoryError(__func__);

	CTRBYTESSTRUCT *ctrbs = (CTRBYTESSTRUCT *)malloc(sizeof(CTRBYTESSTRUCT));
	if (!ctrbs)
		memoryError(__func__);
	// Allocation mémoire pour tous les blocs à chiffrer
	ctrbs->mult_m_blocks = (byte ***)calloc(multiple + 1, sizeof(byte **));
	if (!ctrbs->mult_m_blocks)
		memoryError(__func__);

	ctrbs->res = (byte ***)calloc(multiple + 1, sizeof(byte **));
	if (!ctrbs->res)
		memoryError(__func__);

	return ctrbs;
}

void free_ctrbs(CTRBYTESSTRUCT *ctrbs)
{
	if (!ctrbs)
		memoryError(__func__);

	for (int i = 0; i < multiple; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			free(ctrbs->mult_m_blocks[i][j]);
			free(ctrbs->res[i][j]);
		}
		free(ctrbs->mult_m_blocks[i]);
		free(ctrbs->res[i]);
	}

	free(ctrbs->mult_m_blocks);
	free(ctrbs->res);
	free(ctrbs);
}

char *CTREncryption(char *plaintext)
{
	if (!plaintext)
		memoryError(__func__);

	if(g_IV_state == -1)
	{
		printf("\nLe vecteur d'initialisation est non défini, nous allons le générer");
		askIV();
	}

	char **mult_plaintext = complete_plaintext(plaintext);
	ctrbs = initialize_ctrbs(mult_plaintext);

	byte **key = getHexFromFile("./in/key.txt");

	// Blocs à chiffrer(Ek), puis IV++ puis XORer avec le clair
	for (int i = 0; i < multiple; i++)
	{
		ctrbs->mult_m_blocks[i] = fromHexToMatrice(mult_plaintext[i]);
	}

	pthread_t tid[multiple];
	CTRCOMPUTESTRUCT *ctrcs = (CTRCOMPUTESTRUCT *)calloc(multiple, sizeof(CTRCOMPUTESTRUCT));
	if (!ctrcs)
		memoryError(__func__);

	ret = (char **)calloc(multiple, sizeof(char *));
	if (!ret)
		memoryError(__func__);
	for (int i = 0; i < multiple; i++)
	{
		ret[i] = (char *)calloc(33, sizeof(char));
		if (!ret[i])
			memoryError(__func__);
	}

	for (int i = 0; i < multiple; i++)
	{
		ctrcs[i].it = i;
		ctrcs[i].ctrbs = ctrbs;
		ctrcs[i].key = key;
		pthread_create(&tid[i], NULL, CTRCompute, &ctrcs[i]);
	}

	for (int i = 0; i < multiple; i++)
	{
		pthread_join(tid[i], NULL);
	}

	char *retcat = (char *)calloc(multiple * 32, sizeof(char));
	if (!retcat)
		memoryError(__func__);
	int cpt = 0;
	for (int i = 0; i < multiple * 32; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			retcat[i] = ret[cpt][j];
			i++;
		}
		cpt++;
		i--;
	}

	free_ctrbs(ctrbs);
	free(ctrcs);
	free_mult_plaintext(mult_plaintext);
	
	for(int i = 0; i < 4; i++)
	{
		free(key[i]);
	}
	free(key);

	for (int i = 0; i < multiple; i++)
	{
		free(ret[i]);
	}
	free(ret);

	return retcat;
}
