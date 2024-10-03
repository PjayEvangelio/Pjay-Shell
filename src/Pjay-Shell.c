#include "Pjay-Shell.h"

// Main function: entry point of the shell
int main(int argc, char **argv) {
    // Variables for reading and processing input
    char buf[MAX_BUFFER];                                       // Buffer for input line
    char *args[MAX_ARGS];                                       // Array for tokenized arguments
    char **arg;                                                 // Pointer for iterating through args
    char cwd[MAX_BUFFER];                                       // Buffer for current working directory
    char prompt[MAX_BUFFER + 4];                                // Buffer for shell prompt

    welcome_message();                                          // Display welcome message

    // Setup the SIGCHLD signal handler to handle background process completion
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));                                 // Initialize sigaction struct
    sa.sa_handler = &sigchld_handler;                           // Assign handler function
    sigemptyset(&sa.sa_mask);                                   // Initialize signal set
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;                    // Set flags
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {                  // Apply signal action
        perror("sigaction");                                    // Error handling
        exit(1);
    }

    // Check for batch file argument
    if (argc > 1) {
        execute_batch_file(argv[1]);                            // Execute batch file commands
        return 0;                                               // Exit after batch file processing
    }

    // Interactive mode
    while (!feof(stdin)) {                                      // Loop until end of input
        // Update and display the prompt
        if (getcwd(cwd, sizeof(cwd)) == NULL) {                 // Get current working directory
            perror("getcwd() error");                           // Error handling
            exit(EXIT_FAILURE);
        }
        fprintf(stdout, "%s ==> ", cwd);                        // Print prompt
        fputs(prompt, stdout);                                  // Print prompt
        fflush(stdout);                                         // Flush stdout

        // Read input
        if (fgets(buf, MAX_BUFFER, stdin)) {
            // Tokenize input into args array
            arg = args;
            *arg++ = strtok(buf, SEPARATORS);                   // First token
            while ((*arg++ = strtok(NULL, SEPARATORS)));        // Subsequent tokens

            // Execute command
            if (args[0]) {
                execute_command(args);
            }
            // Background process completion is handled by sigchld_handler
        }
    }

    return 0;                                                   // Exit successfully
}
