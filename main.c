/*
 * File: main.c
 * Auth: Alex Yu
 *       Brennan D Baraban
 */

#include "shell.h"

int execute(char **argv, char *name, int hist);
char **get_args(char **argv);
int run_args(char **argv, char *name, int *hist);

int execute(char **argv, char *name, int hist)
{
	pid_t child_pid;
	int status, flag = 0, ret;
	char *command = argv[0];

	if (command[0] != '/')
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
		if (execve(command, argv, NULL) == -1)
		{
			create_error(name, hist, argv[0], 1);
			return (127);
		}
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

char **get_args(char **argv)
{
	size_t n = 0;
	ssize_t read;
	char *line = NULL;

	read = getline(&line, &n, stdin);
	if (read == -1)
	{
		free(line);
		return (NULL);
	}

	argv = _strtok(line, " ");

	free(line);
	return (argv);
}

int run_args(char **argv, char *name, int *hist)
{
	int ret, index;

	argv = get_args(argv);
	if (!argv)
		return (-1);

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
 *
 * Return: Always 0.
 */
int main(int argc, char *argv[])
{
	int ret, hist = 1;
	char *name = argv[0];

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
		if (ret == -1)
			perror("Failed to tokenize\n");
	}
	return (ret);
}
