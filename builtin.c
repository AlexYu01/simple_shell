/*
 * File: builtin.c
 * Auth: Alex Yu
 *       Brennan D Baraban
 */

#include "shell.h"

int (*get_builtin(char *command))(char **args);
int shellby_exit(char **args);

/**
 * get_builtin - Matches a command with a corresponding
 *               shellby builtin function.
 * @command: The command to match.
 *
 * Return: A function pointer to the corresponding builtin.
 */
int (*get_builtin(char *command))(char **args)
{
	builtin_t funcs[] = {
		{ "exit", shellby_exit },

	/*	{ "env", shellby_env },
		{ "setenv", shellby_setenv },
		{ "unsetenv", shellby_unsetenv },
		{ "cd", shellby_cd },
		{ "alias", shelly_alias },
	*/	{ NULL, NULL }
	};
	int i;

	for (i = 0; funcs[i].name; i++)
	{
		if (strcmp(funcs[i].name, command) == 0)
			break;
	}
	return (funcs[i].f);
}

/**
 * shellby_exit - Causes normal process termination
 *                for the shellby shell.
 * @args: An array of arguments containing the exit value.
 *
 * Return: If the given exit value is invalid - 2.
 *         O/w - exits with the given status value.
 *
 * Description: If no exit value is given, the function exits with 0.
 */
int shellby_exit(char **args)
{
	int i = 0, sign = 1;
	unsigned int num = 0;

	if (args[1])
	{
		if (args[1][i] == '-')
			sign = -1;
		for (; args[1][i]; i++)
		{
			if (args[1][i] == '-')
				sign *= -1;

			if (args[1][i] >= '0' && args[1][i] <= '9')
				num = (num * 10) + (args[1][i] - '0');
			else
				return (2);
		}
	}

	for (i = 0; args[i]; i++)
		free(args[i]);
	free(args);
	free_env();
	exit(num * sign);
}
