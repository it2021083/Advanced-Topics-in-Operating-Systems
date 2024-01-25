#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include "test_2_parent.h"
#include "test_2_child.h"


pid_t fork_pid = -1; // Global



// Main program

int main(int argc, char *argv[]) {
    //Check if all arguments are given
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <text_file> <number_of_children> \n", argv[0]);
        exit(1);
    }
    
    //Open File
    const char *filename = argv[1];
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(fd == -1){
        perror("File not found");
        exit(EXIT_FAILURE);
    }

    //String to integer(Number of child processes)
    int N = atoi(argv[2]);

    //Create linked list
    List *children = create_child_list(N);
    //Create linked list
    List_P *parents = create_parent_list(N);

    //pipes
    edit_list(children->head);


    // Fork a child process
    for ( int j = 0; j < N; j++)
    {
        //Attach child process to child node
        Child *child  = search_child(children, j);

        //Attach child process to child node
        Parent *parent  = search_parent(parents, j);

        fork_pid = fork();
        if (fork_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);

        }
        
        //Processes
        if (fork_pid > 0) {  // Parent process
            parent_process(&j, child, fd, parent);
        } else {  // Child process
            //sleep(2);
            child_process(child, fd );
            
        }
    }

    sleep(2);
    parent_child_control(N,children, parents);

    //Close pipes
    for (int i = 0; i < N; i++)
    {
        //Close read pointer of the one pipe and write pointer from the other pipe
        Child *child  = search_child(children, i);
        close(child->pipe_fd_read[1]);
        close(child->pipe_fd_write[0]);
        close(child->pipe_fd_read[0]);
        close(child->pipe_fd_write[1]);
    }
    
    //Close the file descriptor
    close(fd);
    //Free allocated memory
    free_list(children);

    return 0;

}


