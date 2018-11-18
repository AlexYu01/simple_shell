/*
 * File: builtin.c
 * Auth: Alex Yu
 *       Brennan D Baraban
 */

#include "shell.h"

int (*get_builtin(char *command))(char **args, char **front);
int shellby_exit(char **args, char **front);
int shellby_cd(char **args, char __attribute__((__unused__)) **front);
int shellby_alias(char **args, char __attribute__((__unused__)) **front);

/**
 * get_builtin - Matches a command with a corresponding
 *               shellby builtin function.
 * @command: The command to match.
 *
 * Return: A function pointer to the corresponding builtin.
 */
int (*get_builtin(char *command))(char **args, char **front)
{
	builtin_t funcs[] = {
		{ "exit", shellby_exit },
		{ "env", shellby_env },
		{ "setenv", shellby_setenv },
		{ "unsetenv", shellby_unsetenv },
		{ "cd", shellby_cd },
		{ "alias", shelly_alias },
		{ NULL, NULL }
	};
	int i;

	for (i = 0; funcs[i].name; i++)
	{
		if (_strncmp(funcs[i].name, command, _strlen(funcs[i].name)) == 0)
			break;
	}
	return (funcs[i].f);
}

/**
 * shellby_exit - Causes normal process termination
 *                for the shellby shell.
 * @args: An array of arguments containing the exit value.
 * @front: A double pointer to the beginning of args.
 *
 * Return: If there are no arguments - -3.
 *         If the given exit value is invalid - 2.
 *         O/w - exits with the given status value.
 *
 * Description: Upon returning -3, the program exits back in the main function.
 */
int shellby_exit(char **args, char **front)
{
	int i = 0, sign = 1;
	unsigned int num = 0;

	if (args[0])
	{
		if (args[0][i] == '-')
			sign = -1;
		for (; args[0][i]; i++)
		{
			if (args[0][i] == '-')
				sign *= -1;

			if (args[0][i] >= '0' && args[0][i] <= '9')
				num = (num * 10) + (args[0][i] - '0');
			else
				return (create_error(args, 2));
		}
	}
	else
	{
		return (-3);
	}
	args -= 1;
	free_args(args, front);
	free_env();
	exit(num * sign);
}

/**
 * shellby_cd - Changes the current directory of the shellby process.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 *
 * Return: If the given string is not a directory - 2.
 *         If an error occurs - -1.
 *         Otherwise - 0.

 */
int shellby_cd(char **args, char __attribute__((__unused__)) **front)
{
	char **dir_info;
	char *oldpwd = NULL, *pwd = NULL;
	struct stat dir;

	oldpwd = getcwd(oldpwd, 0);
	if (!oldpwd)
		return (-1);

	if (args[0])
	{
		if (*(args[0]) == '-')
			chdir(*(_getenv("OLDPWD")) + 7);
		else
		{
			if (stat(args[0], &dir) == 0 && S_ISDIR(dir.st_mode)
					&& ((dir.st_mode & S_IXUSR) != 0))
				chdir(args[0]);
			else
			{
				free(oldpwd);
				return (create_error(args, 2));
			}
		}
	}
	else
		chdir(*(_getenv("HOME")) + 5);

	pwd = getcwd(pwd, 0);
	if (!pwd)
		return (-1);

	dir_info = malloc(sizeof(char *) * 2);
	if (!dir_info)
		return (-1);

	dir_info[0] = "OLDPWD";
	dir_info[1] = oldpwd;
	if (shellby_setenv(dir_info, dir_info) == -1)
		return (-1);

	dir_info[0] = "PWD";
	dir_info[1] = pwd;
	if (shellby_setenv(dir_info, dir_info) == -1)
		return (-1);

	free(oldpwd);
	free(pwd);
	free(dir_info);
	return (0);
}

/**
 * shellby_alias -
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 *
 * Return: If an error occurs - -1.
 *         Otherwise - 0.
 */
int shellby_alias(char **args, char __attribute__((__unused__)) **front)
{
	alias_t *temp = aliases;
	int i, j, k, len = 0, ret = 0;
	char *value;

	if (!args[0])
	{
		while(temp)
		{
			printf("%s='%s'\n", temp->name, temp->value);
			temp = temp->next;
		}
		return (ret);
	}
	for (i = 0; args[i]; i++)
	{
		value = _strchar(args[i], "=");
		if (!value)
		{
			while (temp)
			{
				if (strcmp(args[i], temp->name) == 0)
				{
					printf("%s='%s'\n", temp->name, temp->value);
					break;
				}
			}
			if (!temp)
				ret = create_error(args, 1);
		}
		else
		{
			value = '\0';
			value++;
			len = ....;
			new_value = malloc(sizeof(char) * (len + 1));
			if (!new_value)
				continue;
			for (j = 0, k = 0; value[j]; j++)
			{
				if (value[j] != ''' && value[j] != '"')
					new_value[k++] = value[j];
			}
			new_value = '\0';
			while (temp)
			{
				if (strcmp(args[i], temp->name) == 0)
				{
					free(temp->value);
					temp->value = new_value;
					break;
				}
				temp = temp->next;
			}
			if (!temp)
				add_alias_end(head, args[i], new_value);
		}
	}
	return (ret);
}
