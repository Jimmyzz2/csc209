#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int i;
    int iterations;

    if (argc != 2) {
        fprintf(stderr, "Usage: forkloop <iterations>\n");
        exit(1);
    }

    iterations = strtol(argv[1], NULL, 10);
    int main_id = getpid();
    for (i = 0; i < iterations; i++) {
        if (main_id == getpid()) {
	    int n = fork();
	    
	    if (n < 0) {
                perror("fork");
                exit(1);
	    }	
	    if (n == 0) {
		break;
            }
	    printf("%d -> %d\n",  getpid(), n) ;
	} 
    }	
    return 0;
}
