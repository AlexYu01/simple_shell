#include "shell.h"

char *error_2(char *name, int hist, char **argv)
{
	char *error, *hist_str;
	int len;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	len = strlen(name) + 2 + strlen(hist_str) + 2 + strlen(argv[0]) + 2 +
		14 + 2 + strlen(argv[1]) + 1;
	error = malloc(sizeof(char) * (len + 1));

	if (!error)
		return (NULL);

	strcpy(error, name);
	strcat(error, ": ");
	strcat(error, hist_str);
	strcat(error, ": ");
	strcat(error, argv[0]);
	strcat(error, ": ");
	strcat(error, "Illegal Number");
	strcat(error, ": ");
	strcat(error, argv[1]);
	strcat(error, "\n");

	return (error);
}

char *error_126(char *name, int hist, char **argv)
{
	char *error, *hist_str;
	int len;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	len = strlen(name) + 2 + strlen(hist_str) + 2 + strlen(argv[0]) + 2 +
		18;
	error = malloc(sizeof(char) * (len + 1));

	if (!error)
		return (NULL);

	strcpy(error, name);
	strcat(error, ": ");
	strcat(error, hist_str);
	strcat(error, ": ");
	strcat(error, argv[0]);
	strcat(error, ": ");
	strcat(error, "Permission denied\n");

	return (error);
}

char *error_127(char *name, int hist, char **argv)
{
	char *error, *hist_str;
	int len;

	hist_str = _itoa(hist);
	if (!hist_str)
		return (NULL);

	len = strlen(name) + 2 + strlen(hist_str) + 2 + strlen(argv[0]) + 2 +
		10;
	error = malloc(sizeof(char) * (len + 1));

	if (!error)
		return (NULL);

	strcpy(error, name);
	strcat(error, ": ");
	strcat(error, hist_str);
	strcat(error, ": ");
	strcat(error, argv[0]);
	strcat(error, ": ");
	strcat(error, "not found\n");

	return (error);
}
