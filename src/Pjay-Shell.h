#ifndef PJAY_SHELL_H
#define PJAY_SHELL_H

// Standard library includes for functionality
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

// Constants for buffer sizes and token separators
#define MAX_BUFFER 1024
#define MAX_ARGS 64
#define SEPARATORS " \t\n"
#define UNUSED(x) (void)(x)     // Macro to suppress unused parameter warning

// CommandInfo struct: holds information about a command
typedef struct CommandInfo {
    char *command[MAX_ARGS];    // Command and arguments
    char *inputFile;            // Input redirection file, if any
    char *outputFile;           // Output redirection file, if any
    int append;                 // Append mode for output redirection
    int background;             // Background execution flag
} CommandInfo;

// Function prototypes for shell operations
void welcome_message(void);
void sigchld_handler(int sig);
void parse_command(char **args, CommandInfo *cmdInfo);
void handle_redirection(CommandInfo *cmdInfo, int *saved_stdin, int *saved_stdout);
void restore_io(int saved_stdin, int saved_stdout);
void execute_command(char **args);
void execute_batch_file(const char *batchfile);
void cd_command(char **args);
void clr_command(void);
void dir_command(char **args);
void environ_command(void);
void echo_command(char **args);
void help_command(void);
void pause_command(void);
void quit_command(void);

#endif                          // PJAY_SHELL_H
