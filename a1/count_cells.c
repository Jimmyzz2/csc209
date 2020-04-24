#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_image(int num_rows, int num_cols, int arr[num_rows][num_cols], FILE *fp);

void print_image(int num_rows, int num_cols, int arr[num_rows][num_cols]);

// Remember to include the function prototype for count_cells
int count_cells(int num_rows, int num_cols, int arr[num_rows][num_cols]);

int main(int argc, char **argv) {
    // Print a message to stderr and exit with an argument of 1 if there are
    // not the right number of parameters, or the second argument is not -p
//    FILE *fp = fopen ("/Volumes/BOOTCAMP/csc209/zhanzhih/a1/test6.txt", "r");
    if (argc > 3 || argc < 2) {
        printf("Usage: count_cells <imagefile.txt> [-p]");
        return 1;
    }
    if (argc == 3 && (strncmp(argv[2], "-p", strlen(argv[2]))) != 0) {
        printf("Usage: count_cells <imagefile.txt> [-p]");
        return 1;
    }
    FILE *fp = fopen (argv[1], "r");
    if (fp == NULL) {
        printf("No such file");
        return 1;
    }
    int rows;
    int cols;
    fscanf(fp, "%d", &rows);
    fscanf(fp, "%d", &cols);
    int arr[rows][cols];
    read_image(rows, cols, arr, fp);
    int num =count_cells(rows, cols, arr);
    printf("Number of Cells is %d\n",num);
    if (argc == 3 && (strncmp(argv[2], "-p", strlen(argv[2]))) == 0) {
        print_image(rows, cols, arr);
    }
    return 0;
}
