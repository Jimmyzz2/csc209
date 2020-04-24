#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#ifndef PORT
  #define PORT 54852
#endif
#define BUF_SIZE 128

int main(void) {
    // Create the socket FD.
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("client: socket");
        exit(1);
    }

    // Set the IP and port of the server to connect to.
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) < 1) {
        perror("client: inet_pton");
        close(sock_fd);
        exit(1);
    }

    // Connect to the server.
    if (connect(sock_fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("client: connect");
        close(sock_fd);
        exit(1);
    }

	// Get the user to provide a name.
    char buf[2 * BUF_SIZE + 1];            // 2x to allow for usernames
    printf("Please enter a username: ");
    fflush(stdout);
    int num_read = read(STDIN_FILENO, buf, BUF_SIZE);
    buf[num_read] = '\0';
    write(sock_fd, buf, num_read);

    // Read input from the user, send it to the server, and then accept the
    // echo that returns. Exit when stdin is closed.
    int max_fd;
    if (STDIN_FILENO >= sock_fd) {
        max_fd = STDIN_FILENO;
    } else {
        max_fd = sock_fd;
    }
    fd_set set;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    FD_SET(sock_fd, &set);

    while (1) {
        int nready = select(max_fd+1, &set, NULL, NULL, NULL);
        if (nready == -1) {
            perror("client: select");
            exit(1);
        }
        if (FD_ISSET(sock_fd,&set)) {
            num_read = read(sock_fd, buf, BUF_SIZE);
            if (num_read == 0) {
                exit(1);
            }
            buf[num_read] = '\0';
            printf("Received from server: %s", buf);
        }
        if (FD_ISSET(STDIN_FILENO, &set)) {
            num_read = read(STDIN_FILENO, buf, BUF_SIZE);
            if (num_read == 0) {
                break;
            }
            buf[num_read] = '\0';
            int num_written = write(sock_fd, buf, num_read);
            if (num_written != num_read) {
                perror("client: write");
                close(sock_fd);
                exit(1);
            }
        }
		// Should really send '\r\n'
        // reset set
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        FD_SET(sock_fd, &set);
    }

    close(sock_fd);
    return 0;
}
