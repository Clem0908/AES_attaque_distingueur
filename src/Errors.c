#include "../headers/Errors.h"

void memoryError(const char funcname[])
{
	printf("\nE: memoryError at %s\n", funcname);
	exit(EXIT_FAILURE);
}

void readError(const char funcname[])
{
	printf("\nE: readError at %s\n", funcname);
	exit(EXIT_FAILURE);
}

void writeError(const char funcname[])
{
	printf("\nE: writeError at %s\n", funcname);
	exit(EXIT_FAILURE);
}

void openError(const char funcname[])
{
	printf("\nE: openError at %s\n", funcname);
	exit(EXIT_FAILURE);
}

void inputError(const char funcname[])
{
	printf("\nE: inputError at %s\n", funcname);
	exit(EXIT_FAILURE);
}
