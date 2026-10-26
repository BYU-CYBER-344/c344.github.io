// Insert name, date, and assignment name here.

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/file.h>
#include <time.h>

const char c_logfilename[] = "lab5.log";

int GetPrefix(char* buf, int buflen) {

    // Get the process ID
    long pid = (long)getpid();

    // Get the the current time in structured format
    time_t now = time(NULL);
    struct tm time;
    gmtime_r(&now, &time);

    // Buffer-safe formatting write
    return snprintf(buf, buflen, "%04d-%02d-%02dT%02d:%02d:%02d (%ld): ",
        time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec, pid);
}

void WriteToLog(const char *message)
{
    // TODO: Implement this function:
    // * Use GetPrefix to read the log prefix into a buffer.
    // * Open the file named by c_logfilename with the right options
    //   for write only and create if it does not exist.
    // * Lock the file using flock();
    // * Seek to the end of the file.
    // * Write the prefix
    // * Write the message
    // * Write a newline character (\n)
    // * Unlock the file
    // * Close the file
}

int main() {
    WriteToLog("App started.");
    WriteToLog("Processing data.");
    WriteToLog("App exited.");
}