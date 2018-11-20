#include "shell.h"

/**
 * proc_file_commands - Takes a file and attempts to run the commands stored
 * within.
 * @file_path: Path to the file.
 *
 * Return: If file couldn't be opened - 127.
 *	   If malloc fails - -1.
 *	   Otherwise the return value of the last command ran.
 */
int proc_file_commands(char *file_path)
{
	ssize_t file;
	ssize_t b_read;
	unsigned int line_size = 0;
	unsigned int old_size = 120;
	char *line;
	char buffer[120];

	file = open(file_path, O_RDONLY);
	if (file == -1)
	{
		perror("Error ");
		return (127);
	}

	line = malloc(sizeof(char) * old_size);
	if (!line)
		return (-1);
	line[0] = '\0';

	do {
		b_read = read(file, buffer, 120);
		line_size += b_read;
		line = _realloc(line, old_size, line_size);
		_strcat(line, buffer);
		old_size = line_size;
	} while (b_read == 120);

	exit(0);
}
