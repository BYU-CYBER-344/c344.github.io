---
title: "Lab 5: File Sharing and Race Conditions"
---

**Scenario:** You are creating an application that needs to write event records to a log file. Each log entry is one line of text. There may be multiple instances of your application running at the same time so you need to ensure that multiple accesses to the file from different processes do not interfere with each other.

In this lab you will implement a `WriteToLog()` function that writes an entry to the end of a log file. It should be implemented in such a way that multiple processes writing to the log do not interfere with each other. Then, your `main()` function will simply call `WriteToLog()` three times with different messages.

### Learning Objectives

* Learn to use file opening, writing, and locking system calls.
* Apply file-related synchronization primitives.
* Learn to read and interpret function call documentation.

> **AI Reminder:** With the right prompt you could probably get an AI to write the WriteToLog() function for you. Doing so would violate course policies and would impair your learning (which is why that's the policy). Nevertheless, you may ask AI to explain how the system functions you will use operate and even have it give examples of how to call them. Using AI as a tutor is acceptable so long as you compose the prompts in your own words, do not have it write the actual code of your lab, and never paste or submit course content to AI (other than review questions). Your focus should be on learning how these system calls work, not on just getting the assignment done.

## Programming Reference

The following are functions we expect you to use in this application. All are nearly direct calls into the operating system except for `strlen()` which is a standard C language function. Each is a link to the official documentation of the call.

* [open()](https://man7.org/linux/man-pages/man2/open.2.html)
* [flock()](https://man7.org/linux/man-pages/man2/flock.2.html)
* [lseek()](https://man7.org/linux/man-pages/man2/lseek.2.html)
* [write()](https://man7.org/linux/man-pages/man2/write.2.html)
* [close()](https://man7.org/linux/man-pages/man2/close.2.html)
* [strlen()](https://man7.org/linux/man-pages/man3/strlen.3.html)

One of the objectives for this lab is for you to learn how to use system calls based on the official documentation. Some of these, such as `open()` are quite lengthy but all of the information you need is there. That said, you can still ask AI for examples on how to call a function. Just be sure you understand what it is doing and, preferably, know where to find it in the docs.

In addition, you will use the `GetPrefix()` function which we provide for you right here:

### GetPrefix Function

**Documentation**
```
Synopsis:

int GetPrefix(char* buf, int buflen);

buf: Address of a buffer to receive a textual log prefix
buflen: Length, in bytes, of the buffer.
Return value: The length, in bytes, of the prefix that was written into the buffer.

Description:

Generates a log entry prefix consisting of the date and time, in UTC, and the
process ID. Depending on the number of digits in the PID, the prefix will be
between 25 and 32 bytes in length.

Example use:

char prefix[32];
int prefixlen = GetPrefix(prefix, sizeof(prefix));
write(fd, prefix, prefixlen);
```

**Source**

Add these include files:
```c
#include <unistd.h>
#include <time.h>
#include <stdio.h>
```

Insert this function:
```c
int GetPrefix(char* buf, int buflen) {
    // Get the process ID
    long pid = (long)getpid();

    // Get the current time in structured format
    time_t now = time(NULL);
    struct tm tm_now;
    gmtime_r(&now, &tm_now);

    // Formatted write. (Memory safe so long as the provided buflen is correct.)
    int cb = snprintf(buf, buflen, "%04d-%02d-%02dT%02d:%02d:%02d (%ld): ",
        tm_now.tm_year+1900, tm_now.tm_mon+1, tm_now.tm_mday, tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec, pid);

    // snprintf returns the number of bytes required, not the actual number of bytes, when the buffer is too small.
    // Therefore, return the truncated length if the buffer is too small.
    return (cb < buflen) ? cb : buflen-1;
}
```


## Code a Naive Logger

Your first, "naive" version of the app will write to the log but will not prevent interference between multiple instances. Code it as follows:

1. Create your source file (typically `logger.c`).
2. Insert a comment with your name, the date, and the name of the assignment.
3. `#include` all of the needed headers. (Use the documentation from the *Programming Reference* above.)
4. Create a string constant for the name of your log file. (E.g. `lab5.log`).
5. Paste in the `GetPrefix` function from above.
6. Write a function with this declaration: `void WriteToLog(const char* message)`
    * Open the log file with appropriate `O_` flags for write-only and to create the file if it does not exist. For `mode` specify appropriate `S_` flags for user, group, and other permissions. In choosing your `S_` flags, consider how the file will be used and, therefore, who should have read and write permissions. Use the documentation for `open()` to get your arguments right. Remember that multiple `O_` or `S_` flags can be combined using the "`|`" binary OR operator.
    * Seek to the end of the file.
    * Use `GetPrefix()` to get your log entry prefix.
    * Write the prefix to the open file.
    * Write the `message` string to the file being sure to use the correct string length.
    * Write a newline character (`\n`) to complete your log entry.
    * Close the file.
7. Write your main function.
    * It should call `WriteToLog()` three times with the following messages: "App Started", "Processing data", "App exiting".

> The `mode` argument to `open()` is not the final permission set. The kernel clears any bits that are set in the `umask` of the process. A typical umask will remove write access from the group and others even if you ask for it. Also, `mode` is only effective when the file is created. If the log file already exists then it is ignored.

### Testing

Compile and run the application. It should create the log file and write out three entries. Run it again and it should add three more entries to the log file.

Now run five concurrent instances of the logger using the following Bash command:

```sh
for i in {1..5}; do ./a.out & done; wait
```

If you used something other than the default executable filename substitute it for `a.out`.

Look at your log file. Most likely, it's garbled with the different log entries mixed together and overwriting each other. However, since this is a race condition the results are non-deterministic. You could get clean output. If so, consider running more concurrent instances. Or, maybe, just run more for the fun of it to see how messy things get.

Consider how this gets messed up. `WriteToLog()` opens the file, seeks to the end, writes three times, and then closes the file. Between any two of those operations another process could work on that same file. So, for example, Process A could seek to the end, then Process B writes to the file. So, by the time Process A calls `write()`, the file pointer is no longer at the end and it overwrites the text from Process B. And with three calls to `write()` the opportunity for interference is multiplied.

## Fix Your Logger

In your `WriteToLog()` function, right after the file is opened, insert a call to `flock()` requesting exclusive access. This call will block until exclusive access is granted (unless you use the wrong operation flag).

In your `WriteToLog()` function, right before the file is closed, insert a call to `flock()` that clears your lock.

> Clearing the lock is just clean coding practice. Closing a file will clear an open lock on that file descriptor.

### Testing

Use the following command to run five concurrent instances of your application. Check the log file. All new entries should be clean.

```sh
for i in {1..5}; do ./a.out & done; wait
```

Try more instances, make sure it works correctly.

## Commentary

Now that your program is working, here are a few important concepts that are not obvious from what you have just done. These concepts are **fair game** in future quizzes or exams so be sure you understand them.

### Linux File Locks Are Advisory

Under Linux and related operating systems, file locks are advisory. That means that the operating system does not enforce them. Your locks worked in this application because all programs accessing the file followed the lock pattern. But another program that didn't use locks could still corrupt the file. There is also no coordination between file locks (using `flock()`) and byte-range locks (using `fcntl()`). If one program uses file locks and another uses byte-range locks there would be no synchronization. One process can even delete a file that is open in another process. When such a delete occurs, the file is immediately removed from directory listings but it remains open and functional for the process that has it open and it only goes away when the last file descriptor is closed.

> There is an exception to the above. When the file is accessed over the network, [NFS (Network File System)](https://en.wikipedia.org/wiki/Network_File_System) does not implement whole file locks so `flock()` is implemented using a byte-range lock. In that case `fcntl()` and `flock()` will interact.

This stands in stark contrast with Windows which enforces locks. At the time you open a file on Windows you indicate which permissions you are willing to share (read, write, and delete). If another process attempts to perform an action and the file is open without that requisite permission being shared, then the action will fail with an error.

### The O_APPEND Alternative

If you open the file with the `O_APPEND` flag then every `write()` will automatically be made at the end of the file. Furthermore, the seek to the end and the subsequent write are *atomic*, meaning that another process cannot insert a write between the seek and the write. This means that a robust logger could be written without file locks. If you open the file with O_APPEND and you compose the entire log entry in one text buffer before writing it all out with one call to `write()` then the log entry will be clean.

The `O_APPEND` strategy can fail if the file is being accessed over the network. [NFS (Network File System)](https://en.wikipedia.org/wiki/Network_File_System) does not directly implement the `O_APPEND` function. Rather, the local operating system emulates `O_APPEND` by prefixing every write with a seek to the end of the file. Thus, when writing to a network file, the `write()` operation is no longer atomic. If the log file is on a network share, a file lock would be required even when using `O_APPEND`.

### Buffered Files

In all of these labs, we are striving to call the operating system directly whenever possible. The standard C library includes buffered file functions like `fopen()`, `fread()`, `fwrite()`, and `fprintf()`. These functions insert an in-memory buffer between the operations and the actual file. Doing so makes them perform much better than the direct operating system calls when doing many short reads and writes. Additionally, the buffered file calls automatically handle partial write situations (see below) so they are more robust under signalling conditions. However, when you want to be sure data have been written out to the kernel, you must call `fflush()` after a write or series of writes. Of course, `fclose()` automatically flushes before closing the file.

### Short Writes

If a signal is handled by an application while a write operation is underway, the write operation will return with the number of bytes written being fewer than were requested. And if a signal is handled before any bytes are written it can return an error (-1) with `errno` (error number) being `EINTR`. This is known as a "short write" or "partial write". When that happens, the program should simply loop and write the remaining bytes, (all of the bytes in the case of `EINTR`). For this reason, many programs use a function called "write_all", "full_write", "safe_write", or something similar.

In the log file use case it should not happen. Linux does not permit signals to interrupt file writes on local file systems and typical NFS mounts. Where short writes *can* happen is if disk is full, disk quotas are exceeded, or the file grows beyond the maximum size. In those cases, repeated attempts to write the balance of the buffer will fail with corresponding error codes. So, those are problems that repeated writes will not address. However, signals can interrupt writing to a pipe. And there are some unusual network configurations, such as NFS soft mounts, that can result in short writes. So robust code, intended to be used with a variety of conditions should address the short write possibility.

## Submission and Scoring

To complete this lab, ensure there is a comment at the top of your source code with your name, the date, and the title of the homework. sThen submit the source to LearningSuite.

Scoring is as follows:

* [2 Points] Name, Date, and Homework Title in a comment
* [10 Points] Your program compiles without errors or warnings.
* [15 Points] Your `WriteToLog()` function locks the file right after opening it and unlocks the file right before closing it.
* [25 Points] Your program works properly and delivers the expected output.
* [8 Points] Code is clean and well-formatted.

## Extra Credit

* [10 Points] Write a new version of `WriteToLog()` called `AppendToLog()` that uses the `O_APPEND` option as described in **Commentary**. The `open()` call should include the `O_APPEND` flag. Omit the `lseek()` call and all file locking. Compose the entire log entry into one buffer which is written in a single write call. Test this to show that log writes do not interfere with each other. Your code must accommodate messages of any length, be protected against buffer overflows, and must not have memory leaks. If this is in your initial submission, be sure that your source code includes both the original `WriteToLog()` and the `AppendToLog()` alternative so that we can score both approaches.
