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

void create_error(char *name, int hist, char *command, int err)
{
	char *error, *hist_str;
	int len;
	
	hist_str = _itoa(hist);
	if (!hist_str)
		return;

	len = strlen(name) + strlen(hist_str) + strlen(command) + 16;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
		return;

	strcpy(error, name);
	strcat(error, ": ");
	strcat(error, hist_str);
	strcat(error, ": ");
	strcat(error, command);
	strcat(error, ": not found\n");

	write(STDERR_FILENO, error, len);
	(void)err;
}
