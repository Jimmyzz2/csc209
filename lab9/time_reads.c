/* The purpose of this program is to practice writing signal handling
 * functions and observing the behaviour of signals.
 */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

/* Message to print in the signal handling function. */
#define MESSAGE "%ld reads were done in %ld seconds.\n"

/* Global variables to store number of read operations and seconds elapsed. 
 */
long num_reads, seconds;


void handler(int code) {
    fprintf(stdout, MESSAGE, num_reads, seconds);
    exit(0);
}



/* The first command-line argument is the number of seconds to set a timer to run.
 * The second argument is the name of a binary file containing 100 ints.
 * Assume both of these arguments are correct.
 */

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: time_reads s filename\n");
        exit(1);
    }
    seconds = strtol(argv[1], NULL, 10);

    FILE *fp;
    if ((fp = fopen(argv[2], "r")) == NULL) {
      perror("fopen");
      exit(1);
    }

    /* In an infinite loop, read an int from a random location in the file,
     * and print it to stderr.
     */
    struct sigaction newact;
    newact.sa_handler = handler;
    newact.sa_flags = 0;
    sigemptyset(&newact.sa_mask);
    sigaction(SIGPROF, &newact, NULL);
    struct timeval time, zero_time;
    struct itimerval it_time;
    zero_time.tv_sec = 0;
    zero_time.tv_usec = 0;
    time.tv_sec = seconds;
    time.tv_usec = 0;
    it_time.it_interval = zero_time;
    it_time.it_value = time;
    setitimer(ITIMER_PROF, &it_time, NULL);
    num_reads = 0;
    for (;;) {
	    int rand_num = rand() % 100;
	    int num_read;
	    fseek(fp, rand_num * 4, SEEK_SET);
	    fread(&num_read, sizeof(int), 1, fp);
	    fprintf(stderr, "%d\n", num_read);
	    num_reads += 1;
    }
    return 1; // something is wrong if we ever get here!
}
