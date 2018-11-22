/*
 * File: builtin.c
 * Auth: Alex Yu
 *       Brennan D Baraban
 */

#include "shell.h"

int (*get_builtin(char *command))(char **args, char **front);
int shellby_exit(char **args, char **front);
int shellby_cd(char **args, char __attribute__((__unused__)) **front);
int shellby_help(char **args, char __attribute__((__unused__)) **front);
int shellby_history(char **args, char __attribute__((__unused__)) **front);

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
		{ "alias", shellby_alias },
		{ "help", shellby_help },
		{ "history", shellby_history },
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
	int i = 0, len_of_int = 10;
	unsigned int num = 0, max = 1 << (sizeof(int) * 8 - 1);

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
				return (create_error(--args, 2));
		}
	}
	else
		return (-3);

	if (num > max - 1)
		return (create_error(--args, 2));

	args -= 1;
	save_history();
	free_args(args, front);
	free_env();
	free_alias_list(aliases);
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
 * shellby_help - Displays information about shellby builtin commands.
 * @args: An array of arguments.
 * @front: A pointer to the beginning of args.
 *
 * Return: If an error occurs - -1.
 *         Otherwise - 0.
 */
int shellby_help(char **args, char __attribute__((__unused__)) **front)
{
	if (!args[0])
		help_all();
	else if (_strcmp(args[0], "alias") == 0)
		help_alias();
	else if (_strcmp(args[0], "cd") == 0)
		help_cd();
	else if (_strcmp(args[0], "exit") == 0)
		help_exit();
	else if (_strcmp(args[0], "env") == 0)
		help_env();
	else if (_strcmp(args[0], "setenv") == 0)
		help_setenv();
	else if (_strcmp(args[0], "unsetenv") == 0)
		help_unsetenv();
	else if (_strcmp(args[0], "help") == 0)
		help_help();
	else
		write(STDERR_FILENO, name, _strlen(name));

	return (0);
}

/**
 * shellby_history - Displays the history list, one command per line,
 *                   preceded with line numbers (starting at 0).
 * @args: An array of arguments.
 * @front: A pointer to the beginning of args.
 *
 * Return: If an error occurs - -1.
 *         Otherwise - 0.
 */
int shellby_history(char **args, char __attribute__((__unused__)) **front)
{
	history_t *tmp = history;
	unsigned int count = 0, shift;
	char *count_str, *space = " ", *new_line = "\n";

	(void)args;

	while (tmp)
		tmp = tmp->next;

	tmp = history;
	if (tmp->count >= 4096)
	{
		shift = (count % 4096) + 1;
		while (shift)
		{
			tmp = tmp->next;
			shift--;
		}
	}

	while (tmp)
	{
		count_str = _itoa(tmp->count);
		if (!count_str)
			return (-1);
		write(STDOUT_FILENO, count_str, _strlen(count_str));
		write(STDOUT_FILENO, space, 1);
		write(STDOUT_FILENO, tmp->command, _strlen(tmp->command));
		write(STDOUT_FILENO, new_line, 1);
		tmp = tmp->next;
		free(count_str);
	}

	return (0);
}
