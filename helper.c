#include "shell.h"

char *get_pid(void);
char *get_env_value(char *var);

/**
 * get_pid - Gets the current process ID.
 *
 * Return: The current process ID or NULL on failure.
 */
char *get_pid(void)
{
	size_t i = 0;
	char *buffer;
	ssize_t file;

	file = open("/proc/self/stat", O_RDONLY);
	if (file == -1)
	{
		perror("Cant read file");
		return (NULL);
	}
	buffer = malloc(120);
	if (!buffer)
	{
		close(file);
		return (NULL);
	}
	read(file, buffer, 120);
	while (buffer[i] != ' ')
		i++;
	buffer[i] = '\0';

	close(file);
	return (buffer);
}

/**
 * get_env_value - Gets the value corresponding to the environment variable.
 * @var: The environment variable to search for.
 *
 * Return: The value of the environment variable or an empty string if the
 * variable was not found..
 */
char *get_env_value(char *var)
{
	char **var_addr;
	char *replacement, *temp;

	var_addr = _getenv(var);

	if (var_addr)
	{
		temp = *var_addr;
		while (*temp != '=')
			temp++;
		temp++;
		replacement = malloc(_strlen(temp) + 1);
		if (!replacement)
			return (NULL);
		_strcpy(replacement, temp);
	}
	else
	{
		replacement = NULL;
	}

	return (replacement);
}

/**
 * variable_replacement - Replaces $$ with current PID, $? with the return
 * /exit value of the last program exeucted. Environment variables names
 * preceded by the $ will be replaced with their corresponding value.
 *
 * @args: The double pointer containing the command and arguments.
 * @exe_ret: The pointer to the return value of the last executed command.
 */
void variable_replacement(char **args, int *exe_ret)
{
	int i, j, k = 0, len;
	char *replacement = NULL, *var = NULL, *line;

	for (i = 0; args[i]; i++)
	{
		for (j = 0; args[i][j]; j++)
		{
			if (args[i][j] == '$' && args[i][j + 1])
			{
				if (args[i][j + 1] == '$')
				{
					/* has malloc memory */
					replacement = get_pid();
					k = j + 2;
				}
				else if (args[i][j + 1] == '?')
				{
					/* TODO replace ret with real */
					replacement = _itoa(*exe_ret);
					k = j + 2;
				}
				else if (args[i][j + 1])
				{
					for (k = j + 1; args[i][k] && args[i][k] != '$'; k++)
						;
					len = k - (j + 1);
					var = malloc(len + 1);
					if (!var)
						return;
					var[0] = '\0';
					_strncat(var, &args[i][j + 1], len);
					replacement = get_env_value(var);
					free(var);
				}
				line = malloc(j + _strlen(replacement) + _strlen(&args[i][k]) + 1);
				if (!line)
					return;
				line[0] = '\0';
				_strncat(line, args[i], j);
				if (replacement)
				{
					_strcat(line, replacement);
					free(replacement);
					replacement = NULL;
				}
				_strcat(line, &args[i][k]);
				free(args[i]);
				args[i] = line;
				j = -1;
			}
		}
	}
}
