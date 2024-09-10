This repository contains a basic implementation of a shell in C programming language. The shell provides a command-line interface for users to interact with their operating system.

Commands used:
1. `cd`: Change directory - change the current working directory.
2. `help`: Display help information - provide assistance on how to use the shell and its built-in commands.
3. `exit`: Exit the shell - terminate the shell session and return to the parent process.
4. `removedirectory`: Remove directory - delete a directory from the filesystem.
5. `makedirectory`: Make directory - create a new directory in the filesystem.
6. `echo`: Echo - display a line of text.
7. `pwd`: Print working directory - display the current working directory.
8. `wc`: Word count - count the number of lines, words, and characters in a text file.
9. `touch`: Touch - create an empty file or update the access and modification times of an existing file.

Features :

The simple shell implementation provides basic command-line functionality similar to traditional Unix shells. It supports essential features such as changing directories (cd), displaying help information (help), exiting the shell (exit), creating and removing directories (makedirectory and removedirectory), printing the current working directory (pwd), counting lines, words, and characters in a text file (wc), and creating or updating file timestamps (touch). Additionally, the shell incorporates error handling for various file and directory operations, including directory creation, removal, and file opening. It utilizes fork-exec mechanism for executing commands, allowing for the execution of both built-in shell commands. Overall, the simple shell offers a lightweight and functional command-line interface suitable for basic system navigation and file management tasks.

Usage :

Clone the repository to your local machine.
Compile the source code using a C compiler.
Run the executable file to start the shell.
Enter commands at the prompt and press Enter to execute them
