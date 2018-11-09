#include "shell.h"

char **_strtok(char *line, char *delim)
{
	char *token;
	char **ptr;
	int index;
	size_t words = 0;

	for (index = 0; line[index]; index++)
	{
		if (line[index] != *delim && (line[index + 1] == *delim || line[index + 1] == '\0'))
				words++;
	}
	line[index - 1] = '\0';
	ptr = malloc(sizeof(char *) * (words + 1));
	if (!ptr)
	{
		printf("Boom\n");
		return (0);
	}
	token = strtok(line, delim);;
	for (index = 0; token != NULL; index++)
	{
		ptr[index] = malloc(strlen(token) + 1);
		if (!ptr[index])
		{
			for (index -= 1; index >= 0; index--)
				free(ptr[index]);
			free(ptr);
			exit(99);
		}
		strcpy(ptr[index], token);
		token = strtok(NULL, delim);
	}
	ptr[index] = NULL;

	return (ptr);
}
