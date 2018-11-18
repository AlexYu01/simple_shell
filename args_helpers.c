/*
 * File: args_helpers.c
 * Auth: Alex Yu
 *       Brennan D Baraban
 */

#include "shell.h"

char *get_args(char *line, int *exe_ret);
int call_args(char **args, char **front, char *name, int *hist, int *exe_ret);
int run_args(char **args, char **front, char *name, int *hist, int *exe_ret);
int handle_args(char *name, int *hist, int *exe_ret);

/**
 * get_args - Gets a command from standard input.
 * @line: A buffer to store the command.
 * @exe_ret: The return value of the last executed command.
 *
 * Return: If an error occurs - NULL.
 *         Otherwise - a pointer to the stored command.
 */
char *get_args(char *line, int *exe_ret)
{
	size_t n = 0;
	ssize_t read;

	if (line)
		free(line);

	read = _getline(&line, &n, STDIN_FILENO);
	if (read == -1)
		return (NULL);
	if (read == 1)
	{
		if (isatty(STDIN_FILENO))
			printf("$ ");
		return (get_args(line, exe_ret));
	}

	line[read - 1] = '\0';
	variable_replacement(&line, exe_ret);
	handle_line(&line, read);

	return (line);
}

/**
 * call_args - Partitions operators from commands and calls them.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 * @name: The name of the call.
 * @hist: The history number of the call.
 * @exe_ret: The return value of the parent process' last executed command.
 *
 * Return: The return value of the last executed command.
 */
int call_args(char **args, char **front, char *name, int *hist, int *exe_ret)
{
	int ret, index;

	for (index = 0; args[index]; index++)
	{
		if (_strncmp(args[index], "||", 2) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			ret = run_args(args, front, name, hist, exe_ret);
			if (*exe_ret != 0)
			{
				args = &args[++index];
				index = 0;
			}
			else
			{
				for (index++; args[index]; index++)
					free(args[index]);
				return (ret);
			}
		}
		else if (_strncmp(args[index], "&&", 2) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			ret = run_args(args, front, name, hist, exe_ret);
			if (*exe_ret == 0)
			{
				args = &args[++index];
				index = 0;
			}
			else
			{
				for (index++; args[index]; index++)
					free(args[index]);
				return (ret);
			}
		}
	}

	ret = run_args(args, front, name, hist, exe_ret);

	return (ret);
}

/**
 * run_args - Calls the execution of a command.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 * @name: The name of the call.
 * @hist: The history number of the call.
 * @exe_ret: The return value of the parent process' last executed command.
 *
 * Return: The return value of the last executed command.
 */
int run_args(char **args, char **front, char *name, int *hist, int *exe_ret)
{
	int ret, i;
	int (*builtin)(char **args, char **front);

	builtin = get_builtin(args[0]);
	if (builtin)
	{
		ret = builtin(args + 1, front);
		if (ret != EXIT)
		{
			*exe_ret = ret;
			if (ret != 0)
				create_error(name, *hist, args, ret);
		}
	}
	else
	{
		*exe_ret = execute(args, front, name, *hist);
		ret = *exe_ret;
	}

	(*hist)++;

	for (i = 0; args[i]; i++)
		free(args[i]);

	return (ret);
}

/**
 * handle_args - Gets, calls, and runs the execution of a command.
 * @name: The name of the call.
 * @hist: The history number of the call.
 * @exe_ret: The return value of the parent process' last executed command.
 *
 * Return: If an end-of-file is read - END_OF_FILE (-2).
 *         If the input cannot be tokenized - -1.
 *         O/w - The exit value of the last executed command.
 */
int handle_args(char *name, int *hist, int *exe_ret)
{
	int ret, index;
	char **args, *line = NULL, **front;

	line = get_args(line, exe_ret);
	if (!line)
		return (END_OF_FILE);

	args = _strtok(line, " ");
	free(line);
	if (!args)
		return (0);
	front = args;

	for (index = 0; args[index]; index++)
	{
		if (_strncmp(args[index], ";", 1) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			ret = call_args(args, front, name, hist, exe_ret);
			args = &args[++index];
			index = 0;
		}
	}

	ret = call_args(args, front, name, hist, exe_ret);

	free(front);
	return (ret);
}
