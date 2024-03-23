#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<fcntl.h>
#include<utime.h>
#include<time.h>

/*
  Function Declarations for builtin shell commands:
 */
int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);
int shell_rmdir(char **args);
int shell_mkdir(char **args);
int shell_echo(char **args);
int shell_pwd();
int shell_wc(char **args);
int shell_touch(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {   //strings representing the names of the built-in shell commands: cd, help, and exit
"cd",
  "help",
  "exit",
  "removedirectory",
  "makedirectory",
  "echo",
  "pwd",
  "wc",
  "touch",
  
};

int (*builtin_func[]) (char **) = {//holds function pointers to the corresponding built-in functions declared earlier
  &shell_cd,
  &shell_help,
  &shell_exit,
  &shell_rmdir,
  &shell_mkdir,
  &shell_echo,
  &shell_pwd,
  &shell_wc,
  &shell_touch
};

int shell_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
Builtin function implementations.
*/
int shell_cd(char **args)
{
  if (args[1] == NULL) //checks if an argument is provided and prints an error message if not
  {
    fprintf(stderr, "shell: expected argument to \"cd\"\n");
  }
  else
  {
    if (chdir(args[1]) != 0) {//chdir() function changes the current working directory of the calling process to the directory specified by path
      perror("error: unsuccessful change in directory");
      return 1;
    }
  }
  return 0;
}

int shell_help(char **args)
{
  int i;
  printf("Implementation of simple shell\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < shell_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }
return 1;
}

int shell_exit(char **args)
{
  printf("Exiting from the shell\n");
  exit(EXIT_SUCCESS);
}

int shell_rmdir(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "Usage: rmdir <directory_name>\n");
        return 1; // Return non-zero on failure
    }

    const char *directory = args[1];

    if (rmdir(directory) == -1) {
        perror("rmdir failed");
        return 1; // Return non-zero on failure
    }

    printf("Directory '%s' removed successfully.\n", directory);
    return 0; // Return zero on success
}

int shell_mkdir(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "Usage: mkdir <directory_name>\n");
return 1; // Return non-zero on failure
    }

    const char *directory = args[1];

    if (mkdir(directory, 0777) == -1) {
        perror("mkdir failed");
        return 1; // Return non-zero on failure
    }

    printf("Directory '%s' created successfully.\n", directory);
    return 0; // Return zero on success
}

int shell_echo(char **args) {
    int interpret_escapes = 0; // Flag to indicate whether to interpret escape sequences
    char **token = &args[1];   // Skip the first argument, as it is "echo"
    char *delim = " \n\t\r";
    
    // Check if the first argument is "-e" to enable interpreting escape sequences
    if (*token != NULL && strcmp(*token, "-e") == 0) {
        interpret_escapes = 1;
        token++; // Move to the next argument
    }

    // Iterate over arguments and print each one
    while (*token != NULL) {
        // Check if escape sequences should be interpreted
        if (interpret_escapes) {
// Interpret escape sequences in the string
            char *escaped_string = *token;
            while (*escaped_string) {
                if (*escaped_string == '\\' && *(escaped_string + 1) != '\0') {
                    switch (*(escaped_string + 1)) {
                        case 'n':
                            putchar('\n'); // Print newline
                            break;
                        case 't':
                            putchar('\t'); // Print tab
                            break;
                        // Add more cases for other escape sequences as needed
                        default:
                            putchar(*(escaped_string + 1)); // Print the character following '\'
                            break;
                    }
                    escaped_string += 2; // Move to the next character after the escape sequence
                } else {
                    putchar(*escaped_string); // Print the current character
                    escaped_string++; // Move to the next character
                }
            }
        } else {
            printf("%s ", *token); // Print the string without interpreting escape sequences
        }
        token++; // Move to the next argument
    }
    printf("\n");
    return 0; // Return zero to indicate success
}

int shell_pwd() {
    DIR *directory;
    int root_inode;
    int inode;
    struct stat statbuf;
    struct dirent *fileEntry;

    if (lstat(".", &statbuf)) {
        perror("lstat");
        return 1;//return non-zero to indicate failure
    } else {
        // Get the inode of the current directory
        inode = statbuf.st_ino;
        // Print message and exit if current directory is the root directory
        if (inode == 2) {
            printf("/\n");
            return 0;//return 0 to indicate success
        }

        root_inode = inode;
        // Traverse through the parent directories until inode is equal to 2
        while (inode != 2) {
            if (!chdir("..")) {
                if (!(directory = opendir("."))) {
                    perror("opendir");
return 1;//return non-zero to indicate failure
                }
                // Traverse through the contents of the current directory
                while ((fileEntry = readdir(directory))) {
                    inode = fileEntry->d_ino;
                    // Print out directory name if inodes match
                    if (inode == root_inode)
                        printf("%s/", fileEntry->d_name);
                    // Found parent directory
                    else if (inode == 2) {
                        printf("/\n");
                        return 0;
                    }
                }
                if (lstat(".", &statbuf)) {
                    perror("lstat");
                    return 1;
                } else
                    // Next time you want to compare it with the inode of the current directory
                    root_inode = statbuf.st_ino;
                closedir(directory);
            } else {
                perror("chdir");
                return 1;
            }
        }
    }
  return 0;
}
int shell_wc(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "Usage: wc <file_name>\n");
        return 1; // Return non-zero on failure
    }

    const char *file_name = args[1];
    FILE *file = fopen(file_name, "r");
    if (!file) {
        perror("fopen");
        return 1; // Return non-zero on failure
    }

    int lines = 0, words = 0, characters = 0;
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file)) {
        lines++;
        characters += strlen(buffer);

        // Tokenize the line to count words
        char *token = strtok(buffer, " \t\n\r\v\f");
        while (token != NULL) {
            words++;
            token = strtok(NULL, " \t\n\r\v\f");
        }
    }
