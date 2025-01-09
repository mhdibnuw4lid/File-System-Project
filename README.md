# File-System-Project
TMN4133 System Programming Project

Project Overview
supercommand.c is a custom system programming tool designed to perform various file, directory, and keylogging operations using Linux system calls. The tool offers a menu-based interface and supports direct execution through command-line arguments.

Features
1. File Operations
Create, open, read, write, delete files.
Change file permissions.
2. Directory Operations
Create and delete directories.
Display the current working directory.
List files and subdirectories.
3. Keylogging Operations
Record keystrokes in the background.
Save keystrokes to keylog.txt with timestamps.

Setup Instructions
1.Requirements:
Ubuntu or any Linux-based operating system.
GCC compiler (gcc).
Terminal and text editor (e.g., nano, vi).

2.Clone the Repository:
Copy code
git clone <repository-link>
cd <repository-name>

3. Compile the Program:
Copy code
gcc supercommand.c -o supercommand

4.Run the Program:
For menu-based interface:
Copy code
./supercommand
For direct execution:
Copy code
./supercommand -m <mode> <operation> [parameters]

Usage
Command-Line Examples
1.File Operations:
Create a file:
./supercommand -m 1 1 newFile.txt
Change file permissions:
./supercommand -m 1 2 filename 777
2.Directory Operations:
List contents of the current directory:
./supercommand -m 2 3 .
Keylogger Operations:

Start keylogging:
bash
Copy code
./supercommand -m 3 keylog.txt

Project Structure
supercommand.c: Main source code.
keylog.txt: Output file for keylogging operations.
README.md: Project documentation.

Contributors
Mohammad Ibnu Walid Bin Abdullah
Izzan Hazirah Binti Harun
Vianney Camelie anak Kerbun
Voon Siew Ling

Reflection
This project was a learning experience in system programming using C.
Key skills learned: system calls, debugging, and team collaboration.
