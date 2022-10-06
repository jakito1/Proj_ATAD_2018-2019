#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Função que separa uma String pelo delimitador*/
char** split(char *string, int nFields, const char *delim);