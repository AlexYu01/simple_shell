# shellby - Simple Shell

A simple UNIX command interpreter written as part of the low-level programming and algorithm track at Holberton School.

## Description

**Shellby** is a simple UNIX command language interpreter that executes commands read from standard input.

### Invocation
**Shellby** can be invoked both interactively and non-interactively. If **shellby** is invoked with standard input not connected to a terminal, it reads and executes received commands in order.

If **shellby** is invoked with standard input connected to a terminal (determined by [isatty](https://linux.die.net/man/3/isatty)(3)), an *interactive* shell is opened. When executing interactively, **shellby** displays the prompt `$ ` when it is ready to read a command.

### Command Execution
After receiving a command, **shellby** tokenizes it into words using `" "` as a delimiter and takes the following actions:
* If the command name contains no slashes, the shell searches for it in the list of shell builtins. If there exists a builtin by that name, the builtin is invoked.
* If the commmand name contains no slashes and is not a builtin, **shellby** searches each element of the **PATH** environmental variable for a directory containing an executable file by that name.
* If the command contains slashes or either of the above searches is successful, the shell executes the named program with any remaining given arguments in a separate execution environment.

### Exit Status
**Shellby** returns the exit status of the last command executed, with zero indicating success and non-zero indicating failure.

If a command is not found, the return status is 127; if a command is found but is not executable, the return status is 126.

All builtins return zero on success and two on incorrect usage.

## Signals
While running in interactive mode, **shellby** ignores the keyboard input `Ctrl+c`. Alternatively, an input of end-of-file (`Ctrl+d`) will exit the program. 

### Shellby Builtin Commands
* **cd**
  * Usage: `cd [DIRECTORY]`
  * Changes the current directory of the process to `DIRECTORY`.
  * If no argument is given, the command is interpreted as `cd $HOME`.
  * If the argument `-` is given, the command is interpreted as `cd $OLDPWD`.
  * The environment variables `PWD` and `OLDPWD` are updated after a change of directory.

* **exit**
  * Usage: `exit [STATUS]`
  * Exits the shell.
  * The `STATUS` argument is the integer used to exit the shell.
  * If no argument is given, the command is interpreted as `exit 0`.

* **env**
  * Usage: `env`
  * Prints the current environment.

* **setenv**
  * Usage: `setenv [VARIABLE] [VALUE]`
  * Initializes a new environment variable, or modifies an existing one.
  * Upon failure, prints a message to `stderr`.

* **unsetenv**
  * Usage: `unsetenv [VARIABLE]`
  * Removes an environmental variable.
  * Upon failure, prints a message to `stderr`.

## Authors
* Alex Yu <[AlexYu01](https://github.com/AlexYu01)> - Holberton School student
* Brennan D Baraban <[bdbaraban](https://github.com/bdbaraban)> - Holberton School student

## Acknowledgements
**Shellby** emulates basic functionality of the **sh** shell. This README borrows form the Linux man page [sh(1)](https://linux.die.net/man/1/sh).

This project was written as part of the curriculum for Holberton School. Holberton School is a campus-based full-stack software engineering program that prepares students for careers in the tech industry using project-based peer learning. For more information, visit [this link](https://www.holbertonschool.com/).

<p align="center">
  <img src="http://www.holbertonschool.com/holberton-logo.png">
</p>
