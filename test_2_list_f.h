#ifndef LIST_F_H
#define LIST_F_H

#include "test_2_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

//List functions

//Create Child List
List* create_child_list(int N) {
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
        memset(new_child->buffer, 0, sizeof(new_child->buffer));
        memset(new_child->lastMessage, 0, sizeof(new_child->lastMessage));
        memset(new_child->lastSent, 0, sizeof(new_child->lastSent));
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

//Create Parent List
List_P* create_parent_list(int N) {
    List_P* list = (List_P*)malloc(sizeof(List_P));
    if (list == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    list->size = 0;
    Parent * temp = NULL;
    
    for(int i=0;i<N;i++){
        Parent* new_parent = (Parent*)malloc(sizeof(Parent));
        if (new_parent == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        new_parent->id = i;
        memset(new_parent->lastMessage, 0 , sizeof(new_parent->lastMessage));
        memset(new_parent->lastSentToFile, 0 , sizeof(new_parent->lastSentToFile));
        memset(new_parent->lastSentToPipe, 0 , sizeof(new_parent->lastSentToPipe));
        new_parent->next=NULL;
        if(list->head == NULL){
            list->head = new_parent;
            temp = new_parent; 
            list->size = 1;
        }else{
                temp->next = new_parent;
                temp = new_parent;
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

void free_parent_list(List_P* list) {
    Parent* current_parent = list->head;
    while (current_parent != NULL) {
        Parent* next_parent = current_parent->next;
        free(current_parent);
        current_parent = next_parent;
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

//Search parent by id
Parent* search_parent(List_P* list, int target_id) {
    Parent* current = list->head;

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