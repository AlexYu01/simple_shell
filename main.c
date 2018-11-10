/*
 * File: main.c
 * Auth: Alex Yu
 *       Brennan D Baraban
 */

#include "shell.h"
#include <errno.h>

void sig_handler(int sig);
int execute(char **argv, char *name, int hist);
char **get_args(char **argv);
int run_args(char **argv, char *name, int *hist);

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
 * @argv: An array of arguments.
 * @name: The name of the call.
 * @hist: The history number of the call.
 *
 * Return: If an error occurs - a corresponding error code.
 *         O/w - The exit value of the last executed command.
 */
int execute(char **argv, char *name, int hist)
{
	pid_t child_pid;
	int status, flag = 0, ret;
	char *command = argv[0];

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
				_exit(create_error(name, hist, argv, 126));
			else
				_exit(create_error(name, hist, argv, 127));
		}
		/*
		if (access(command, X_OK) == -1)
			return (create_error(name, hist, argv[0], 126));
		*/
		execve(command, argv, NULL);
		if (errno == EACCES)
			_exit(create_error(name, hist, argv, 126));
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
 * get_args - Reads and tokenizes arguments from the command line.
 * @argv: An array to store the tokenized arguments.
 *
 * Return: If an error occurs - NULL.
 *         O/w - the tokenized array of arguments.
 */
char **get_args(char **argv)
{
	size_t n = 0;
	ssize_t read;
	char *line = NULL;

	read = getline(&line, &n, stdin);
	if (read == -1 || read == 1)
	{
		if (read == -1)
			perror("Failed to tokenize\n");
		free(line);
		return (NULL);
	}

	argv = _strtok(line, " ");

	free(line);
	return (argv);
}

/**
 * run_args - Calls the execution of a command.
 * @argv: An array of arguments.
 * @name: The name of the call.
 * @hist: The history number of the call.
 *
 * Return: If an error occurs - -1.
 *         O/w - the exit value of the last executed command.
 */
int run_args(char **argv, char *name, int *hist)
{
	int ret, index;
	int (*builtin)(char **argv);

	argv = get_args(argv);
	if (!argv)
		return (-1);

	builtin = get_builtin(argv[0]);
	if (builtin)
	{
		ret = builtin(argv);
		if (ret)
			create_error(name, *hist, argv, ret);
	}
	else
		ret = execute(argv, name, *hist);

	(*hist)++;

	for (index = 0; argv[index]; index++)
		free(argv[index]);
	free(argv);
	argv = NULL;

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
	int ret, hist = 1;
	char *name = argv[0];

	signal(SIGINT, sig_handler);

	if (argc != 1)
		return (execute(argv + 1, name, hist));

	if (!isatty(STDIN_FILENO))
	{
		while (ret != -1)
			ret = run_args(argv, name, &hist);
		return (0);
	}

	while (1)
	{
		printf("$ ");
		ret = run_args(argv, name, &hist);
	}

	return (ret);
}
