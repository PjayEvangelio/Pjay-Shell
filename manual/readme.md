Pjay-Shell User Manual


Introduction

With its intuitive interface and Unix-like command-line interpreter, Pjay-Shell is designed to give users effective file and process management tools. With an easy-to-use interface for running commands and exploring the filesystem, this shell was created with simplicity in mind and is especially suited for those who are new to UNIX systems. Pjay-Shell offers a strong basis for handling a wide range of activities with simplicity and efficiency, regardless of your level of experience with command-line interfaces. This includes both novices and seasoned users looking for a lightweight yet flexible solution.


Commands

cd
- Usage: cd [directory]
- Description: Changes the current working directory to the one specified. If no directory is provided, it reports the current directory. If the directory does not exist, an error is reported. This command also updates the PWD environment variable.

clr
- Usage: clr
- Description: Clears the terminal screen, removing all previously displayed commands and outputs.

dir
- Usage: dir [directory]
- Description: Lists the contents of the specified directory in detail, similar to ls -al in UNIX. If no directory is provided, it lists the contents of the current directory.

environ
- Usage: environ
- Description: Lists all the environment variables of the shell session, providing key information about the shell's operating environment.

echo
- Usage: echo [text]
- Description: Displays the specified text on the display, followed by a new line. Multiple spaces/tabs in the input text may be reduced to a single space.

help
- Usage: help
- Description: Displays the user manual using the more filter, allowing users to scroll through the manual page by page.

pause
- Usage: pause
- Description: Pauses operation of the shell, resuming only when 'Enter' is pressed. Useful for halting the shell's activity temporarily.

quit
- Usage: quit
- Description: Quits the shell, terminating the session.


Essential Concepts


Other Commands and Integration

One of Pjay-Shell's greatest qualities is its ability to expand and integrate with ease beyond its basic features. Pjay-Shell dynamically looks for external programmes to run when a user types a command that doesn't match one of the built-in commands. Process forking and the usage of execv to substitute the command-specified programme for the shell process allow for this to be accomplished. This feature, which runs outside of apps in an environment filled with necessary variables—like 'parent=/Pjay-Shell,' which indicates the location of the Pjay-Shell executable—makes it especially potent. It is recommended that users expand the functionality of Pjay-Shell by adding executables to the system's PATH or establishing custom paths, which will open up countless options for automation and personalisation.


Batch Mode

Pjay-Shell's batch mode capabilities, which enables the sequential execution of commands from a file, is one of its best features. Users can automate processes automatically without requiring human intervention by starting Pjay-Shell with the path to a batch file as an argument. In order to enable a hands-off approach to command execution, this mode reads and runs commands line by line until the file ends. Pjay-Shell's adaptability is best demonstrated by its batch mode, which helps users automate complicated procedures or simplify repetitive chores to increase productivity and job management effectiveness.


I/O Redirection

Sophisticated I/O redirection is supported by Pjay-Shell, giving users the ability to route command input and output to and from files, increasing the versatility and strength of command execution. For jobs that need to process data from a file or save output for later inspection, this functionality is crucial.

- Input Redirection (<): Pjay-Shell allows you to reroute a command's standard input (stdin) from a file to the keyboard by using the < operator. This makes it possible for a programme to read data from a file and interpret it as though the user had typed it themselves. For instance, programname arg1 arg2 < inputfile runs programname with arguments arg1 and arg2, substituting inputfile for stdin.

- Output Redirection (> and >>): Using the > and >> operators, Pjay-Shell enables the redirection of standard output (stdout) to a file. When you use >, a command's output is redirected to a file, either creating it if it doesn't already exist or truncating it if it does. The output is appended to the file with the >> operator, keeping its original contents intact. This function is essential for processing outputs with other tools or storing the outcomes of instructions.

  - For example, programname arg1 arg2 > outputfile creates or truncates the output of programname as needed and directs it into outputfile. As an alternative, programname arg1 arg2 >> outputfile appends the output while preserving the content of outputfile.

  - Pjay-Shell extends output redirection capabilities to internal commands such as dir, environ, echo, and help, allowing their outputs to be directed to files for documentation or further processing. This integration ensures that both external and built-in commands can leverage the full potential of I/O redirection for advanced data management tasks.


Program Environment

Pjay-Shell functions within a sophisticated programme environment made up of variables that provide specifics about user preferences and system characteristics. With a window into this environment, the environ command displays all shell session variables that are currently in use. Users can personalise their Pjay-Shell experience, adjust functionality, and streamline their workflow by becoming proficient in the programme environment. Comprehending and adjusting the environment variables can have a substantial effect on shell behaviour, the visibility of system properties, and the way custom shell scripts run.


Background Program Execution

With Pjay-Shell, users may run commands in the background, which makes the shell responsive to new inputs even while previously executed commands are still running. This improves multitasking capabilities. Performing lengthy or resource-intensive tasks without interfering with the user's productivity is especially helpful in this regard. To indicate to Pjay-Shell that the user wants the programme to execute in the background while the shell returns to the command prompt right away, background execution is indicated by adding an ampersand (&) to the end of a command line.

But this capability is meant to be used with "Other Commands," which are outside programmes that aren't integrated into the shell. Because of their design and the way the shell operates, internal commands run inside the shell process and usually finish before the shell asks for more input. Users can easily start many jobs at once and wait for each to finish by using the & character. To maximise productivity and efficiency in a multitasking environment, the user can carry out other actions while a file backup or network scan is running in the background.

Notably, background processes are separated from the interactive shell session, which permits the shell to receive and handle new instructions while background processes are running. If supported, users can oversee the execution of these background operations using system tools or by using job control instructions. Pjay-Shell's versatility and intuitive design are demonstrated by its ability to execute operations in the background, which makes it a useful tool for handling a range of jobs in a dynamic computer environment.
