/*
 * File: main.c
 * Auth: Alex Yu
 *       Brennan D Baraban
 */

#include "shell.h"
#include <errno.h>

void sig_handler(int sig);
int execute(char **args, char **front, char *name, int hist);
int handle_args(char *name, int *hist, int *exe_ret);

/**
 * sig_handler - Prints a new prompt upon a signal.
 * @sig: The signal.
 */
void sig_handler(int sig)
{
	char *new_prompt = "\n$ ";

	(void)sig;
	signal(SIGINT, sig_handler);
	write(STDIN_FILENO, new_prompt, 3);
}

/**
 * execute - Executes a command in a child process.
 * @args: An array of arguments.
 * @name: The name of the call.
 * @hist: The history number of the call.
 *
 * Return: If an error occurs - a corresponding error code.
 *         O/w - The exit value of the last executed command.
 */
int execute(char **args, char **front, char *name, int hist)
{
	pid_t child_pid;
	int status, flag = 0, ret = 0;
	char *command = args[0];

	if (command[0] != '/' && command[0] != '.')
	{
		flag = 1;
		command = get_location(command);
	}

	child_pid = fork();
	if (child_pid == -1)
	{
		if (flag)
			free(command);
		perror("Error child:");
		return (1);
	}
	if (child_pid == 0)
	{
		if (!command || (access(command, F_OK) == -1))
		{
			if (errno == EACCES)
				ret = (create_error(name, hist, args, 126));
			else
				ret = (create_error(name, hist, args, 127));
			free_env();
			free_args(args, front);
			_exit(ret);
		}
	/*
	*	if (access(command, X_OK) == -1)
	*		return (create_error(name, hist, argv[0], 126));
	*/
		execve(command, args, NULL);
		if (errno == EACCES)
			ret = (create_error(name, hist, args, 126));
		free_env();
		free_args(args, front);
		_exit(ret);
	}
	else
	{
		wait(&status);
		ret = WEXITSTATUS(status);
	}

	if (flag)
		free(command);
	return (ret);
}

/**
 * get_args - Gets a command from standard input.
 * @line: A buffer to store the command.
 * @exe_ret: The return value of the last executed command.
 *
 * Return: If an end-of-file is read - END_OF_FILE (-2).
 *         Otherwise - 0.
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
 * run_args - Calls the execution of a command.
 * @args: An array of arguments.
 * @name: The name of the call.
 * @hist: The history number of the call.
 * @exe_ret: The return value of the last executed command.
 *
 * Return: The exit value of the last executed command.
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
 * handle_args - Gets and calls the execution of a command.
 * @name: The name of the call.
 * @hist: The history number of the call.
 * @exe_ret: The return value of the last executed command.
 *
 * Return: If an end-of-file is read - -2.
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
			ret = run_args(args, front, name, hist, exe_ret);
			args = &args[++index];
			index = 0;
		}
		else if (_strncmp(args[index], "||", 2) == 0)
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
				free(front);
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
				{
					printf("args[index]: %s\n", args[index]);
					free(args[index]);
				}
				free(front);
				return (ret);
			}
		}
	}

	ret = run_args(args, front, name, hist, exe_ret);

	free(front);
	return (ret);
}

/**
 * main - Runs a simple UNIX command interpreter.
 * @argc: The number of arguments supplied to the program.
 * @argv: An array of pointers to the arguments.
 *
 * Return: The last executed command.
 */
int main(int argc, char *argv[])
{
	int ret = 0, hist = 1, retn;
	int *exe_ret = &retn;
	char *name = argv[0];

	signal(SIGINT, sig_handler);

	*exe_ret = 0;
	environ = _copyenv();
	if (!environ)
		exit(-100);

	if (argc != 1)
	{
		ret = execute(argv + 1, argv + 1, name, hist);
		free_env();
		return (ret);
	}

	if (!isatty(STDIN_FILENO))
	{
		while (ret != END_OF_FILE && ret != EXIT)
			ret = handle_args(name, &hist, exe_ret);
		free_env();
		return (*exe_ret);
	}

	while (1)
	{
		printf("$ ");
		ret = handle_args(name, &hist, exe_ret);
		if (ret == END_OF_FILE || ret == EXIT)
		{
			if (ret == END_OF_FILE)
				printf("\n");
			free_env();
			exit(*exe_ret);
		}
	}

	free_env();
	return (*exe_ret);
}
