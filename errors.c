#include "shell.h"

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
int create_error(char *name, int hist, char **argv, int err)
{
	char *error;

	switch (err)
	{
		case 2:
			error = error_2(name, hist, argv);
			break;
		case 126:
			error = error_126(name, hist, argv);
			break;
		case 127:
			error = error_127(name, hist, argv);
			break;
	}
	write(STDERR_FILENO, error, strlen(error));

	if (error)
		free(error);
	return (err);

}
