#ifndef _SHELL_H_
#define _SHELL_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct node {
	struct node *next;
	char *dir;
} node_t;

char **_strtok(char *line, char *delim);
char *_getenv(const char *name);
int _setenv(const char *name, const char *value, int overwrite);
int _unsetenv(const char *name);
node_t *get_path_dir(char *path);
char *get_location(char *command);

node_t *add_node_end(node_t **head, char *dir);
void free_list(node_t *head);



#endif
