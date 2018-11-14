/*
 * File: main.c
 * Auth: Alex Yu
 *       Brennan D Baraban
 */

#include "shell.h"
#include <errno.h>

void sig_handler(int sig);
int execute(char **args, char *name, int hist);
int handle_args(char *name, int *hist);

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
	int status, flag = 0, ret;
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
				_exit(create_error(name, hist, args, 126));
			else
				_exit(create_error(name, hist, args, 127));
		}
		/*
		if (access(command, X_OK) == -1)
			return (create_error(name, hist, argv[0], 126));
		*/
		execve(command, args, NULL);
		if (errno == EACCES)
			_exit(create_error(name, hist, args, 126));
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
 *
 * Return: If an end-of-file is read - -2.
 *         If the input cannot be tokenized - -1.
 *         O/w - The value of the last executed command.
 */
int handle_args(char *name, int *hist)
{
	int ret;
	size_t index = 0;
	ssize_t read;
	char **args, *line = NULL;
	int (*builtin)(char **argv);

	read = getline(&line, &index, stdin);
	if (read == -1)
	{
		free(line);
		return (-2);
	}
	if (read == 1)
	{
		free(line);
		printf("$ ");
		return (handle_args(name, hist));
	}

	args = _strtok(line, " ");
	free(line);
	if (!args)
	{
		perror("Failed to tokenize");
		return (-1);
	}
	builtin = get_builtin(args[0]);
	if (builtin)
	{
		ret = builtin(args);
		if (ret)
			create_error(name, *hist, args, ret);
	}
	else
		ret = execute(args, name, *hist);

	(*hist)++;

	for (index = 0; args[index]; index++)
		free(args[index]);
	free(args);

	return (ret);
}

/**
 * main - Runs a simple UNIX command interpreter.
 * @argc: The number of arguments supplied to the program.
 * @argv: An array of pointers to the arguments.
 *
 * Return: Always 0.
 */
int main(int argc, char *argv[])
{
	int ret = 0, hist = 1;
	char *name = argv[0];

	signal(SIGINT, sig_handler);

	if (argc != 1)
		return (execute(argv + 1, name, hist));

	if (!isatty(STDIN_FILENO))
	{
		while (ret == 0)
		{
			ret = handle_args(name, &hist);
			if (ret == -2)
				return (0);
		}
		return (ret);
	}

	while (1)
	{
		printf("$ ");
		ret = handle_args(name, &hist);
		if (ret == -2)
		{
			printf("\n");
			exit(0);
		}
	}

	return (ret);
}
