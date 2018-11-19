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
	char *all = "Shellby\nThese shell commands are defined internally.\n"
		    "Type 'help' to see this list.\nType 'help name' to find "
		    "out more about the function 'name'.\n\n  alias   \t"
		    "alias [NAME[='VALUE'] ...]\n  cd    \tcd   "
		    "[DIRECTORY]\n  exit    \texit [STATUS]\n  env     \tenv"
		    "\n  setenv  \tsetenv [VARIABLE] [VALUE]\n  unsetenv\t"
		    "unsetenv [VARIABLE]\n";
	char *alias = "alias: alias [NAME[='VALUE'] ...]\nHandles aliases.\n\n"
		      "alias: Prints a list of all aliases, one per line, in "
		      "the format NAME='VALUE'.\nalias name [name2 ...]:prints"
		      " the aliases name, name2, etc. one per line, in the "
		      "form NAME='VALUE'.\nalias NAME='VALUE' [...]: Defines"
		      " an alias for each NAME whose VALUE is given. If NAME "
		      "is already an alias, replace its value with VALUE.\n";
	char *cd = "cd: cd [DIRECTORY]\n\tChanges the current directory of the"
		   " process to DIRECTORY.\n\n\tIf no argument is given, the "
		   "command is interpreted as cd $HOME. If the argument '-' is"
		   " given, the command is interpreted as cd $OLDPWD.\n\n"
		   "\tThe environment variables PWD and OLDPWD are updated "
		   "after a change of directory.\n";
	char *exit = "exit: exit [STATUS]\n\tExits the shell.\n\n\tThe "
		    "STATUS argument is the integer used to exit the shell."
		    " If no argument is given, the command is interpreted as"
		    "exit 0.\n";
	char *env = "env: env\n\tPrints the current environment.\n";
	char *setenv = "setenv: setenv [VARIABLE] [VALUE]\n\tInitializes a new"
		       "environment variable, or modifies an existing one.\n\n"
		       "\tUpon failure, prints a message to stderr.\n";
	char *unsetenv = "unsetenv: unsetenv [VARIABLE]\n\tRemoves an "
		         "environmental variable.\n\n\tUpon failure, prints a "
			 "message to stderr.\n";

	if (!args[0])
		write(STDOUT_FILENO, all, _strlen(all));
	else if (_strcmp(args[0], "alias") == 0)
		write(STDOUT_FILENO, alias, _strlen(alias));
	else if (_strcmp(args[0], "cd") == 0)
		write(STDOUT_FILENO, cd, _strlen(cd));
	else if (_strcmp(args[0], "exit") == 0)
		write(STDOUT_FILENO, exit, _strlen(exit));
	else if (_strcmp(args[0], "env") == 0)
		write(STDOUT_FILENO, env, _strlen(env));
	else if (_strcmp(args[0], "setenv") == 0)
		write(STDOUT_FILENO, setenv, _strlen(setenv));
	else if (_strcmp(args[0], "unsetenv") == 0)
		write(STDOUT_FILENO, unsetenv, _strlen(unsetenv));
	else
		write(STDERR_FILENO, name, _strlen(name));

	return (0);
}
