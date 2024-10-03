# Pjay-Shell

Pjay-Shell is a simple Unix-like command-line shell implemented in C. It supports various commands and batch file execution, providing a platform for executing shell commands in a customizable environment.

## Features
- **Command Execution**: Execute built-in commands such as `cd`, `clr`, `dir`, `environ`, `echo`, `help`, `pause`, and `quit`, ensuring seamless interaction with the shell environment.
- **Batch Mode**: Read and execute commands from a batch file, allowing for automated command processing without user intervention.
- **User-Friendly Interface**: Interactive prompt for user input, providing a familiar command-line experience.
- **Comment Handling**: Ignore lines in batch files that start with `@REM`, ensuring that comments do not interfere with command execution.
- **I/O Redirection**: Support for redirecting input and output streams, enhancing the shell's ability to handle files and processes effectively.
- **Background Execution**: Ability to run programs in the background, allowing users to continue using the shell while processes execute.

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/PjayEvangelio/Pjay-Shell.git

2. Navigate to the project directory:
   ```bash
   cd Pjay-Shell

3. Compile the source code using the provided MakeFile:
   ```bash
   make Pjay-Shell

4. Start Pjay-Shell in interactive mode by executing the Pjay-Shell binary file in the bin directory:
   ```bash
   ./bin/Pjay-Shell

## Project Stewardship
- Head Maintainer: Patrick John Evangelio

## Getting Assitance
If you run into any challenges or have inquiries regarding this project, you can:
- Create an issue in the [GitHub Issues](https://github.com/PjayEvangelio/Pjay-Shell/issues) section.
- Contact the Head Maintainer (me) directly via email for support.

## References

  ### Acknowledgments
  A heartfelt thank you to [Dr. Graham Healy](https://www.dcu.ie/computing/people/graham-healy) for providing invaluable guidance during the initial development of Pjay-Shell.

  ### Research Sources
  - A. Robbins, Unix in a Nutshell. “O’Reilly Media, Inc.,” 2005.
  - P. S. Wang, Mastering Linux. Hoboken: CRC Press, 2010.
  - N. Matthew and R. Stones, Beginning Linux Programming, Second Edition. 1999.
  - C. Newham and B. Rosenblatt, Learning the bash shell. Sebastopol, Calif. ; Farnham: O’reilly, 2005.
  - C. Negus, MAC OS X UNIX Toolbox. John Wiley & Sons, 2009.
