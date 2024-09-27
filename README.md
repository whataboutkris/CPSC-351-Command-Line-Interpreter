# CPSC 315 Operating System Concepts

# Assignment 1 - Command Line Interpreter

# Group Members:
Aidan Ross
Melissa Liu
Amanda Chen
Kristian Losenara
Fnu Mahsa 

# MyShell

`MyShell` is a custom Windows shell written in C++ that provides basic command-line functionality, similar to the Windows Command Prompt. It supports various commands such as `dir`, `help`, `vol`, `ping`, and more. This shell is capable of running commands asynchronously using threads.

## Features

The shell supports the following commands:
- **dir**: Lists the contents of the current directory.
- **help**: Displays a help message listing all supported commands.
- **vol**: Displays the volume label and serial number of the current drive.
- **path**: Displays the system's `PATH` environment variable.
- **tasklist**: Displays a list of currently running tasks.
- **notepad**: Opens Notepad, optionally with a specified file.
- **echo**: Outputs a message to the console.
- **color**: Changes the console text and background colors.
- **ping**: Sends ICMP Echo Request packets to a specified host.
- **exit/quit**: Exits the shell.
