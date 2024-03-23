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
Command Execution: Users can execute commands provided by the operating system or built-in commands provided by the shell itself.
Directory Manipulation: Built-in commands like cd, removedirectory, and makedirectory allow users to navigate through directories and create or remove directories.
File Operations: Built-in commands like touch enable users to create files.
Output Redirection: The echo command supports basic output redirection.
Text File Analysis: The wc command provides functionality to count lines, words, and characters in a text file.
Help Functionality: Users can access help documentation for built-in commands using the help command.
Exit Mechanism: The exit command allows users to gracefully exit the shell.
Usage :
Clone the repository to your local machine.
Compile the source code using a C compiler.
Run the executable file to start the shell.
Enter commands at the prompt and press Enter to execute them
