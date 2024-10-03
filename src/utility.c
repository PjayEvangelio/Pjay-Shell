#include "Pjay-Shell.h"

// Prints a welcome message to the user
void welcome_message(void) {
    printf("Welcome to Pjay-Shell!\n");

    // Display some commands to help the user navigate through the shell
    printf("Type 'help' to see the list of available commands.\n");
    printf("Type 'quit' to exit the shell.\n");
}

// Handles SIGCHLD signals to reap terminated child processes
void sigchld_handler(int sig) {
    UNUSED(sig);                                                                    // Macro to suppress unused parameter warning.

    // Save the current errno value. This is done to restore it later, ensuring
    // that the handler does not interfere with any system call error handling
    // that was in progress when the signal was received.
    int saved_errno = errno;

    // Reap all terminated children without blocking.
    // waitpid() is called in a loop to ensure that all terminated children
    // are reaped, as a single SIGCHLD could be for multiple children.
    // PID -1 waits for any child process.
    // The status is ignored (passed as 0).
    // WNOHANG specifies that waitpid should return immediately if no child
    // has exited.
    while (waitpid((pid_t)(-1), 0, WNOHANG) > 0) {}

    // Restore errno to its original value to avoid affecting the program's
    // flow due to the signal handler execution.
    errno = saved_errno;
}

// Parses the command line input and populates the CommandInfo struct
void parse_command(char **args, CommandInfo *cmdInfo) {
    int i = 0, cmdIndex = 0;
    memset(cmdInfo, 0, sizeof(CommandInfo));                                        // Initialize command information structure

    // Iterate through each argument passed to the function
    while (args[i] != NULL) {

        // Input redirection
        if (strcmp(args[i], "<") == 0) {
            i++;                                                                    // Move to the filename argument
            if (args[i] != NULL) {                                                  // Ensure there is a filename
                cmdInfo->inputFile = args[i];                                       // Assign input file for redirection
            }
        }

        // Output redirection (overwrite mode)
        else if (strcmp(args[i], ">") == 0) {
            cmdInfo->append = 0;                                                    // Set append mode to false
            i++;                                                                    // Move to the filename argument
            if (args[i] != NULL) {                                                  // Ensure there is a filename
                cmdInfo->outputFile = args[i];                                      // Assign output file for redirection
            }
        }
        // Output redirection (append mode)
        else if (strcmp(args[i], ">>") == 0) {
            cmdInfo->append = 1;                                                    // Set append mode to true
            i++;                                                                    // Move to the filename argument
            if (args[i] != NULL) {                                                  // Ensure there is a filename
                cmdInfo->outputFile = args[i];                                      // Assign output file for redirection
            }
        }
        // Regular command or argument
        else {
            cmdInfo->command[cmdIndex++] = args[i];                                 // Store the command/argument
        }
        i++;                                                                        // Move to the next argument
    }

    // Background execution
    if (i > 0 && strcmp(args[i - 1], "&") == 0) {
        cmdInfo->background = 1;                                                    // Set background flag
        cmdInfo->command[--cmdIndex] = NULL;                                        // Remove '&' from arguments
        // Note: Debugging statement below, could be removed in production code
        printf("Background command detected: %s\n", cmdInfo->command[0]);
    } else {
        cmdInfo->background = 0;                                                    // Not a background command
    }
}

void handle_redirection(CommandInfo *cmdInfo, int *saved_stdin, int *saved_stdout) {
    // Save the current standard input and output file descriptors
    *saved_stdin = dup(STDIN_FILENO);
    *saved_stdout = dup(STDOUT_FILENO);

    // Handle input redirection if specified
    if (cmdInfo->inputFile != NULL) {
        // Open the specified input file
        int in = open(cmdInfo->inputFile, O_RDONLY);
        if (in < 0) {
            perror("Failed to open input file");
            exit(EXIT_FAILURE);
        }
        // Redirect standard input to the file
        if (dup2(in, STDIN_FILENO) == -1) {
            perror("Failed to redirect standard input");
            exit(EXIT_FAILURE);
        }
        close(in);                                                                  // Close the file descriptor as it's no longer needed
    }

    // Handle output redirection if specified
    if (cmdInfo->outputFile != NULL) {
        // Set file open flags based on append mode
        int flags = O_WRONLY | O_CREAT | (cmdInfo->append ? O_APPEND : O_TRUNC);
        // Open the specified output file
        int out = open(cmdInfo->outputFile, flags, 0666);
        if (out < 0) {
            perror("Failed to open output file");
            exit(EXIT_FAILURE);
        }
        // Redirect standard output to the file
        if (dup2(out, STDOUT_FILENO) == -1) {
            perror("Failed to redirect standard output");
            exit(EXIT_FAILURE);
        }
        close(out);                                                                 // Close the file descriptor as it's no longer needed
    }
}

