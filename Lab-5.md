---
title: "Lab 5: File Sharing and Race Conditions"
---

**Scenario:** You are creating an application that needs to write event records to a log file. Each log entry is one line of text. There may be multiple instances of your application running at the same time so you need to ensure that multiple accesses to the file from different processes do not interfere with each other.

In this lab you will implement a `WriteToLog()` function that writes an entry to the end of a log file. It should be implemented in such a way that multiple processes writing to the log do not interfere with each other.

### Learning Objectives

* Learn to use file opening, writing, and locking system calls.
* Apply file-related synchronization primitives.
* Learn to read and interpret function call documentation.

> **AI Reminder:** With the right prompt you could probably get an AI to write the WriteToLog() function for you. Doing so would violate course policies and would impair your learning (which is why that's the policy). Nevertheless, you may ask AI to explain how the system functions you will use operate and even have it give examples of how to call them. Using AI as a tutor is acceptable so long as you make the requests in you own words, do not have it write the actual code of your lab, and never paste or submit course content to AI (other than review questions). Your focus should be on learning how these system calls work, not on just getting the assignment done.

## Programming Reference

The following are system calls we expect you to use in this application. Each has a link to the official documentation of the call.

* [open()](https://man7.org/linux/man-pages/man2/open.2.html)
* [flock()](https://man7.org/linux/man-pages/man2/flock.2.html)
* [lseek()](https://man7.org/linux/man-pages/man2/lseek.2.html)
* [write()](https://man7.org/linux/man-pages/man2/write.2.html)
* [close()](https://man7.org/linux/man-pages/man2/close.2.html)

In addition, you will use the `GetPrefix()` function which we have provided for you. Here is the documentation for that function.

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

## Instructions

1. Download [logger.c]{}
The `