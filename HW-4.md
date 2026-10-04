---
title: "HW 4: Programming in C"
---

The remaining homework and labs in this course involve systems programming which we will do in [the C Programming Language](https://www.c-language.org/). We use C because it is the language in which the operating system itself was written.Thus, it has native access to all system features. Using C can also give insights into how the operating system functions.

In this homework, you will install the C developer tools that you need and ensure they work through a simple program that makes system-level calls. In doing so, we will remind you of how pointers work because they are an essential feature of C programming.

## A: Setup and Test

We will be doing systems programming on Linux. Specifically, Ubuntu. If you are running Windows, you can use the [Windows Subsystem for Linux (WSL)](https://learn.microsoft.com/en-us/windows/wsl/install) which you likely already have since it is required for Docker. If you prefer, you can use an Ubuntu instance in a virtual machine.

If you are running macOS, we recommend using Ubuntu virtual machine. Since the Mach OS which underlies macOS is Unix-based you may be able to use the C development tools which are installed with the command `xcode-select --install`. However, these instructions assume Ubuntu.

Install the gcc compiler and related developer tools with the following commands:
```sh
sudo apt update
sudo apt install build-essential
```

The application we will develop in this homework is called `type`. It simply prints a file to the terminal, similar to how people use `cat`. To start with, we will create a variant on the traditional "Hello World!" application.

Create a working directory for this lab.

> If you are using WSL, remember that WSL can access your Windows drives through the `/mnt` directory and Windows can access your WSL files through `\\wsl$\Ubuntu`. You can take advantage of these mappings to use a Windows editor while building on Linux.


In your working directory, create the following `hello.c` file:

**hello.c**
```c
#include <unistd.h>

const char msg[] = "Hello, world!\n";

int main(void) {
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    return 0;
}
```

This is a bit different from other C "Hello, world!" examples because it uses the system-level `write()` function rather than the more commonly-used `printf()`.

Compile and link the program with the following command line:
```sh
gcc hello.c
```

If you do not specify an output filename when compiling then the executable file is written to `a.out`. Run the program with this command:
```sh
./a.out
```

## B: A Printfile Program

The main exercise in this homework is to write a C program that reads a file specified on the command line and copies it to the terminal. We will call it `printfile`.

Start with this empty program:
```c
#include <unistd.h> // read(), write()
#include <fcntl.h> // open(), close()

int main(int argc, char* argv[]) {
    return 0;
}
```

### A Quick C Refresher

* The `#include` statements bring in header files that declare functions and constants. The two includes in this sample bring in the system calls that will be needed in your application.
* The `main` function is the entry point into a program. `argc` is the number of arguments on the command line and `argv` is a pointer to an array of pointers to strings that contain the arguments. The first argument is the name of the program being run. So, if `argc == 1` then no name was given; *it should be 2*. The name of the program will be in `argv[0]` and the name of the file to be printed will be in `argv[1]`.
* An application should return `0` if it is successful and `-1` if there is an error.
* You can declare a string constant like this: `const char message[] = "Hello";`<br/>The string will contain all of the characters plus a terminating null. So `sizeof(message)` would be `6`. That's why the sample above wrote out `sizeof(message)-1` bytes. Meanwhile `strlen(message)` would be 5.

### Some Building Blocks

Open files, pipes, and network sockets are referenced by file descriptors with are integers. Three pre-opened and pre-defined descriptors are STDIN_FILENO, STDOUT_FILENO, and STDERR_FILENO. Which are simply constants for the numbers 0, 1, and 2.

To open the file named on the command line in read-only mode:
```c
int fd = open(argv[1], O_RDONLY);
```
The return value is the file descriptor. If it returns -1 then it is an error (e.g. file not found).

To declare a buffer with size of 256 bytes (you can choose a size to use):
```c
char buf[256];
```

To read a bufferfull from the input:
```c
int n = read(fd, buf, sizeof(buf));
```
The return value (`n`) will be the number of bytes that were read.

To write `n` bytes from the buffer to the standard output:
```c
int nw = write(STDOUT_FILENO, buf, n);
```
The return value (`nw`) is the number of bytes written or -1 if there was an error.

Typically you report errors to STDERR instead of STDOUT. That way error messages show in the terminal instead of being redirected when the output is sent to a file or piped to another application. To report an error:
```c
write(STDERR_FILENO, message, sizeof(message)-1);
```

### Completing the Application

The program should do the following:
1. Check the program arguments. Report an error with the proper syntax if no filename was given.
2. Open the file. Report an error if the file was not found.
3. Declare a buffer.
4. Enter a loop, reading from the open file and writing to STDOUT_FILENO.
5. Exit the loop when 0 bytes were read indicating the end of the file.
6. Close the file.

> This is a simple enough program that if you give the prior instructions to an AI, it will quickly write the program for you and do a good job. However, unless you tell it otherwise, the AI will likely use the buffered functions, `fopen`, `fread`, etc. Those come from the standard C library and offer performance benefits when you do lots of small reads and writes. In our case, we intentionally use direct calls to the operating system.

You may use AI to help with your code. For example, AI may offer advice for writing a loop that exits at the middle (after a read of 0 bytes) rather than at the end. You may ask it how to use certain functions. Also, you may take a fully written program and ask AI to evaluate it. But don't have the AI write the whole thing. Ensure you learn what you are expected to learn from this assignment.

## Writeup and Submission

To complete this homework, insert a comment at the top of your source code with your name, the date, and the title of the homework. Then submit the source to LearningSuite.

Scoring is as follows:

* [2 Points] Name, Date, and Homework Title in a comment
* [15 Points] Your program compiles and works.
* [10 Points] Your program uses the direct system calls (`open()`, `close()`, `read()`, `write()`).
* [3 points] Code is clean and well-formatted.

## Extra Credit

[10 Points] Before printing (copying) the file, clear the screen and move the cursor to the bottom of the screen.

**Hint:** Linux terminals (and Windows since roughly 2021) support ECMA-48 escape codes for clearing the screen, moving the cursor, changing colors, and more. These codes are also known as ANSI escape codes, VT100 escape codes, and Control Sequence Introducer (CSI) sequences.