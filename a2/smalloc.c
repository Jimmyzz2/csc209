#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "smalloc.h"



void *mem;
struct block *freelist;
struct block *allocated_list;


/* This is a helper function that creates new struct block for the program needs
 * addr: address the block contains
 * size: size the block contains
 * next: a pointer to the next block
 * returns a pointer to newly constructed struct block
 */
struct block * make_node(void *addr, unsigned int size, struct block *next) {
    struct block *new_node = malloc(sizeof(struct block));
    new_node->addr = addr;
    new_node->size = (int) size;
    new_node->next = next;
    return new_node;
}

void *smalloc(unsigned int nbytes) {
    struct block * curr = freelist;
    if (curr == NULL || nbytes < 0) {
        return NULL;
    }
    //cast nbytes a mutiple of 8
    if (nbytes % 8 != 0) {
        nbytes = (nbytes / 8 + 1) * 8;
    }
    //change head
    if (curr->size == nbytes) {
        //remove from free_list
        freelist = curr->next;
        //append to allocated_list head
        curr->next = allocated_list;
        allocated_list = curr;
        return allocated_list->addr;
    }
    if (curr->size > nbytes) {
        //append to allocated_list head
        struct block *new_node = make_node(curr->addr, nbytes, allocated_list);
        allocated_list = new_node;
        //change from free_list
        freelist->size -= (int) nbytes;
        freelist->addr += nbytes;
        return new_node->addr;
    }
    // change body
	while (curr->next != NULL) {
	    if (curr->next->size == nbytes) {
            //remove from free_list
            struct block* temp = curr->next;
            curr->next = curr->next->next;
            //append to allocated_list head
            temp->next = allocated_list;
            allocated_list = temp;
            return allocated_list->addr;
	    }
	    if (curr->next->size > nbytes) {
            //append to allocated_list head
            struct block *new_node = make_node(curr->next->addr, nbytes, allocated_list);
            allocated_list = new_node;
            //change free_list
            curr->next->size -= (int) nbytes;
            curr->next->addr += nbytes;
            return new_node->addr;
	    }
        curr = curr->next;
	}
	return NULL;
}


/* This is a helper function that finds the right location to insert the block into freelist
 * node: a pointer to struct block which needs to be inserted
 * returns void
 */
void insert_freelist(struct block * node) {
    struct block * curr = freelist;
    //change head
    if (curr == NULL || curr->addr >= node->addr) {
        node->next = freelist;
        freelist = node;
        return;
    }
    //change body
    while (curr->next != NULL) {
	if (curr->next->addr >= node->addr) {
            node->next = curr->next;
            curr->next = node;
            return;
        }
        curr = curr->next;
    }
    if (curr->next == NULL) {
	curr->next = node;
    }	    
}

int sfree(void *addr) {
	struct block * curr = allocated_list;
    if (curr == NULL) {
        return -1;
    }
    //change head
    if (curr->addr == addr) {
        //remove from allocated_list
        allocated_list = curr->next;
        //append to free_list
        insert_freelist(curr);
        return 0;
    }
    //change body
    while (curr->next != NULL) {
        if (curr->next->addr == addr) {
            //append to free_list
            struct block *temp = curr->next->next;
            insert_freelist(curr->next);
            //remove from allocated_list
            curr->next = temp;
            return 0;
        }
        curr = curr->next;
    }
    return -1;
}


/* Initialize the memory space used by smalloc,
 * freelist, and allocated_list
 * Note:  mmap is a system call that has a wide variety of uses.  In our
 * case we are using it to allocate a large region of memory.
 * - mmap returns a pointer to the allocated memory
 * Arguments:
 * - NULL: a suggestion for where to place the memory. We will let the
 *         system decide where to place the memory.
 * - PROT_READ | PROT_WRITE: we will use the memory for both reading
 *         and writing.
 * - MAP_PRIVATE | MAP_ANON: the memory is just for this process, and
 *         is not associated with a file.
 * - -1: because this memory is not associated with a file, the file
 *         descriptor argument is set to -1
 * - 0: only used if the address space is associated with a file.
 */
void mem_init(int size) {
    mem = mmap(NULL, size,  PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if(mem == MAP_FAILED) {
         perror("mmap");
         exit(1);
    }
    //set up freelist
    freelist = malloc(sizeof(struct block));
    freelist->addr = mem;
    freelist->size = size;
    freelist->next = NULL;
    //set up allocatedlistp
    allocated_list = NULL;
}

void mem_clean(){
    //free freelist
    struct block * curr1 = freelist;
    while (curr1 != NULL) {
        struct block *temp = curr1;
        curr1 = curr1->next;
        free(temp);
    }
    freelist = NULL;
    //free allocatedlist
    struct block * curr2 = allocated_list;
    while (curr2 != NULL) {
        struct block *temp = curr2;
        curr2 = curr2->next;
        free(temp);
    }
    allocated_list = NULL;

}