void restore_io(int saved_stdin, int saved_stdout) {
    // Restore the original standard input file descriptor
    if (dup2(saved_stdin, STDIN_FILENO) == -1) {
        perror("Failed to restore standard input");
        exit(EXIT_FAILURE);
    }
    // Restore the original standard output file descriptor
    if (dup2(saved_stdout, STDOUT_FILENO) == -1) {
        perror("Failed to restore standard output");
        exit(EXIT_FAILURE);
    }
    // Close the temporary file descriptors as they are no longer needed
    close(saved_stdin);
    close(saved_stdout);
}

void execute_command(char **args) {
    CommandInfo cmdInfo;
    parse_command(args, &cmdInfo);                                                  // Parse the command and its arguments

    // Set up for I/O redirection by saving the current stdin and stdout
    int saved_stdin, saved_stdout;
    saved_stdin = dup(STDIN_FILENO);
    if (saved_stdin == -1) {
        perror("Failed to duplicate stdin");
        return;                                                                     // Return to avoid exiting the shell entirely
    }
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1) {
        perror("Failed to duplicate stdout");
        close(saved_stdin);                                                         // Clean up already duplicated stdin
        return;
    }

    handle_redirection(&cmdInfo, &saved_stdin, &saved_stdout);                      // Apply redirection if specified

    // Determine if the command is an internal shell command and execute it
    if (!strcmp(cmdInfo.command[0], "cd")) {
        cd_command(cmdInfo.command);                                                // Change directory command
    } else if (!strcmp(cmdInfo.command[0], "clr")) {
        clr_command();                                                              // Clear the terminal screen
    } else if (!strcmp(cmdInfo.command[0], "dir")) {
        dir_command(cmdInfo.command);                                               // List directory contents
    } else if (!strcmp(cmdInfo.command[0], "environ")) {
        environ_command();                                                          // List environment variables
    } else if (!strcmp(cmdInfo.command[0], "echo")) {
        echo_command(cmdInfo.command);                                              // Echo command
    } else if (!strcmp(cmdInfo.command[0], "help")) {
        help_command();                                                             // Display help
    } else if (!strcmp(cmdInfo.command[0], "pause")) {
        pause_command();                                                            // Pause shell until Enter is pressed
    } else if (!strcmp(cmdInfo.command[0], "quit")) {
        quit_command();                                                             // Exit the shell
    } else {
        // Handle external commands, including support for background execution
        pid_t pid = fork();                                                         // Create a child process
        if (pid == -1) {
            perror("Failed to fork");                                               // Fork failed
            restore_io(saved_stdin, saved_stdout);                                  // Restore original stdin and stdout before returning
            return;
        } else if (pid == 0) {
            // Child process executes the external command
            if (cmdInfo.background) {
                // Redirect stdout and stderr to /dev/null for background commands
                int devNull = open("/dev/null", O_WRONLY);
                if (devNull == -1) {
                    perror("Failed to open /dev/null");
                    exit(EXIT_FAILURE);
                }
                if (dup2(devNull, STDOUT_FILENO) == -1 || dup2(devNull, STDERR_FILENO) == -1) {
                    perror("Failed to redirect output to /dev/null");
                    exit(EXIT_FAILURE);
                }
                close(devNull);
            }
            execvp(cmdInfo.command[0], cmdInfo.command);
            perror("Failed to execute command");                                    // execvp should not return, unless an error occurs
            exit(EXIT_FAILURE);
        } else {
            // Parent process: if not a background command, wait for the child to finish
            if (!cmdInfo.background) {
                int status;
                waitpid(pid, &status, 0);
            } else {
                // If a background command, print the PID and return immediately
                printf("Background process started with PID: %d\n", pid);
            }
        }
    }

    // After executing the command, restore the original stdin and stdout
    restore_io(saved_stdin, saved_stdout);

    // Asynchronously check and report any completed background processes
    int status;
    while (waitpid(-1, &status, WNOHANG) > 0);
}

