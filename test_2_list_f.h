#ifndef LIST_F_H
#define LIST_F_H

#include "test_2_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

//List functions

//Create List
List* create_list(int N) {
    List* list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    list->size = 0;
    Child * temp = NULL;
    
    for(int i=0;i<N;i++){
        Child* new_child = (Child*)malloc(sizeof(Child));
        if (new_child == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        new_child->id = i;
        new_child->next=NULL;
        if(list->head == NULL){
            list->head = new_child;
            temp = new_child; 
            list->size = 1;
        }else{
                temp->next = new_child;
                temp = new_child;
                list->size++;
            }    
    }
    return list;
}

//Free list
void free_list(List* list) {
    Child* current_child = list->head;
    while (current_child != NULL) {
        Child* next_child = current_child->next;
        free(current_child);
        current_child = next_child;
    }
    free(list);
}

//Iterate list
void edit_list(Child * head) {
    Child * current = head;
    while (current != NULL) {
        if(pipe(current->pipe_fd_read)==-1){
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        if(pipe(current->pipe_fd_write)==-1){
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        current = current->next;
    }
}

//Search child by id
Child* search_child(List* list, int target_id) {
    Child* current = list->head;

    while (current != NULL) {
        if (current->id == target_id) {
            // Node with the target ID found
            return current;
        }
        current = current->next;
    }

    // Node with the target ID not found
    return NULL;
}

#endif //LIST_F_H