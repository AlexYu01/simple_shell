#ifndef _SHELL_H_
#define _SHELL_H_

/*
 * File: shell.h
 * Auth: Alex Yu
 *       Brennan D Baraban
 */

#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

/**
 * struct list_s - A new struct type defining a linked list.
 * @dir: A directory path.
 * @next: A pointer to another struct list_s.
 */
typedef struct list_s
{
	char *dir;
	struct list_s *next;
} list_t;

/**
 * struct builtin_s - A structure type defining all builtin commands.
 * @name: The name of the builtin command.
 * @f: A function pointer to the command's function.
 */
typedef struct builtin_s
{
	char *name;
	int (*f)(char **argv);
} builtin_t;

/* Helper Functions */
char **_strtok(char *line, char *delim);
char *get_location(char *command);
list_t *get_path_dir(char *path);
list_t *add_node_end(list_t **head, char *dir);
void free_list(list_t *head);
int num_len(int num);
char *_itoa(int num);
int create_error(char *name, int hist, char **argv, int err);

/* Builtins */
int (*get_builtin(char *command))(char **argv);
int shellby_exit(char **argv);
char *_getenv(const char *name);
int _setenv(const char *name, const char *value, int overwrite);
int _unsetenv(const char *name);

/* Error handling */
int num_len(int num);
char *_itoa(int num);
int create_error(char *name, int hist, char **argv, int err);
char *error_2(char *name, int hist, char **argv);
char *error_126(char *name, int hist, char **argv);
char *error_127(char *name, int hist, char **argv);

#endif /* _SHELL_H_ */
