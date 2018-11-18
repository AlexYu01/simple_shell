#ifndef _SHELL_H_
#define _SHELL_H_

/*
 * File: shell.h
 * Auth: Alex Yu
 *       Brennan D Baraban
 */

#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define END_OF_FILE -2
#define EXIT -3

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
 * struct builtin_s - A new struct type defining builtin commands.
 * @name: The name of the builtin command.
 * @f: A function pointer to the builtin command's function.
 */
typedef struct builtin_s
{
	char *name;
	int (*f)(char **argv, char **front);
} builtin_t;

/* Helper Functions */
ssize_t _getline(char **lineptr, size_t *n, FILE *stream);
char **_strtok(char *line, char *delim);
char *get_location(char *command);
list_t *get_path_dir(char *path);
int execute(char **args, char **front, char *name, int hist);
void free_list(list_t *head);
char *_itoa(int num);

/* Input Helpers */
void handle_line(char **line, ssize_t read);
void variable_replacement(char **args, int *exe_ret);
char *get_args(char *line, int *exe_ret);
int call_args(char **args, char **front, char *name, int *hist, int *exe_ret);
int run_args(char **args, char **front, char *name, int *hist, int *exe_ret);
int handle_args(char *name, int *hist, int *exe_ret);
void free_args(char **args, char **front);

/* String functions */
char *_strcat(char *dest, const char *src);
char *_strncat(char *dest, const char *src, size_t n);
int _strncmp(const char *s1, const char *s2, size_t n);
int _strlen(const char *s);
char *_strcpy(char *dest, const char *src);

/* Builtins */
int (*get_builtin(char *command))(char **args, char **front);
int shellby_exit(char **args, char __attribute__((__unused__)) **front);
int shellby_env(char **args, char __attribute__((__unused__)) **front);
int shellby_setenv(char **args, char __attribute__((__unused__)) **front);
int shellby_unsetenv(char **args, char __attribute__((__unused__)) **front);
int shellby_cd(char **args, char **front);

/* Builtin Helpers */
char **_copyenv(void);
void free_env(void);
char **_getenv(const char *name);

/* Error Handling */
int create_error(char *name, int hist, char **args, int err);
char *error_env(char *name, int hist, char **args);
char *error_2_exit(char *name, int hist, char **args);
char *error_2_cd(char *name, int hist, char **args);
char *error_126(char *name, int hist, char **args);
char *error_127(char *name, int hist, char **args);

#endif /* _SHELL_H_ */
