/*
 * This program takes in one command-line argument.
 * It then echoes that command-line argument back to stdout.
 */

#include <stdio.h>
int main() {
    int group_size = 3;
    float total_cost;
    float cost[4];
    cost[0] = 0;
    cost[1] = 10.75;
    cost[2] = 20.50;
    cost[3] = 30.0;
    total_cost = group_size * cost[4-group_size];
    printf("%f", total_cost);
    return 0;
}
