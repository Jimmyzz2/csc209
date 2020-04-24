#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <time.h>
#include "socket.h"  
#include "message.h"
#include "controller.h"

#define MAXFD(x,y) ((x) >= (y)) ? (x) : (y)

int main(int argc, char *argv[]){
	int port;
	struct cignal cig;
	// A buffer to store a serialized message
	char *cig_serialized = malloc(sizeof(char)*CIGLEN);
	// An array to registered sensor devices
	int device_record[MAXDEV] = {0};
	
	if(argc == 2){
		port = strtol(argv[1], NULL, 0);
	} else{
		fprintf(stderr, "Usage: %s port\n", argv[0]);
		exit(1);
	}

	int gatewayfd = set_up_server_socket(port);
	printf("\nThe Cignal Gateway is now started on port: %d\n\n", port);

	/* TODO: Implement the body of the server.  
	 *
	 * Use select so that the server process never blocks on any call except
	 * select. If no sensors connect and/or send messsages in a timespan of
	 * 5 seconds then select will return and print the message "Waiting for
	 * Sensors update..." and go back to waiting for an event.
	 *
	 * The server will handle connections from devices, will read a message from
	 * a sensor, process the message (using process_message), write back
	 * a response message to the sensor client, and close the connection.
	 * After reading a message, your program must print the "RAW MESSAGE"
	 * message below, which shows the serialized message received from the *
	 * client.
	 * 
	 *  Print statements you must use:
     * 	printf("Waiting for Sensors update...\n");
	 * 	printf("RAW MESSAGE: %s\n", YOUR_VARIABLE);
	 */
    fd_set all_fds;
    FD_ZERO(&all_fds);
    FD_SET(gatewayfd, &all_fds);
    struct timeval tv;
    int nready;
    int max_fd = gatewayfd;




	// TODO implement select loop

	// Suppress unused variable warning.  The next 5 lines can be removed
	// after the variables are used.
	while(1) {
	    fd_set listen_fds = all_fds;
	    tv.tv_usec = 0;
	    tv.tv_sec = 5;
	    nready = select(max_fd + 1, &listen_fds, NULL, NULL, &tv);
	    if (nready == 0) {
            printf("Waiting for Sensors update...\n");
	        continue;
	    }
	    if (nready == -1) {
	        perror("gateway: select");
	        exit(1);
	    }
	    // the original socket
	    if (FD_ISSET(gatewayfd, &listen_fds)) {
	        int peer_fd = accept_connection(gatewayfd);
	        if (peer_fd < 0) {
	            perror("gateway: accept");
	            exit(1);
	        }
	        if (peer_fd > max_fd) {
	            max_fd = peer_fd;
	        }
	        FD_SET(peer_fd, &all_fds);
	    }
	    // the client socket
        for (int i = 0; i < FD_SETSIZE; ++i)
            if (FD_ISSET (i, &listen_fds) && i != gatewayfd) {
                int num_read = read(i, cig_serialized, sizeof(char)*CIGLEN);
                if (num_read == 0) {
                    exit(1);
                }
                printf("RAW MESSAGE: %s\n", cig_serialized);
                unpack_cignal(cig_serialized, &cig);
                if (process_message(&cig, device_record) == -1) {
                    exit(1);
                }
                char * result = serialize_cignal(cig);
                int num_write = write(i, result, strlen(result));
                if (num_write == 0) {
                    exit(1);
                }
                close(i);
                FD_CLR(i, &all_fds);
            }
	}
	return 0;
}
