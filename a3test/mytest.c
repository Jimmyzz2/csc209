#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "smalloc.h"


#define SIZE 160


/*my test for smalloc and sfree. */

int main(void) {

    mem_init(SIZE);
    
    char *ptrs[20];
    ptrs[0] = smalloc(10);
    write_to_mem(10, ptrs[0], 0);
    // this call wont success since there is only space for 160 - 16 = 144 bytes
    ptrs[1] = smalloc(150);
    printf("%d\n",ptrs[1] == NULL);
    // thus we cannot call 'write_to_mem(150, ptrs[1], 1);' since smalloc will return NULL
    // this will create a segementaion fault if we do so 
    // the right bytes to call smalloc
    ptrs[1] = smalloc(144);
    // this time we can call on write_to_mem(144, ptrs[1], 1)
    write_to_mem(144, ptrs[1], 1);
    printf("List of allocated blocks:\n");
    print_allocated();
    printf("List of free blocks:\n");
    print_free();
    printf("Contents of allocated memory:\n");
    print_mem();
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    //now we have no free space, all the memory has been allocated
    //we want to get some memories but we can not
    ptrs[2] = smalloc(8);
    printf("%d\n",ptrs[2] == NULL);
    //now we will have to free some memories 
    printf("freeing %p result = %d\n", ptrs[0], sfree(ptrs[0]));
    //now we have already free 16 bytes 
    //we can try to reserve 16 bytes for our new block 
    ptrs[2] = smalloc(16);
    write_to_mem(16, ptrs[2], 2);
    printf("List of allocated blocks:\n");
    print_allocated();
    printf("List of free blocks:\n");
    print_free();
    printf("Contents of allocated memory:\n");
    //notice that block contains size 16 is on the top of allocated memory
    print_mem();
    //now lets sfree everything 
    printf("freeing %p result = %d\n", ptrs[1], sfree(ptrs[1]));
    printf("freeing %p result = %d\n", ptrs[2], sfree(ptrs[2]));
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("List of allocated blocks:\n");
    print_allocated();
    printf("List of free blocks:\n");
    print_free();
    printf("Contents of allocated memory:\n");
    print_mem();
    //now lets try one more exmple 
    //since there are two free spaces 16bytes, 144bytes
    //lets cut 144 bytes into 9 pieces of 16 bytes memories
    for (int i = 3; i < 13; i++) {
	ptrs[i] = smalloc(16);
	write_to_mem(16, ptrs[i], i);
    }
    printf("List of allocated blocks:\n");
    print_allocated();
    printf("List of free blocks:\n");
    print_free();
    printf("Contents of allocated memory:\n");
    print_mem();
    //now we have sliced it, we can put it back to freelist
    for (int i = 3; i < 13; i++) {
	    printf("freeing %p result = %d\n", ptrs[i], sfree(ptrs[i]));
    }
    printf("List of allocated blocks:\n");
    print_allocated();
    printf("List of free blocks:\n");
    print_free();
    printf("Contents of allocated memory:\n");
    print_mem();
    //now if we try to reserved a spacce for 17 bytes, it wont work
    ptrs[13] = smalloc(17);
    printf("%d\n",ptrs[13] == NULL);
    //this make senses since we sliced all the memories into spaces that contians exactly
    //16 bytes
    //now we are done, clean the memory
    mem_clean();
    return 0;
}
