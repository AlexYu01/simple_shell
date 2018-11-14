/*
 * File: environ.c
 * Auth: Alex Yu
 *       Brennan D Baraban
 */

#include "shell.h"

char *_getenv(const char *name);
int _setenv(const char *name, const char *value, int overwrite);
int _unsetenv(const char *name);


/**
 * _copyenv - Creates a copy of the environment.
 *
 * Return: Double pointer to the copy of the environment.
 *	   Or NULL if copy fails.
 */
char ** _copyenv(void)
{
	char **new_envrion;
	size_t size;
	int index;

	for (size=0; environ[size]; size++)
		;

	new_environ = malloc(sizeof(char *) * (size + 1));
	if (!new_environ)
		return (NULL);

	for (index = 0; environ[index]; index++)
	{
		new_environ[index] = malloc(strlen(environ[index] + 1));
		if (!new_environ[index])
		{
			for (index--; index >= 0; index--)
				free(new_environ[index]);
			free(new_environ);
			return (NULL);
		}
		strcpy(new_environ[index], environ[index]);
	}
	return (new_environ);
}
/**
 * _getenv - Gets an environmental variable from the PATH.
 * @name: The name of the environmental variable to get.
 *
 * Return: If the environmental variable does not exist - NULL.
 *         Otherwise - a pointer to the environmental variable.
 */
char *_getenv(const char *name)
{
	int index, len;

	len = strlen(name);
	for (index = 0; environ[index]; index++)
	{
		if (strncmp(name, environ[index], len) == 0)
			return (environ[index]);
	}
	return (NULL);
}

/**
 * _setenv - Changes or adds an environmental variable to the PATH.
 * @name: The name of the environmental variable to change or add.
 * @value: The value of the environmental variable to change or add.
 * @overwrite: A flag indicating if the variable should be overwritten.
 *
 * Return: If an error occurs - -1.
 *         Otherwise - 0.
 */
int _setenv(const char *name, const char *value, int overwrite)
{
	char *env_var, **new_environ, *new_value;
	size_t size;
	int index;
	size_t len_name = strlen(name), len_value = strlen(value);

	env_var = _getenv(name);

	new_value = malloc(len_name + 1 + len_value + 1);
	strcpy(new_value, name);
	strcat(new_value, "=");
	strcat(new_value, value);

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
		new_environ[index] = environ[index];

	free(environ);
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

/**
 * _unsetenv - Deletes an environmental variable from the PATH.
 * @name: The name of the environmental variable to delete.
 *
 * Return: If an error occurs - -1.
 *         Otherwise - 0.
 */
int _unsetenv(const char *name)
{
	char *env_var, **new_environ;
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
			for (index2--; index2 >= 0; index2--)
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
