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
    // Alocate memory for the list of children
    List* list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Initialize list's size and a child node
    list->size = 0;
    Child * temp = NULL;
    
    // Create and store all the children to the list
    for(int i=0;i<N;i++){
        // Allocate memory for a child node
        Child* new_child = (Child*)malloc(sizeof(Child));
        if (new_child == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        // Set child's id
        new_child->id = i;
        // Ensure that all the buffers used from child are NULL
        memset(new_child->buffer, 0, sizeof(new_child->buffer));
        memset(new_child->lastMessage, 0, sizeof(new_child->lastMessage));
        memset(new_child->lastSent, 0, sizeof(new_child->lastSent));
        // Connect the new child to the previous of the list
        new_child->next=NULL;
        if(list->head == NULL){ // First child , start of the list
            list->head = new_child;
            temp = new_child; 
            list->size = sizeof(Child);
        }else{ // 2nd and after child
                temp->next = new_child;
                temp = new_child;
                list->size = list->size + sizeof(Child); // Increase the list's size per child node
            }    
    }
    return list;
}

//Create Parent List
List_P* create_parent_list(int N){
    // Alocate memory for the list of parent nodes
    List_P* list = (List_P*)malloc(sizeof(List_P));
    if (list == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Initialize list's size and a parent node
    list->size = 0;
    Parent * temp = NULL;
    
    // Create and store all the parent nodes to the list
    for(int i=0;i<N;i++){
        Parent* new_parent = (Parent*)malloc(sizeof(Parent));
        if (new_parent == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        // Set node's id
        new_parent->id = i;
        // Ensure that all the buffers used by parent node are NULL
        memset(new_parent->lastMessage, 0 , sizeof(new_parent->lastMessage));
        memset(new_parent->lastSentToFile, 0 , sizeof(new_parent->lastSentToFile));
        memset(new_parent->lastSentToPipe, 0 , sizeof(new_parent->lastSentToPipe));
        // Connect the new parent node to the previous of the list
        new_parent->next=NULL;
        if(list->head == NULL){ // First parent node, start of the list
            list->head = new_parent; 
            temp = new_parent; 
            list->size = sizeof(Parent);
        }else{  // 2nd and after parent node
                temp->next = new_parent;
                temp = new_parent;
                list->size = list->size + sizeof(Child);    // Increase the list's size per parent node
            }    
    }
    return list;
}


//Delete children list
void free_list(List* list) {
    // Create a child node and set it to the head of the list
    Child* current_child = list->head;
    // While -the created child node represents an existing child- loop to delete all the nodes
    while (current_child != NULL) {
        Child* next_child = current_child->next;    // Create a child node that shows to the next child of the previously created node
        free(current_child); // Free the allocated memory of child node size
        current_child = next_child; 
    }
    free(list);
}

// Delete the parent's nodes list 
void free_parent_list(List_P* list) {
    Parent* current_parent = list->head;
    while (current_parent != NULL) {
        Parent* next_parent = current_parent->next;
        free(current_parent);
        current_parent = next_parent;
    }
    free(list);
}

//Create the pipes for all the children
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