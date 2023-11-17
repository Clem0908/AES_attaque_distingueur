#include "../headers/FileOperations.h"
#include "../headers/Errors.h"
#include "../headers/Includes.h"
#include "../headers/AES.h"

int openFile(const char *const file)
{
	const int fd = open(file, O_RDWR);
	if (fd == -1)
		openError(__func__);

	return fd;
}

char *readFile(const int fd)
{
	char *buffer = (char *)calloc(MESSAGE_LENGTH + 1, sizeof(char));
	if (!buffer)
		memoryError(__func__);

	const int ret = read(fd, buffer, MESSAGE_LENGTH);

	if (ret == -1)
		readError(__func__);

	buffer[32] = '\0';

	return buffer;
}

void readFileCTR(const int fd, char *buffer){
	char c = '\0';
	int length = 0;
	int ret = 1;

	while( c != '\n' && ret != 0 ){
		ret = read(fd, &c, 1);
		buffer[length] = c;
		length++;
		if (ret == -1)
			readError(__func__);
	}
	buffer[length -1] = '\0';
	// return buffer;
}

char *readChosenFile(const int fd)
{
	char *const buffer = (char *)calloc(MAX_BUFFER_LENGTH, sizeof(char));
	if (!buffer)
		memoryError(__func__);

	const int ret = read(fd, buffer, MAX_BUFFER_LENGTH - 1);

	if (ret == -1)
		readError(__func__);

	if (ret == MAX_BUFFER_LENGTH - 1)
		return buffer;
	else
		return NULL;
}

byte **getHexFromFile(char *file)
{
	const int fd = openFile(file);
	char *hex = readFile(fd);
	byte **k = fromHexToMatrice(hex);
	free(hex);
	close(fd);

	return k;
}

char **arrangeBufferChosen(char *const buffer)
{
	if (!buffer)
		memoryError(__func__);

	char **fill_state = (char **)calloc(NB_CLAIRCHIFFRE, sizeof(char *));
	if (!fill_state)
		memoryError(__func__);
	for (int i = 0; i < NB_CLAIRCHIFFRE; i++)
	{
		fill_state[i] = (char *)calloc(MESSAGE_LENGTH + 1, sizeof(char));
		if (!fill_state[i])
			memoryError(__func__);
	}

	int j = 0;
	int cpt_buffer = 0;
	// Lecture des clairs et copie dans fill
	for (int i = 0; i < NB_CLAIRCHIFFRE; i++)
	{
		while (buffer[cpt_buffer] != '\n')
		{
			fill_state[i][j] = buffer[cpt_buffer];
			j++;
			cpt_buffer++;
		}
		fill_state[i][j] = '\0';
		j = 0;
		cpt_buffer++;
	}

	return fill_state;
}

void removeNewLinesAndAddCipher(char **fill_state, char *buffer, int fd, byte **key)
{
	if (!fill_state || !buffer || !key)
		memoryError(__func__);

	byte **mat = malloc(sizeof(byte *));
	if (!mat)
		memoryError(__func__);
	for (int i = 0; i < 4; i++)
	{
		mat[i] = malloc(4 * sizeof(byte));
		if (!mat[i])
			memoryError(__func__);
	}

	// Retrait des \n et écriture des chiffrés
	int cpt = 0;
	int cpt_new = 0;
	char *tmp_buf = malloc(sizeof(char) * 32);
	if (!tmp_buf)
		memoryError(__func__);
	char new_buffer[MAX_BUFFER_LENGTH * 2];

	for (int i = 0; i < NB_CLAIRCHIFFRE; i++)
	{
		mat = fromHexToMatrice(fill_state[i]);
		chiffrement(mat, key);

		if (buffer[cpt] == '\0')
		{
			new_buffer[cpt_new] = '\0';
			break;
		}
		while (buffer[cpt] != '\n')
		{
			new_buffer[cpt_new] = buffer[cpt];
			cpt++;
			cpt_new++;
		}

		cpt++;
		new_buffer[cpt_new] = ' ';
		cpt_new++;

		for (int j = 0; j < 32; j++)
		{
			tmp_buf = fromMatriceToHex(mat);
			new_buffer[cpt_new] = tmp_buf[j];
			cpt_new++;
		}

		new_buffer[cpt_new] = '\n';
		cpt_new++;
	}

	lseek(fd, 0, SEEK_SET);
	int ret = write(fd, new_buffer, (MAX_BUFFER_LENGTH - 1) * 2);
	if (ret == -1)
		writeError(__func__);
	if (ret != 16896)
		writeError(__func__);

	free(tmp_buf);
	for (int i = 0; i < 4; i++)
	{
		free(mat[i]);
	}
	free(mat);
}
