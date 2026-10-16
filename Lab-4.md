---
title: "Lab 4: System Programming"
---

Among the most useful features of the Bash shell and its predecessors (sh, ksh, csh, and tcsh) is the ability to pipe the output of one application to the input of another. In this lab we will learn how shell programs use system calls to create pipes and launch other processes. The application you write will list all current processes that are running at root privilege level. It is the equivalent of this Bash command:

```sh
ps -ef | grep ^root
```

From a linux command-line run the above command so that you recognize the expected output of your application.

> **AI Reminder:** With the right prompt you could probably get an AI to write this entire lab. Doing so would violate course policies and would impair your learning (which is why that's the policy). Nevertheless, you may ask AI to explain certain concepts. For example, you could ask AI, "Explain the Linux fork() function and give an example of its use." If you are having trouble getting your code to work you can ask AI to find bugs or syntax errors. Using AI as a tutor is acceptable so long as you make the requests in you own words, do not have it write the actual code of your lab, and never paste or submit course content to AI. Your focus should be on learning how these system calls work, not on just getting the assignment done.

### Resources

* [YouTube: The Secret [Pipe] Unix Character](https://www.youtube.com/watch?v=20b9rc4BlSg)
* [`fork()` system call reference](https://man7.org/linux/man-pages/man2/fork.2.html)
* [`exec()` (`execlp`) system call reference](https://man7.org/linux/man-pages/man3/execlp.3.html)
* [`pipe()` system call reference](https://man7.org/linux/man-pages/man2/pipe.2.html)
* [`dup2()` system call reference](https://man7.org/linux/man-pages/man2/dup2.2.html)
* [`close()` system call reference](https://man7.org/linux/man-pages/man2/close.2.html)
* [`waitpid()` system call reference](https://man7.org/linux/man-pages/man2/waitpid.2.html)

## Functional Outline

Your program will be based on the `fork()` and `exec()` functions. There are many variations of `exec()`. For this lab we recommend `execlp()`. See the links above for reference material for all system calls above we expect you to use in this lab.

`fork()` creates a duplicate of the currently-running process. The original process is called the *parent* and the new copy is called the *child*. The return value from `fork()` is an integer *process ID (pid)*. Both copies of the process continue with the code immediately after the call to `fork()`. If the `pid` is not zero then it is the parent process that is running and `pid` is the `process ID` of the child. If the `pid` is zero then it is the child process.

`exec()` replaces the currently-running process image with a new process image by loading the specified executable file in its place. Thus, to run a new process, you first `fork`, and then, if the `pid` is zero, you `exec` the new application. Importantly, `exec()` retains all open file descriptors. So, after calling `fork()` and before calling `exec()` you can connect the standard input file descriptor (`STDIN_FILENO`, 0) and the standard output file descriptor (`STDOUT_FILENO`, 1) to appropiate values.

In this lab we will do the following:
* Create a pipe.
* Create a child using fork.
* In the child, connect stdout to the write end of the pipe.
* In the child, exec to the `ps` program (with appropriate arguments).
* Create another child using fork.
* In child 2, connect stdin to the read end of the pipe.
* In child 2, exec to the `grep` program (with appropriate arguments).
* Wait for both child processes to exit.

Here are the details:

> Remember to reference the appropriate `#include` files at the beginning of your program. The documentation links above will indicate which files are required.

1. Use the `pipe()` system call to create a pipe. The argument to `pipe()` is an array of two integers which receives the file descriptors for the read (index 0) and write (index 1) ends of the pipe.
2. Use `fork()` to create a duplicate process.
3. If the `pid` is zero, this is the child process. Do the following:
    * Use `dup2()` to duplicate the write end of the pipe into `STDOUT_FILENO`. Doing so automatically closes the previous `STDOUT_FILENO` descriptor.
    * `close()` both file descriptors you got from `pipe()`. This only closes the copies in the child process. Those in the parent process will remain open as will the duplicate that you made into `STDOUT_FILENO`. This is important housekeeping. Failing to close the write end can cause the read end to keep waiting for input.
    * Use `execlp()` to launch `ps` with `-ef` as the sole argument.
    * A sucessful call to `execlp()` replaces the current image with the new one. It should not return. Thus, if it returns, there was an error. You should put error reporting code immediately after `execlp()`.
4. (In the parent) Use `fork()` to create a another duplicate process.
5. if the `pid` is zero, this is the child process. Do the following:
    * Use `dup2()` to duplicate the read end of the pipe into `STDIN_FILENO`. Doing so automatically closes the previous `STDIN_FILENO` descriptor.
    * `close()` both file descriptors you got from `pipe()`. This only closes the copies in the child process. Those in the parent process will remain open as will the duplicate that you made into `STDOUT_FILENO`. This is important housekeeping. Failing to close the write end can cause the read end to keep waiting for input.
    * Use `execlp()` to launch `grep` with `^root` as the sole argument.
    * Include error reporting code after `execlp()` and before closing the `if` block.
6. (In the parent) `close()` both file descriptors you got from `pipe()`. (Yes, this is the third time you are closing them.) This will leave the only copy of the write end being the STDOUT for the first child process and the only copy of the read end being the STDIN for the second copy.
7. Use `waitpid()` to wait for the first process to end (using its non-zero `pid`) and then call `waitpid()` to wait for the second process to end.
8. Return 0 meaning success.

### Tips
* Read the documentation for each function carefully.
* The first argument to execlp() is the name of the program to run. The second argument is the name of what was run that is provided to the program. So, the first two arguments are the same. Only after that do you provide other arguments such as `-ef` or `^root`. Since execlp() takes a variable number of arguments, you have to let it know when the arguments are finished. So, the last argument must be `NULL`.
* `waitpid` requires three arguments. For the second and third you can just provide defaults, `NULL` and `0`.

## Error Handling

You should check all system calls for errors with the exception that you may ignore errors on `close()`. If an error occurs, report the error using [`perror()`](https://man7.org/linux/man-pages/man3/perror.3.html) and then exit with a non-zero value.

Here is an example:
```c
int pid = fork();
if (pid < 0) {
    perror("Fork failure.");
    exit(EXIT_FAILURE);
}
```

> `perror()` writes your error message to STDERR_FILENO instead of STDOUT_FILENO. That way error messages go to the console rather than being piped to another program. You could also write `fprintf(stdout, "Fork failure.");`

## Writing and Testing the Program

Write the program using your favorite programming editor and load it into a Linux instance similar to how you did it for **Homework 4**.

Test the program. It should give you the same output as this command: `ps -ef | grep ^root`

## Submission and Scoring

To complete this homework, insert a comment at the top of your source code with your name, the date, and the title of the homework. Then submit the source to LearningSuite.

Scoring is as follows:

* [2 Points] Name, Date, and Homework Title in a comment
* [10 Points] Your program compiles without errors or warnings.
* [30 Points] Your program works properly and delivers the expected output.
* [10 Points] Essential system calls are accompanied by appropriate error handling code. (You may ignore errors on calls to `close()`.)
* [8 Points] Code is clean and well-formatted.

## Extra Credit

[10 Points] Add a third step to the pipeline. For example, you might send the output to `less` or to `tail`.

