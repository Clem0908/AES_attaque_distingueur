#pragma once 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void memoryError(const char funcname[]);

void readError(const char funcname[]);

void writeError(const char funcname[]);

void openError(const char funcname[]);

void inputError(const char funcname[]);
