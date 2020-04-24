#include <stdio.h>
#include <stdlib.h>

/*
 Your task is to write a small C program called phone.c that uses scanf to read two values from standard input. The first is a 10 character string and the second is an integer. The program takes no command-line arguments.

If the integer is -1, the program prints the full string to stdout. If the integer is between 0 and 9 the program prints only the corresponding digit from the string to stdout. In both of these cases the program returns 0.

If the integer is less than -1 or greater than 9, the program prints the message "ERROR" to stdout and returns 1.

We haven't learned about strings yet, but you will see that to hold a string of 10 characters, you actually need to allocate space for 11 characters. The extra space is for a special character that indicates the end of the string. Use this line

char phone[11];
to declare the variable to hold the string. Other than this line, there is no starter code for this program.

Note: the program must not print any prompts. The program only prints what is specified above.
*/

int main() {
    char phone[11];
    int b = 0; 
    scanf("%s", phone);
    scanf("%d", &b);    
    if (b == -1) {
	printf("%s", phone);
	return 0;
    } 
    if (b >= 0 && b <= 9) {
	printf("%c", phone[b]);
	return 0;
    }	
    if (b < -1 || b >= 10) {
    	printf("%s", "ERROR");
	return 1;
    }
}    
