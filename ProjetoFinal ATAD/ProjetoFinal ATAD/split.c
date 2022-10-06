#include "split.h"

char** split(char *string, int nFields, const char *delim) {
	char **tokens = malloc(sizeof(char*) * nFields);

	int index = 0;
	char *next_token = NULL;

	char *token = (char*)strtok_s(string, delim, &next_token);

	while (token) {
		tokens[index++] = token;
		token = strtok_s(NULL, delim, &next_token);
	}

	return tokens;
}