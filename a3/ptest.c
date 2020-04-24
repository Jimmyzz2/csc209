#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "pmake.h"

/* This program reads and evaluates a Makefile very much like the program make
   but with some simplifications, and the option to evaluate dependencies in 
   parallel.

   Options:
     "-o"  - print the rules data structure as if it were a Makefile
     "-p"  - create a new process to evaluate each dependency of a rule
     "-f <filename>" - Use "filename" as the input Makefile.  If this option is
             not provided then pmake will use a file called "Makefile"
     "target" - specify the rule in the file to evaluate. If no target is
             provided, pmake will evaluate the first rule.
 */

int file_exist (char *filename)
{
    struct stat buffer;
    return (stat (filename, &buffer));
}


int main() {
    if (file_exist("test.txt") == 0) {
        char *fd = "test.txt";
        struct stat *buf = malloc(sizeof(struct stat));
        stat(fd, buf);
        struct timespec *time_spec;
        time_spec = (&(buf->st_mtimespec));
        printf("%lld.%.9ld\n", (long long)time_spec->tv_sec, time_spec->tv_nsec);
        if (time_spec->tv_nsec >= 0 && time_spec->tv_sec >=0) {
            printf("Pig");
        }
        free(buf);
    }

    return 0;
}
