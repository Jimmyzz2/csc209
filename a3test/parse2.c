#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>

#include "pmake.h"


/* Print the list of actions */
void print_actions(Action *act) {
    while(act != NULL) {
        if(act->args == NULL) {
            fprintf(stderr, "ERROR: action with NULL args\n");
            act = act->next_act;
            continue;
        }
        printf("\t");

        int i = 0;
        while(act->args[i] != NULL) {
            printf("%s ", act->args[i]) ;
            i++;
        }
        printf("\n");
        act = act->next_act;
    }    
}

/* Print the list of rules to stdout in makefile format. If the output
   of print_rules is saved to a file, it should be possible to use it to 
   run make correctly.
 */
void print_rules(Rule *rules){
    Rule *cur = rules;
    
    while(cur != NULL) {
        if(cur->dependencies || cur->actions) {
            // Print target
            printf("%s : ", cur->target);
            
            // Print dependencies
            Dependency *dep = cur->dependencies;
            while(dep != NULL){
                if(dep->rule->target == NULL) {
                    fprintf(stderr, "ERROR: dependency with NULL rule\n");
                }
                printf("%s ", dep->rule->target);
                dep = dep->next_dep;
            }
            printf("\n");
            
            // Print actions
            print_actions(cur->actions);
        }
        cur = cur->next_rule;
    }
}

/* Create the rules data structure and return it.
   Figure out what to do with each line from the open file fp
     - If a line starts with a tab it is an action line for the current rule
     - If a line starts with a word character it is a target line, and we will
       create a new rule
     - If a line starts with a '#' or '\n' it is a comment or a blank line 
       and should be ignored. 
     - If a line only has space characters ('', '\t', '\n') in it, it should be
       ignored.
 */


void find_dep(char* sentence, int num, char* result) {
    // find the num-th dependencies in a sentence
    int count = 0;
    int result_index = 0;
    for (int i = 0; i < strlen(sentence); i++) {
        if (count == num + 1) {
            break;
        }
        if (count == num) {
            // not include the empty space
            if (sentence[i] != ' ' && sentence[i] != '\n' && sentence[i] != '\t' ) {
                result[result_index] = sentence[i];
                result_index += 1;
            }
        }
        // increment count
        if (sentence[i] == ' ') {
            count += 1;
        }
    }
}

struct rule_node* check_rule_node(struct rule_node* rules, char* name) {
    struct rule_node* curr = rules;
    while (curr!= NULL) {
        if ((strcmp(curr->target, name) == 0))  {
            return curr;
        }
        curr = curr->next_rule;
    }
    return NULL;
}

struct rule_node* build_rule_node(char *target) {
    struct rule_node* new_node = malloc(sizeof(struct rule_node));
    new_node->target = malloc(sizeof(char) * (strlen(target) + 1));
    new_node->dependencies = malloc(sizeof(struct dep_node));
    new_node->actions = malloc(sizeof(struct action_node));
    new_node->next_rule = malloc(sizeof(struct rule_node));
    memset(new_node->target, '\0', (strlen(target) + 1));
    strcpy(new_node->target, target);
    new_node->dependencies = NULL;
    new_node->actions = NULL;
    new_node->next_rule = NULL;
    return new_node;
}

struct action_node* build_action_node(char * action_line) {
    struct action_node* new_node = malloc(sizeof(struct action_node));
    // get # of act of one action_line
    int act = 0;
    for (int i = 0; i < strlen(action_line); i++) {
        if (action_line[i] == ' ' && i >= 1 && i < strlen(action_line) - 1) {
            // there should not be two consecutive spaces
            if (action_line[i-1] != ' ' && action_line[i+1] != ' ' &&
                action_line[i-1] != '\n' && action_line[i+1] != '\n'){
                act += 1;
            }
        }
    }
    act += 1;
    printf("\n%d  duck you\n", act);
    // now # actions are all get
    char ** act_list = malloc(sizeof(char*) * (act + 1));
    for (int i = 0; i < act + 1; i++) {
        act_list[i] = malloc(sizeof(char) * (MAXLINE+1));
    }
    for (int i = 0; i < act + 1; i++) {
        if (i == act) {
            act_list[i] = NULL;
        } else {
            memset(act_list[i],'\0', MAXLINE+1);
            find_dep(action_line, i, act_list[i]);
            printf("\nvery close2%s, %lu\n", act_list[i], strlen(act_list[i]));
        }
    }
    for (int i = 0; i < act; i ++ ) {
        printf("\nvery close%s, %lu\n", act_list[i], strlen(act_list[i]));
    }
    new_node->args = malloc(sizeof(char*) * act);
    new_node->args = act_list;
    new_node->next_act = malloc(sizeof(struct action_node));
    new_node->next_act = NULL;
    return new_node;
}

struct dep_node * build_dep_node(struct rule_node* rule) {
    struct dep_node * new_node = malloc(sizeof(struct dep_node));
    new_node->rule = malloc(sizeof(struct rule_node));
    new_node->rule = rule;
    new_node->next_dep = malloc(sizeof(struct dep_node));
    new_node->next_dep = NULL;
    return new_node;
}


