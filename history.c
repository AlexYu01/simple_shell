/*
 * File: history.c
 * Auth: Alex Yu
 *       Brennan D Baraban
 */

#include "shell.h"

history_t *add_history_end(char *command);
void free_history_list(void);
char *get_history_file(void);
void init_history(void);
void save_history(void);

/**
 * add_history_end - Adds a new node to the end of a history_t linked list.
 * @command: The command to add to the new node.
 *
 * Return: A pointer to the new node.
 */
history_t *add_history_end(char *command)
{
	history_t *new_node = malloc(sizeof(history_t));
	history_t *last;

	if (!new_node)
		return (NULL);

	new_node->command = malloc(sizeof(char) * (_strlen(command) + 1));
	if (!new_node->command)
		return (NULL);
	_strcpy(new_node->command, command);
	new_node->count = 0;
	new_node->next = NULL;

	if (history != NULL)
	{
		last = history;
		while (last->next != NULL)
			last = last->next;
		new_node->count = last->count + 1;
		last->next = new_node;
	}
	else
		history = new_node;

	return (new_node);
}

/**
 * free_history_list - Frees a history_t linked list.
 */
void free_history_list(void)
{
	history_t *next;

	while (history)
	{
		next = history->next;
		free(history->command);
		free(history);
		history = next;
	}
}

/**
 * get_history_file - Gets the full pathname of the history
 *                    file .simple_shell_history.
 *
 * Return: If the HOME variable does not exis - NULL.
 *         A string containing the full pathname.
 */
char *get_history_file(void)
{
	char *home = NULL, *file;

	home = *_getenv("HOME");
	if (!home)
		return (NULL);

	home += 5;

	file = malloc(sizeof(char) * (_strlen(home) + 23));
	if (!file)
		return (NULL);

	_strcpy(file, home);
	_strcat(file, "/.simple_shell_history");

	return (file);
}

/**
 * init_history - Initializes a history_t linked list.
 *
 * Description: If there exists a saved history file .simple_shell_history,
 *              the linked list is initialized with the commands stored in
 *              the file.
 */
void init_history(void)
{
	char *file, *cmds = NULL, buffer[1024];
	int o, r, size = 0, index1, index2;

	history = NULL;

	file = get_history_file();
	if (!file)
		return;

	o = open(file, O_RDONLY);
	if (o != -1)
	{
		do {
			r = read(o, buffer, 1023);
			if (r == -1)
			{
				free(cmds);
				free(file);
				return;
			}
			if (r > 0)
			{
				cmds = _realloc(cmds, size, size + 1024);
				if (!cmds)
				{
					close(o);
					free(file);
					return;
				}
				cmds[size] = '\0';
				size += 1024;
				buffer[r] = '\0';
				_strcat(cmds, buffer);
			}
		} while (r != 0);

		if (cmds == NULL)
		{
			close(o);
			free(file);
			return;
		}

		for (index1 = 0, index2 = 0; cmds[index1]; index1++)
		{
			if (cmds[index1] == '\n' || cmds[index1] == '\0')
			{
				cmds[index1] = '\0';
				add_history_end(&cmds[index2]);
				index2 = index1 + 1;
			}
		}
		free(cmds);
		close(o);
	}

	free(file);
}

/**
 * save_history - Saves the contents of a history_t linked list
 *                to a hidden file .simple_shell_history.
 */
void save_history(void)
{
	history_t *tmp = history;
	unsigned int count = 0, shift;
	char *file, *new_line = "\n";
	int fd;

	file = get_history_file();
	if (!file)
	{
		free_history_list();
		return;
	}
	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		free_history_list();
		free(file);
		return;
	}

	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}

	tmp = history;
	if (count >= 4096)
	{
		shift = (count % 4096) + 1;
		while (shift)
		{
			tmp = tmp->next;
			shift--;
		}
	}
	while (tmp)
	{
		write(fd, tmp->command, _strlen(tmp->command));
		write(fd, new_line, 1);
		tmp = tmp->next;
	}

	free(file);
	close(fd);
	free_history_list();
}
