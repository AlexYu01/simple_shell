#include "shell.h"

extern char **environ;

char *_getenv(const char *name)
{
	int index;
	int len;

	len = strlen(name);
	for (index = 0; environ[index]; index++)
	{
		if (strncmp(name, environ[index], len) == 0)
			return (environ[index]);
	}
	return (NULL);
}

int _setenv(const char *name, const char *value, int overwrite)
{
	char *env_var;
	char **new_environ;
	size_t size;
	int index, index2;
	size_t len_name = strlen(name);
	size_t len_value = strlen(value);
	char *new_value;

	env_var = _getenv(name);

	new_value = malloc(len_name + 1 + len_value + 1);
	for (index = 0; name[index]; index++)
		new_value[index] = name[index];
	new_value[index++] = '=';
	for (index2 = 0; value[index2]; index++, index2++)
		new_value[index] = value[index2];
	new_value[index] = '\0';

	for (size = 0; environ[size]; size++)
		;

	if (env_var)
		new_environ = malloc(sizeof(char *) * (size + 1));
	else
		new_environ = malloc(sizeof(char *) * (size + 2));
	if (!new_environ)
	{
		free(new_value);
		return (-1);
	}

	for (index = 0; environ[index]; index++)
	{
		new_environ[index] = malloc(strlen(environ[index] + 1));
		if (!new_environ[index])
		{
			for(index--; index >= 0; index--)
				free(new_environ[index]);
			free(new_environ);
			free(new_value);
			return (-1);
		}
		strcpy(new_environ[index], environ[index]);
	}
	environ = new_environ;
	env_var = _getenv(name);
	if (env_var && overwrite)
	{
		env_var = new_value;
		return (0);
	}
	environ[index] = new_value;
	environ[index + 1] = NULL;
	return (0);
}

int _unsetenv(const char *name)
{
	char *env_var;
	char **new_environ;
	size_t size;
	int index, index2;

	env_var = _getenv(name);
	if (!env_var)
		return (0);

	for (size = 0; environ[size]; size++)
		;

	new_environ = malloc(sizeof(char *) * size);
	if (!new_environ)
		return (-1);

	for (index = 0, index2 = 0; environ[index]; index++)
	{
		if (env_var == environ[index])
			continue;
		new_environ[index2] = malloc(strlen(environ[index] + 1));
		if (!new_environ[index2])
		{
			for(index2--; index2 >= 0; index2--)
				free(new_environ[index2]);
			free(new_environ);
			return (-1);
		}
		strcpy(new_environ[index2], environ[index]);
		index2++;
	}
	environ = new_environ;
	environ[size - 1] = NULL;
	return (0);
}
