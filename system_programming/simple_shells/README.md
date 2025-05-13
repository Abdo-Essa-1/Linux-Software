# My Simple Shells
This is my own created Shells in linux using c programing, With ubuntu 22.04.

Each Shell separated in c files given in this folder.

# Shell 1: Femto Shell
This simple Shell contains only echo and exit utilities.

# Shell 2: Pico Shell
My Femto Shell shall grow and will become a Pico Shell :D

My Pico Shell supports:
- Built-in echo, pwd and cd, in addition to exit. And any other commands supported in the PATH variable can be excuted using fork and execvp system calls.
- Command line parsing to parse the command line entered by the user, divide it into arguments (the only supported separator is the space or multiple successive spaces).
- Executing the programs using their names directly without the need to enter the full path.
