#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include "test_2_parent.h"
#include "test_2_child.h"

//Gracefull shutdown
#include <signal.h>
#include <unistd.h>

pid_t fork_pid = -1; // Global

void kill_child(int sig){
    kill(fork_pid, SIGTERM);
}

void on_sig_term(int sig){
    printf("I am child, my father just terminates me! \n");
}

void kill_child_2(int sig){
    kill(fork_pid, SIGINT);
}

void on_sig_term_2(int sig){
    printf("I am child, my father just terminates me! \n");
}


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
    List *children = create_list(N);

    //pipes
    edit_list(children->head);

    // Fork a child process
    for ( int j = 0; j < N; j++)
    {
        //Attach child process to child node
        Child *child  = search_child(children, j);
        
        fork_pid = fork();
        if (fork_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        
        //Processes
        if (fork_pid > 0) {  // Parent process
            parent_process(&j, child, fd);
            signal(SIGINT, (void (*)(int))kill_child_2);
            signal(SIGTERM, (void (*)(int))kill_child);
            //sleep(30);
            //alarm(30);
            //wait(NULL);
            //printf("Child has been gracefully shut down\n");
        } else {  // Child process
            //sleep(2);
            child_process(&j, child, fd );
            signal(SIGINT, (void (*)(int))kill_child_2);
            signal(SIGTERM, (void (*)(int))kill_child);
            //printf("This is a test\n");
            //sleep(60);
            
        }
    }
    
    //Close the file descriptor
    close(fd);
    //Free allocated memory
    free_list(children);

    return 0;

}


