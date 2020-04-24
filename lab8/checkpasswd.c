#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 256

#define SUCCESS "Password verified\n"
#define INVALID "Invalid password\n"
#define NO_USER "No such user\n"

int main(void) {
  char user_id[MAXLINE];
  char password[MAXLINE];

  /* The user will type in a user name on one line followed by a password 
     on the next.
     DO NOT add any prompts.  The only output of this program will be one 
	 of the messages defined above.
   */

  if(fgets(user_id, MAXLINE, stdin) == NULL) {
      perror("fgets");
      exit(1);
  }
  if(fgets(password, MAXLINE, stdin) == NULL) {
      perror("fgets");
      exit(1);
  }
  
  // TODO
  int fds[2];
  //pipe
  if (pipe(fds) == -1) {
	 perror("pipe");
  }
  //fork
  int result = fork();
  if (result < 0) {
	  perror("fork");
	  exit(1);
  } else if (result == 0) { //child process
	close(fds[1]);
	// Reset stdi so that it reads from the pipe
	if (dup2(fds[0], fileno(stdin)) == -1) {
	       perror("dup2");
	       exit(1);
	}
	// excel on the new stdi 
	execl("./validate", "validate", NULL);
	// if excel is not sucessful 
	perror("exec");
	return 1;
	
  } else { //parent process 
	int status;
	close(fds[0]);
	write(fds[1], user_id, sizeof(char) * 10);
	write(fds[1], password, sizeof(char) * 10);
	close(fds[1]);
	if (wait(&status) == -1) {
		perror("wait");
		exit(1);
	}
	if (WIFEXITED(status)) {
		int ans = WEXITSTATUS(status);
		if (ans == 0) {
			printf(SUCCESS);
		} else if (ans == 2) {
			printf(INVALID);
		} else {
			printf(NO_USER);
		}	
	} else if (WIFSIGNALED(status)) {
		fprintf(stderr, "child process terminated due to singal\n");
	}
  }	 

  return 0;
}
