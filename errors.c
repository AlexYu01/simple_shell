#include "shell.h"

int num_len(int num)
{
	int len = 1;

	while (num > 9)
	{
		len++;
		num /= 10;
	}

	return (len);
}

char *_itoa(int num)
{
	char *buffer;
	int len = num_len(num);

	buffer = malloc(sizeof(char) * (len + 1));
	if (!buffer)
		return (NULL);

	buffer[len] = '\0';

	len--;
	while (len >= 0)
	{
		buffer[len] = (num % 10) + '0';
		num /= 10;
		len--;
	}

	return (buffer);
}

int create_error(char *name, int hist, char *command, int err)
{
	char *error, *hist_str;
	int len;

	hist_str = _itoa(hist);
	if (!hist_str)
		/* TODO return a better error number for number conversion fail */
		return (err);

	len = strlen(name) + strlen(hist_str) + strlen(command) + 6;
	if (err == 127)
		len += 10;
	else
		len += 18;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
		/* TODO return a better error number for malloc failure */
		return (err);

	strcpy(error, name);
	strcat(error, ": ");
	strcat(error, hist_str);
	strcat(error, ": ");
	strcat(error, command);
	if (err == 127)
		strcat(error, ": not found\n");
	else
		strcat(error, ": Permission denied\n");

	write(STDERR_FILENO, error, len);

	return (err);
}