fclose(file);

    // Exclude newline characters from character count
    characters -= lines;

    printf("Lines: %d\n", lines);
    printf("Words: %d\n", words);
    printf("Characters: %d\n", characters);

    return 0; // Return zero on success
}

int shell_touch(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "Usage: touch <file_name>\n");
        return 1; // Return non-zero on failure
    }

    const char *file_name = args[1];
    struct stat st;
    int file_exists = stat(file_name, &st) == 0;

    if (!file_exists) {
        // If file does not exist, create it with open()
        int fd = open(file_name, O_CREAT | O_WRONLY, 0644);
        if (fd == -1) {
            perror("open");
            return 1; // Return non-zero on failure
        }
close(fd);
    } else {
        // If file exists, update its access and modification times
        struct utimbuf times;
        times.actime = st.st_atime;
        times.modtime = time(NULL);
        if (utime(file_name, &times) == -1) {
            perror("utime");
            return 1; // Return non-zero on failure
        }
    }

    return 0; // Return zero on success
}

int shell_launch(char **args)
{
  pid_t pid;//pid_t is a data type defined in the header <sys/types.h>
  int status;//status will be used to store the exit status of the child process.

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {//execvp() attempts to execute the program specified by args[0] with the arguments stored in the args array
      perror("shell: execution error");
    }
    exit(EXIT_FAILURE);//exit(EXIT_FAILURE) statement is reached only if execvp() fails, If execvp() is successful, it never returns -1. Instead, it dire>
  } else if (pid < 0) {
    // Error forking
perror("shell:forking error");
  } else {
    // Parent process
    do {
      waitpid(pid, &status, WUNTRACED);//WUNTRACED option specifies that the parent should also return if a child has stopped due to a signal
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int shell_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < shell_num_builtins(); i++) 
  {
    if (strcmp(args[0], builtin_str[i]) == 0)//compares each one with the first element of the args array using strcmp() function
    {
      return (*builtin_func[i])(args);
    }
  }

  return shell_launch(args);
}


char *shell_read_line(void)
{
#ifdef SHELL_USE_STD_GETLINE
  char *line = NULL;
  ssize_t bufsize = 0; // of type ssize_t (a signed integer type), initialized to 0, it will be used to store the size of the buffer allocated by getline>
  if (getline(&line, &bufsize, stdin) == -1) {
    if (feof(stdin))//feof is used to check whether the end-of-file indicator has been set for a given file stream 
    {
      exit(EXIT_SUCCESS);  // We received an EOF
    } else  {
      perror("shell: getline\n");
      exit(EXIT_FAILURE);
    }
  }
  return line;
#else
#define SHELL_RL_BUFSIZE 1024
  int bufsize = SHELL_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;//since getchar() returns an integer value, ASCII code of the character read, -1 for EOF
 if (!buffer) {
    fprintf(stderr, "shell: allocation error\n");//prints error message to stderr(standard error). //stderr is unbuffered
    exit(EXIT_FAILURE);//EXIT_FAILURE is a macro defined in the <stdlib.h> header file in C. It's typically used to represent a generic failure exit stat>
  }
while (1) {
    // Read a character
    c = getchar();

    if (c == EOF) {
      exit(EXIT_SUCCESS);
    } else if (c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += SHELL_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
 fprintf(stderr, "shell: allocation error\n");//prints error message to stderr(standard error). //stderr is unbuffered
    exit(EXIT_FAILURE);//EXIT_FAILURE is a macro defined in the <stdlib.h> header file in C. It's typically used to represent a generic failure exit stat>
     } 
    }
  }
#endif
}

#define SHELL_TOK_BUFSIZE 64
#define SHELL_TOK_DELIM " \t\r\n\a"  // specifies a string containing delimiters used to split the input line into tokens 

char **shell_split_line(char *line)//The function splits the input line into tokens and stores them in an array of strings 
{
  int bufsize = SHELL_TOK_BUFSIZE, position = 0;//Position Tracks the current position in the tokens array 
  char *tokens = malloc(bufsize * sizeof(char));//  pointer to a pointer to a character, used to store the array of tokens
  char *token;//pointer to a character, used to store each token temporarily during tokenization 
  char  **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "shell: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, SHELL_TOK_DELIM);/* This line uses the strtok function to tokenize the input line. strtok breaks the string into a series of token>
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += SHELL_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
                free(tokens_backup);
        fprintf(stderr, "shell: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
token = strtok(NULL, SHELL_TOK_DELIM);// strtok is called again with NULL as the first argument to continue tokenizing the remaining string. This all>
  }
  tokens[position] = NULL;
  return tokens;
}


void shell_loop(void)
{
  char *line;
  char **args;
   int status;

  while(1){
    printf("> ");
    line = shell_read_line();
    args = shell_split_line(line);
   status = shell_execute(args);

    free(line);
    free(args);
  }

}


int main(int argc,char **argv)
{
printf(">
   printf("                                                 IMPLEMENTATION OF SIMPLE SHELL\n ");
                                                                             
   printf(">
   // Run command loop.
   shell_loop();

   return EXIT_SUCCESS;
}
