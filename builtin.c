/*
 * File: builtin.c
 * Auth: Alex Yu
 *       Brennan D Baraban
 */

#include "shell.h"

int (*get_builtin(char *command))(char **args);
int shellby_exit(char **args);
int shellby_cd(char **args);

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

	//	{ "env", shellby_env },
	//	{ "setenv", shellby_setenv },
	//	{ "unsetenv", shellby_unsetenv },
		{ "cd", shellby_cd },
	//	{ "alias", shelly_alias },
		{ NULL, NULL }
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

/**
 * shellby_cd - Changes the current directory of the shellby process.
 * @args: An array of arguments.
 *
 * Return: If an error occurs - -1.
 *         Otherwise - 0.
 */
int shellby_cd(char **args)
{
	char *oldpwd = NULL, *pwd = NULL;
	struct stat dir;

	oldpwd = getcwd(oldpwd, 0);
	if (!oldpwd)
		return (-1);

	if (args[1])
	{
		if (*(args[1]) == '-')
			chdir(_getenv("OLDPWD"));
		else
		{
			if (stat(args[1], &dir) == 0 && S_ISDIR(dir.st_mode))
				chdir(args[1]);
			else
			{
				free(oldpwd);
				return (2);
			}
		}
	}
	else
		chdir(_getenv("HOME"));

	pwd = getcwd(pwd, 0);
	if (!pwd)
		return (-1);

	//_setenv("OLDPWD", oldpwd, 1);
	//_setenv("PWD", pwd, 1);

	free(oldpwd);
	free(pwd);

	return (0);
}
