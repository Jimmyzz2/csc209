#include <stdio.h>
#include <string.h>
int main() {
    char *a = "123";
    printf("%d", strlen(a));
    char *b = "abc";

    strncpy(a,b,3);
    printf(a);
    return 0;

}