/*
 * File: main.c
 * Auth: Alex Yu
 *       Brennan D Baraban
 */

#include "shell.h"

/**
 * main - Runs a simple UNIX command interpreter.
 *
 * Return: Always 0.
 */
int main(void)
{
	pid_t child_pid;
	int status;
	char **argv;
	size_t n, index;
	ssize_t read;
	char *line;
	int run = 2;

	line = NULL;
	while (run)
	{
		printf("$ ");
		n = 0;
		read = getline(&line, &n, stdin);
		if (read == -1)
		{
			printf("read failed\n");
			return (1);
		}
		argv = _strtok(line, " ");
		if (!argv)
		{
			perror("Failed to tokenize\n");
			continue;
		}
		if (argv[0][0] != '/')
			/* TODO handle NULL case? */
			argv[0] = get_location(argv[0]);
		child_pid = fork();
		if (child_pid == -1)
		{
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
		for (index = 0; argv[index]; index++)
			free(argv[index]);
		free(argv);
		free(line);
		run--;
	}
	return (0);
}
