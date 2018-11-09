#include "shell.h"
int main(void)
{
	pid_t child_pid;
	int status;
	char **argv;
	size_t n, index;
	ssize_t read;
	char *line;

	line = NULL;
	while (1)
	{
		printf("$ ");
		n = 0;
		if ((read = getline(&line, &n, stdin)) == -1)
		{
			printf("read failed\n");
			return (1);
		}
		argv = _strtok(line, " ");
		if (argv[0][0] != '/')
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
		return (0);
	}
	return (0);
}
