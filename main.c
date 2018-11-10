/*
 * File: main.c
 * Auth: Alex Yu
 *       Brennan D Baraban
 */

#include "shell.h"

int execute(char **argv);
char **clear_input(char **argv);

int execute(char **argv)
{
	pid_t child_pid;
	int status, flag = 0;

	if (argv[0][0] != '/')
	{
		flag = 1;
		argv[0] = get_location(argv[0]);
	}

	child_pid = fork();
	if (child_pid == -1)
	{
		if (flag)
			free(argv[0]);
		perror("Error child:");
		return (1);
	}
	if (child_pid == 0)
	{
		if (execve(argv[0], argv, NULL) == -1)
			perror("Error exec gone wrong:");
	}
	else
	{
		wait(&status);
	}

	if (flag)
		free(argv[0]);
	return (0);
}

char **clear_input(char **argv)
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

/**
 * main - Runs a simple UNIX command interpreter.
 *
 * Return: Always 0.
 */
int main(int argc, char *argv[])
{
	int ret;
	size_t n, index;
	ssize_t read;
	char *line, *command;

	if (argc != 1)
		return (execute(argv + 1));

	if (!isatty(STDIN_FILENO))
	{
		argv = clear_input(argv);
		while (argv)
		{
			command = argv[0];
			ret = execute(argv);
			for (index = 1; argv[index]; index++)
				free(argv[index]);
			free(argv);
			free(command);
			argv = NULL;
			argv = clear_input(argv);
		}
		free(argv);
		return (0);
	}

	line = NULL;
	while (1)
	{
		printf("$ ");
		n = 0;
		read = getline(&line, &n, stdin);
		if (read == -1)
		{
			perror("read failed\n");
			return (1);
		}
		argv = _strtok(line, " ");
		if (!argv)
		{
			perror("Failed to tokenize\n");
			continue;
		}

		command = argv[0];
		ret = execute(argv);

		for (index = 1; argv[index]; index++)
			free(argv[index]);
		free(argv);
		free(line);
		free(command);
		return (0);
	}
	return (ret);
}