void target_actions(struct rule_node* target_node, char action_lines[50][257], int i) {
    int k = 0;
    if (i == 0) {
        return;
    }
    struct action_node * curr_node = build_action_node(action_lines[0]);
    target_node->actions = curr_node;
    while (k+1 < i) {
        curr_node -> next_act = build_action_node(action_lines[k+1]);
        curr_node = curr_node->next_act;
        k += 1;
    }
}
void target_dependencies(struct rule_node* target_node, int dep, char dep_list[dep][257], struct rule_node* first_node,
        struct rule_node* curr_node){
    if (dep == 0) {
        return;
    }
    struct rule_node* first_dep_rule = check_rule_node(first_node, dep_list[0]);
    if (first_dep_rule == NULL) {
        first_dep_rule = build_rule_node(dep_list[0]);
        curr_node->next_rule = first_dep_rule;
        curr_node = curr_node->next_rule;
    }
    target_node->dependencies = build_dep_node(first_dep_rule);
    struct dep_node* curr2_node = target_node->dependencies;
    for (int i = 1; i < dep; i++) {
        struct rule_node*  curr_dep_node = check_rule_node(first_node, dep_list[i]);
        if (curr_dep_node == NULL) {
            curr_dep_node = build_rule_node(dep_list[i]);
            curr_node->next_rule = curr_dep_node;
            curr_node = curr_node->next_rule;
        }
        curr2_node->next_dep = build_dep_node(curr_dep_node);
        curr2_node = curr2_node->next_dep;
    }
}

Rule *parse_file(FILE *fp) {
    char curr_line[MAXLINE + 1];
    char* condition;
    struct rule_node * first_node;
    struct rule_node * curr_node;
    // read the first target line
    while (1) {
        condition = fgets(curr_line, MAXLINE+1, fp);
        if (isalpha(curr_line[0]) || isdigit(curr_line[0])) {
            // create the first node
            // get target
            char * first_space = strchr(curr_line, ' ');
            unsigned len_target = strlen(curr_line) - strlen(first_space) + 1;
            char target[len_target];
            memset(target,'\0', len_target);
            strncpy(target, curr_line, len_target - 1);
            printf("%s\n", target);
            first_node = build_rule_node(target);
            curr_node = first_node;
            break;
        }
    }
    while (condition != NULL){
        // must be target line here
        // get target
        char * first_space = strchr(curr_line, ' ');
        unsigned len_target = strlen(curr_line) - strlen(first_space) + 1;
        char target[len_target];
        memset(target,'\0', len_target);
        strncpy(target, curr_line, len_target - 1);
        printf("%s\n", target);
        struct rule_node * target_node;
        if ((target_node = check_rule_node(first_node, target)) == NULL) {
            target_node = build_rule_node(target);
            curr_node->next_rule = target_node;
            curr_node = curr_node->next_rule;
        }
        // get dependencies
        // first count how many dependencies it has
        int dep = 0;
        for (int i = 0; i < strlen(first_space); i++) {
            if (first_space[i] == ' ' && i >= 1 && i < strlen(first_space) - 1) {
                // there should not be two consecutive spaces
                if (first_space[i-1] != ' ' && first_space[i+1] != ' ' &&
                first_space[i-1] != '\n' && first_space[i+1] != '\n'){
                    dep += 1;
                }
            }
        }
        printf("%d\n", dep);
        // now # dependencies are all get
        char dep_list[dep][MAXLINE + 1];
        for (int i = 0; i < dep; i++) {
            // skip the first white space
            memset(dep_list[i],'\0', MAXLINE+1);
            find_dep(first_space, i+2, dep_list[i]);
        }
        for (int i = 0; i < dep; i ++ ) {
            printf("%s, %lu\n", dep_list[i], strlen(dep_list[i]));
        }
        // find the next few action lines
        char action_lines[50][MAXLINE+1];
        int i = 0;
        while ((condition = fgets(curr_line, MAXLINE+1, fp)) != NULL) {
            // break at new target line
            if (isalpha(curr_line[0]) || isdigit(curr_line[0])) {
                break;
            }
            // record action line
            if (curr_line[0] == '\t') {
                memset(action_lines[i], '\0', MAXLINE+1);
                strcpy(action_lines[i], curr_line);
                printf("%s", action_lines[i]);
                i += 1;
            }
        }
        // target action line
        target_actions(target_node, action_lines, i);
        // target dependency line
        target_dependencies(target_node, dep, dep_list, first_node, curr_node);
        while (curr_node->next_rule != NULL) {
            curr_node = curr_node->next_rule;
        }
    }
    return first_node;
}


int main() {
    FILE * fp;
    if ((fp = fopen("test.txt","r")) == NULL) {
        perror("fopen");
        exit(1);
    }
    struct rule_node * rule_node =  parse_file(fp);
    print_rules(rule_node);
//    build_action_node("\tgcc -o test1 test1.o");


    }