void execute_batch_file(const char *batchfile) {
    // Open the batch file for reading
    FILE *file = fopen(batchfile, "r");
    if (!file) {
        // If opening the file fails, print an error message
        perror("Error opening batch file");
        return;
    }

    char buf[MAX_BUFFER];                                                           // Buffer to store each line from the batch file
    char *args[MAX_ARGS];                                                           // Array to store the tokenized arguments
    char **arg;                                                                     // Pointer to iterate through args

    // Read each line from the file until EOF
    while (fgets(buf, sizeof(buf), file) != NULL) {
        // Skip lines starting with @REM (comments) or empty lines
        if (strncmp(buf, "@REM", 4) == 0 || strncmp(buf, "\n", 1) == 0) {
            continue;
        }

        // Tokenize the line into arguments
        arg = args;
        *arg++ = strtok(buf, SEPARATORS);                                           // Initialize the first argument
        // Continue to tokenize the rest of the line
        while ((*arg++ = strtok(NULL, SEPARATORS)));

        // If the first argument is not NULL, execute the command
        if (args[0]) {
            execute_command(args);
        }
    }

    // Check for errors in closing the file (if none, close the file after processing all commands)
    if (fclose(file) != 0) {
        perror("Failed to close batch file");
    }
}

void cd_command(char **args) {
    // If no directory is specified, print the current working directory
    if (args[1] == NULL) {
        char cwd[MAX_BUFFER];                                                       // Buffer to hold the current working directory
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            // Successfully retrieved the current directory, print it
            printf("%s\n", cwd);
        } else {
            // Failed to get the current directory, print an error message
            perror("getcwd() error");
        }
    } else {
        // Attempt to change the current directory to the one specified in args[1]
        if (chdir(args[1]) != 0) {
            // Changing directory failed, print an error message
            perror("cd");
        }
    }
}

void clr_command(void) {
    // Use ANSI escape codes to clear the console screen and move the cursor to the top-left corner
    printf("\033[H\033[J");
}

void dir_command(char **args) {
    char command[1024] = "ls -al ";                                                 // Initialize with the command part

    // Check if a directory path is provided as an argument
    if (args[1]) {
        // Safely concatenate the directory argument to the command
        // Ensure that the total length does not exceed the buffer size
        strncat(command, args[1], sizeof(command) - strlen(command) - 1);
    } else {
        // Concatenate a dot for the current directory if no argument is provided
        strncat(command, ".", sizeof(command) - strlen(command) - 1);
    }

    // Execute the ls command
    int status = system(command);

    // Check for errors in executing the system command
    if (status == -1) {
        perror("Failed to execute command");
    } else if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
        fprintf(stderr, "Command exited with status %d\n", WEXITSTATUS(status));
    }
}

void environ_command(void) {
    // Utilize the global 'environ' variable to access environment variables
    extern char **environ;
    
    // Iterate through the list of environment variables and print each one
    for (int i = 0; environ[i] != NULL; i++) {
        // Print the current environment variable to stdout
        printf("%s\n", environ[i]);
    }
}

void echo_command(char **args) {
    // Start from args[1] as args[0] is the "echo" command itself
    for (int i = 1; args[i] != NULL; i++) {
        // Print each argument followed by a space to separate them
        printf("%s ", args[i]);
    }
    // After printing all arguments, print a newline to move to the next line
    printf("\n");
}

void help_command(void) {
    system("more manual/readme.md");
}

void pause_command(void) {
    // Prompt the user to press Enter to continue
    printf("Press Enter to continue...");
    
    // Wait in a loop until the user presses the Enter key
    while (getchar() != '\n');
}

void quit_command(void) {
    // Print a notification message to the user
    printf("Exiting the shell... Goodbye!\n");
    
    // Flush stdout to ensure the message is displayed before exiting
    fflush(stdout);

    // Terminates the shell program with a success status code
    exit(EXIT_SUCCESS);                                                             // Using EXIT_SUCCESS for clarity and best practice
}
