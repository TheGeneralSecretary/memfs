#include "path_parser.h"

#include <malloc.h>
#include <string.h>
#include <stdio.h>

size_t _delim_occurrence(const char *path, char delim)
{
		size_t c = 0;
		for (size_t i = 0; i < strlen(path); i++)
				if (path[i] == delim)
						c++;
		return c;
}

char **parse_path(const char *path)
{
		char *tmp = NULL;
		char *saveptr = NULL;
		char **tokens = NULL;
		size_t i = 0;
		size_t size = _delim_occurrence(path, '/') + 1;

		tokens = malloc(size * sizeof(char *));
		if (!tokens)
				return NULL;

		tmp = strdup(path);

		char *token = strtok_r(tmp, "/", &saveptr);
		while (token) {
				size_t len = strlen(token) + 2;
				tokens[i] = malloc(len);
				snprintf(tokens[i++], len, "/%s", token);
				token = strtok_r(NULL, "/", &saveptr);
		}

		tokens[i] = NULL;
		free(tmp);
		return tokens;
}
