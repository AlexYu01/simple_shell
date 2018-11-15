/*
 * File: error_msgs.c
 * Auth: Alex Yu
 *       Brennan D Baraban
 */

#include "shell.h"

char *error_env(char *name, int hist, char **args);
char *error_2_exit(char *name, int hist, char **args);
char *error_2_cd(char *name, int hist, char **args);
char *error_126(char *name, int hist, char **args);
char *error_127(char *name, int hist, char **args);

/**
 * error_env - Creates an error message for shellby_env errors.
 * @name: The command name.
 * @hist: The history number of the command.
 * @args: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *error_env(char *name, int hist, char **args)
{
	char *error, *hist_str;
	int len;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	len = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 45;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": ");
	_strcat(error, args[0]);
	_strcat(error, ": ");
	_strcat(error, "Unable to add/remove from environment\n");

	free(hist_str);
	return (error);
}

/**
 * error_2_exit - Creates an error message for shellby_exit errors.
 * @name: The command name.
 * @hist: The history number of the command.
 * @args: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *error_2_exit(char *name, int hist, char **args)
{
	char *error, *hist_str;
	int len;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	len = _strlen(name) + _strlen(hist_str) + _strlen(args[0])
	      + _strlen(args[1]) + 23;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": ");
	_strcat(error, args[0]);
	_strcat(error, ": ");
	_strcat(error, "Illegal Number");
	_strcat(error, ": ");
	_strcat(error, args[1]);
	_strcat(error, "\n");

	free(hist_str);
	return (error);
}

/**
 * error_2_cd - Creates an error message for shellby_cd errors.
 * @name: The command name.
 * @hist: The history number of the command.
 * @args: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *error_2_cd(char *name, int hist, char **args)
{
	char *error, *hist_str;
	int len;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	len = _strlen(name) + _strlen(hist_str) + _strlen(args[0])
	      + _strlen(args[1]) + 18;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": ");
	_strcat(error, args[0]);
	_strcat(error, ": ");
	_strcat(error, "can't cd to ");
	_strcat(error, args[1]);
	_strcat(error, "\n");

	free(hist_str);
	return (error);
}

/**
 * error_126 - Creates an error message for permission denied failures.
 * @name: The command name.
 * @hist: The history number of the command.
 * @args: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *error_126(char *name, int hist, char **args)
{
	char *error, *hist_str;
	int len;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	len = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 24;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": ");
	_strcat(error, args[0]);
	_strcat(error, ": ");
	_strcat(error, "Permission denied\n");

	free(hist_str);
	return (error);
}

/**
 * error_127 - Creates an error message for command not found failures.
 * @name: The command name.
 * @hist: The history number of the command.
 * @args: An array of arguments passed to the command.
 *
 * Return: The error string.
 */
char *error_127(char *name, int hist, char **args)
{
	char *error, *hist_str;
	int len;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	len = _strlen(name) + _strlen(hist_str) + _strlen(args[0]) + 16;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	_strcpy(error, name);
	_strcat(error, ": ");
	_strcat(error, hist_str);
	_strcat(error, ": ");
	_strcat(error, args[0]);
	_strcat(error, ": ");
	_strcat(error, "not found\n");

	free(hist_str);
	return (error);
}
