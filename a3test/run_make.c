#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "pmake.h"


int file_exist (char *filename)
{
    struct stat buffer;
    return (stat (filename, &buffer));
}

int compare_time(struct timespec* t1, struct timespec* t2) {
    if (t1->tv_sec < t2->tv_sec) {
        return -1;
    }
    if (t1->tv_sec > t2->tv_sec) {
        return 1;
    }
    if (t1->tv_nsec < t2->tv_nsec) {
        return -1;
    }
    if (t1->tv_nsec > t2->tv_nsec) {
        return 1;
    }
    return 0;
}

struct timespec* zero_timespec() {
    struct timespec *time_spec = malloc(sizeof(struct timespec));
    time_spec->tv_sec = 0;
    time_spec->tv_nsec = 0;
    return time_spec;
}


int action(struct rule_node* node) {
    struct action_node * curr = node->actions;
    while (curr != NULL) {
        int result;
        result = fork();
        if (result == -1) {
            perror("fork");
            return 1;
        } else if (result == 0) {
            if (execvp(curr->args[0], curr->args) == -1) {
                perror("execvp");
                return 1;
            }
        } else {
            int status;
            if (wait(&status) == -1) {
                perror("wait");
                return 1;
            }
        }
        curr = curr->next_act;
    }
    return 0;
}

struct timespec *recursive_dep(struct rule_node* node) {
    // base case
    if (node->dependencies == NULL) {
        // action
        if (action(node) == 1) {
            perror("action");
            exit(1);
        }
        return zero_timespec();
    }
    // recursive case
    struct dep_node * curr = node->dependencies;
    struct timespec* max_time = zero_timespec();
    struct timespec* node_time = zero_timespec();;
    struct timespec* new_time;

    // update each dependencies, compare the last modified time
    while (curr != NULL) {
        new_time = recursive_dep(curr->rule);
        // get new_time if dependency exits 
        if (file_exist(curr->rule->target) == 0) {
            char *fd = curr->rule->target;
            struct stat *buf = malloc(sizeof(struct stat));
            stat(fd, buf);
            new_time->tv_sec = buf->st_mtimespec.tv_sec;
            new_time->tv_nsec = buf->st_mtimespec.tv_nsec;
        }    
        if (compare_time(max_time, new_time) == -1) {
                max_time = new_time;
        }
        curr = curr->next_dep;
    }

    // get node's time
    if (file_exist(node->target) == 0) {
        char *fd = node->target;
        struct stat *buf = malloc(sizeof(struct stat));
        stat(fd, buf);
        node_time->tv_sec = buf->st_mtimespec.tv_sec;
        node_time->tv_nsec = buf->st_mtimespec.tv_nsec;
        printf("%lld.%.9ld\n", (long long)node_time->tv_sec, node_time->tv_nsec);
        printf("%lld.%.9ld\n", (long long)max_time->tv_sec, max_time->tv_nsec);
        printf("this is it %d\n", compare_time(node_time,max_time));
        printf("%d",compare_time(node_time,max_time) == -1);
        printf("%s", node->target);
    }
    // compare node_time, with max_time
    if ((compare_time(node_time,max_time) == -1) || (file_exist(node->target) != 0)) {
        //action
        if (action(node) == 1) {
            perror("action");
            exit(1);
        }
    }
    if (compare_time(node_time, max_time) == -1) {
        return max_time;
    } else {
        return node_time;
    }
}

void par_recursive_dep(struct rule_node* node) {
    // base case
    int status1;
    if (node->dependencies == NULL) {
        // action
        if (action(node) == 1) {
            perror("action");
            exit(1);
        }
        return;
    }
    // recursive case
    struct dep_node * curr = node->dependencies;
    struct timespec* max_time = zero_timespec();
    struct timespec* node_time = zero_timespec();

    // update each dependencies at once, compare the last modified time
    while (curr != NULL) {
        int result = fork();
        if (result == -1) {
            perror("fork");
            exit(1);
        }
        if (result == 0) {
            par_recursive_dep(curr->rule);
            exit(1);
        }
        else {
            // get node's time
            if (wait(&status1) == -1) {
                perror("wait");
                exit(1);
            }
        }
        curr = curr->next_dep;
    }
    if (WIFEXITED(status1)) {
        if (file_exist(node->target) == 0) {
            char *fd = node->target;
            struct stat *buf = malloc(sizeof(struct stat));
            stat(fd, buf);
            node_time->tv_sec = buf->st_mtimespec.tv_sec;
            node_time->tv_nsec = buf->st_mtimespec.tv_nsec;
        }
        struct dep_node *curr2 = node->dependencies;
        while (curr2 != NULL) {
            if (file_exist(curr2->rule->target) == 0) {
                struct timespec *new_time = zero_timespec();
                char *fd = curr2->rule->target;
                struct stat *buf = malloc(sizeof(struct stat));
                stat(fd, buf);
                new_time->tv_sec = buf->st_mtimespec.tv_sec;
                new_time->tv_nsec = buf->st_mtimespec.tv_nsec;
                if (compare_time(new_time, max_time) == 1) {
                    max_time = new_time;
                }
            }
            curr2 = curr2->next_dep;
        }
        printf("%lld.%.9ld\n", (long long)node_time->tv_sec, node_time->tv_nsec);
        printf("%lld.%.9ld\n", (long long)max_time->tv_sec, max_time->tv_nsec);
        printf("this is it %d\n", compare_time(node_time,max_time));
        printf("%d",compare_time(node_time,max_time) == -1);
        printf("%s", node->target);
        // compare node_time, with max_time
        if ((compare_time(node_time, max_time) == -1) || (file_exist(node->target) != 0)) {
            //action
            if (action(node) == 1) {
                perror("action");
                exit(1);
            }
        }
    }
}




void run_make(char *target, Rule *rules, int pflag) {
    struct rule_node *tar = check_rule_node(rules, target);
    if (tar == NULL) {
        tar = rules;
    }
    if (pflag == 0) {
        recursive_dep(tar);
    } else if (pflag == 1) {
        par_recursive_dep(tar);
    }
    return;
}


