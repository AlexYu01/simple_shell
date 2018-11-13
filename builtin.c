#include "shell.h"

int (*get_builtin(char *command))(char **argv)
{
	builtin_t funcs[] = {
		{ "exit", shellby_exit },

	/*	{ "env", shellby_env },
		{ "setenv", shellby_setenv },
		{ "unsetenv", shellby_unsetenv },
		{ "cd", shellby_cd },
		{ "alias", shelly_alias },
	*/	{ NULL, NULL }
	};
	int i;
	for (i = 0; funcs[i].name; i++)
	{
		if (strcmp(funcs[i].name, command) == 0)
			break;
	}
	return (funcs[i].f);
}

int shellby_exit(char **argv)
{
	int i = 0, sign = 1;
	unsigned int num = 0;

	if (argv[1])
	{
		if (argv[1][i] == '-')
			sign = -1;
		for (; argv[1][i]; i++)
		{
			if (argv[1][i] == '-')
				sign *= -1;

			if (argv[1][i] >= '0' && argv[1][i] <= '9')
				num = (num * 10) + (argv[1][i] - '0');
			else
				return (2);
		}
	}
	exit(num * sign);
}
