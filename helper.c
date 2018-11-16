#include "shell.h"

char *get_pid(void);
char *get_env_value(char *var);

/**
 * free_args - Free up memory taken by args.
 *
 * @args: The double array containing command/arguments terminated with NULL.
 */
void free_args(char **args)
{
	size_t i;

	for (i = 0; args[i]; i++)
		free(args[i]);
	free(args);
}
/**
 * get_pid - Gets the current process ID. Starts by opening up the stat file
 * for the current process. Various information about the process is stored in
 * the file and is delimited by a space. The PID is the first word in the file.
 * Read the file into a buffer and replace the space at the end of the pid with
 * \0.
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
 * Environment variables are stored as such VARIABLE=VALUE.
 *
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
 * @args: The double pointer to the line read.
 * @exe_ret: The pointer to the return value of the last executed command.
 */
void variable_replacement(char **line, int *exe_ret)
{
	int j, k = 0, len;
	char *replacement = NULL, *old_line = NULL, *var = NULL, *new_line;

	old_line = *line;
	for (j = 0; old_line[j]; j++)
	{
		if (old_line[j] == '$' && old_line[j + 1])
		{
			if (old_line[j + 1] == '$')
			{
				replacement = get_pid();
				k = j + 2;
			}
			else if (old_line[j + 1] == '?')
			{
				replacement = _itoa(*exe_ret);
				k = j + 2;
			}
			else if (old_line[j + 1])
			{
				/* extract the variable name to search for */
				for (k = j + 1; old_line[k] &&
						old_line[k] != '$' &&
						old_line[k] != ' '; k++)
					;
				len = k - (j + 1);
				var = malloc(len + 1);
				if (!var)
					return;
				var[0] = '\0';
				_strncat(var, &old_line[j + 1], len);
				replacement = get_env_value(var);
				free(var);
			}
			new_line = malloc(j + _strlen(replacement) + _strlen(&old_line[k]) + 1);
			if (!line)
				return;
			new_line[0] = '\0';
			_strncat(new_line, old_line, j);
			if (replacement)
			{
				_strcat(new_line, replacement);
				free(replacement);
				replacement = NULL;
			}
			_strcat(new_line, &old_line[k]);
			free(old_line);
			*args = new_line;
			old_line = new_line;
			j = -1;
		}
	}
}
