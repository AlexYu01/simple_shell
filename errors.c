#include "shell.h"

int num_len(int num);
char *_itoa(int num);
int create_error(char *name, int hist, char *command, int err);

/**
 * num_len - Counts the digit length of a number.
 * @num: The number to measure.
 *
 * Return: The digit length.
 */
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

/**
 * _itoa - Converts an integer to a string.
 * @num: The integer.
 *
 * Return: The converted string.
 */
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


/**
 * create_error - Writes a custom error message to stderr.
 * @name: The name of the call causing the error.
 * @hist: The history number of the call.
 * @command: The command causing the error.
 * @err: The error value.
 *
 * Return: The error value.
 */
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
