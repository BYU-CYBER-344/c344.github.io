---
title: "Lab 4: System Programming"
---

(Under Construction)

Among the most useful features of the bash shell and its predecessors (sh, ksh, csh, and tcsh) is the ability to pipe the output of one application to the input of another. In this lab we will learn how shell programs use system calls to create pipes and launch other processes. The application you write will list all current processes that are running at root privilege level. It is the equivalent of this Bash command:

```sh
ps -ef | grep ^root
```

### Resources

* YouTube Video: The Invention of the Pipe
* `fork` system call
* `exec` (`execlp`) system call
* `pipe` system call
* `dup2` system call
* `close` system call
* `waitpid` system call

## Functional Outline

## Writing the Program

## Submission and Scoring
