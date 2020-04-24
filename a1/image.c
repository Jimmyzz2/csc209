#include <stdio.h>
    
/* Reads the image from the open file fp into the two-dimensional array arr 
 * num_rows and num_cols specify the dimensions of arr
 */
void read_image(int num_rows, int num_cols,
                int arr[num_rows][num_cols], FILE *fp) {
    for(int i = 0; i < num_rows; i++){
        for(int j = 0; j < num_cols; j++){
            fscanf(fp, "%d", &(arr[i][j]));
        }
    }
}

/* Print to standard output the contents of the array arr */
void print_image(int num_rows, int num_cols, int arr[num_rows][num_cols]) {
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            printf("%d", arr[i][j]);
            if (j == num_cols - 1){
                printf("\n");
            }
            else {
                printf(" ");
            }

        }
    }
}

void change(int rows, int cols, int arr[rows][cols], int i, int j) {
    //base case
    if (arr[i][j] != 255) {
        return;
    }
    // recursive case
    // change the num
    arr[i][j] = 1;
    // no touching left wall
    if (j != 0) {
        change(rows, cols, arr, i, j-1);
    }
    // no touching right wall
    if (j != cols - 1) {
        change(rows, cols, arr, i, j+1);
    }
    // no touching upper wall
    if (i != 0) {
        change(rows, cols, arr, i-1, j);
    }
    // no touching lower wall
    if (i != rows - 1) {
        change(rows, cols, arr, i+1, j);
    }
}

//
///* TODO: Write the count_cells function */
int count_cells(int num_rows, int num_cols, int arr[num_rows][num_cols]) {
    int count = 0;
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            if (arr[i][j] == 255) {
                count += 1;
                change(num_rows, num_cols, arr, i, j);
            }
        }
    }
    return count;
}
