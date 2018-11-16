/*
 * File: main.c
 * Auth: Alex Yu
 *       Brennan D Baraban
 */

#include "shell.h"
#include <errno.h>

void sig_handler(int sig);
int execute(char **args, char *name, int hist);
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
int execute(char **args, char *name, int hist)
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
			free_args(args);
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
		free_args(args);
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
 * handle_args - Gets and calls the execution of a command.
 * @name: The name of the call.
 * @hist: The history number of the call.
 * @exe_ret: The return value of the last executed command.
 *
 * Return: If an end-of-file is read - -2.
 *         If the input cannot be tokenized - -1.
 *         O/w - The value of the last executed command.
 */
int handle_args(char *name, int *hist, int *exe_ret)
{
	int ret;
	size_t index = 0;
	ssize_t read;
	char **args, *line = NULL;
	int (*builtin)(char **argv);

	read = _getline(&line, &index, STDIN_FILENO);
	if (read == -1)
	{
		free(line);
		return (-2);
	}
	if (read == 1)
	{
		if (isatty(STDIN_FILENO))
			printf("$ ");
		free(line);
		return (handle_args(name, hist, exe_ret));
	}

	/* replace \n with \0 */
	line[read - 1] = '\0';
	variable_replacement(&line, exe_ret);
	args = _strtok(line, " ");
	free(line);
	if (!args)
		return (0);
	builtin = get_builtin(args[0]);
	if (builtin)
	{
		ret = builtin(args + 1);
		if (ret != -3)
		{
			*exe_ret = ret;
			if (ret != 0)
				create_error(name, *hist, args, ret);
		}
	}
	else
	{

		*exe_ret = execute(args, name, *hist);
		ret = *exe_ret;
	}
	(*hist)++;

	free_args(args);

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
		ret = execute(argv + 1, name, hist);
		free_env();
		return (ret);
	}

	if (!isatty(STDIN_FILENO))
	{
		/* TODO probably dont have to change ret to *exe_ret */
		while (ret == 0)
		{
			ret = handle_args(name, &hist, exe_ret);
			if (ret == -2)
			{
				free_env();
				return (0);
			}
		}
		free_env();
		return (ret);
	}

	while (1)
	{
		printf("$ ");
		ret = handle_args(name, &hist, exe_ret);
		if (ret == -2 || ret == -3)
		{
			if (ret == -2)
				printf("\n");
			free_env();
			exit(*exe_ret);
		}
	}

	free_env();
	return (*exe_ret);
}
