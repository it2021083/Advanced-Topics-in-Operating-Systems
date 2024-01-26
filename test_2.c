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

#include <signal.h>
#include <sys/signal.h>

#include "test_2_parent.h"
#include "test_2_child.h"


pid_t fork_pid = -1; // Global

//Made everything global so signal handlers can work

//Create a linked list for children.
List *children ;
//Create a linked list for parent's nodes
List_P *parents ;
//Number of child processes
int N ;
// File descriptor
int fd ;

// SIGALARM/SIGINT signal handler of parent, also sends(with kill) SIGTERM signal to all children processes
void kill_child(int sig) {
    kill(0, SIGTERM); // With 0 it sends the SIGTERM signal to all the running processes(children and parent)
}

// SIGTERM signal handler of children
void on_sig_term(int sig){

    printf("I am child, my father just terminated me!\n");
    exit(0);
}

// SIGTERM signal handler of parent that frees all allocated memory
void on_sig_term_1(int sig){
    //Close all the pipes
    for (int i = 0; i < N; i++)
    {
        //Close read and write pipes(both ends) from every child
        Child *child  = search_child(children, i);
        close(child->pipe_fd_read[1]);
        close(child->pipe_fd_write[0]);
        close(child->pipe_fd_read[0]);
        close(child->pipe_fd_write[1]);
        wait(NULL);
    }
    printf("Closed all the pipes\n");
    
    //Close the file descriptor
    close(fd);
    printf("Closed the file descriptor of parent\n");
    //Free allocated memory
    free_list(children);
    free_parent_list(parents);
    printf("Closed the children list and the parent's nodes list\n");
    printf("Parent closed by parent\n");

    exit(0);
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
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(fd == -1){
        perror("File not found");
        exit(EXIT_FAILURE);
    }

    //String to integer(Number of child processes)
    N = atoi(argv[2]);

    //Create a linked list for children.
    children = create_child_list(N);
    //Create a linked list for parent's nodes
    parents = create_parent_list(N);

    //Create the pipes of children
    edit_list(children->head);


    // Fork a child process
    for ( int j = 0; j < N; j++)
    {
        // Assign signal handler for the SIGALARM signal
        signal(SIGALRM,(void (*)(int))kill_child);
        // Assign signal handler for the SIGINT signal
        signal(SIGINT,(void (*)(int))kill_child);
        
        //Attach child process to child node
        Child *child  = search_child(children, j);

        //Choose the right parent node that corresponds to each child
        Parent *parent  = search_parent(parents, j);

        // Create the child processes
        fork_pid = fork();
        if (fork_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);

        }
        
        //Processes

        if (fork_pid > 0) {  // Parent process
            // Signal handler of SIGTERM signal
            signal(SIGTERM, (void (*)(int))on_sig_term_1);
            // Signal handler of SIGINT signal
            signal(SIGINT, (void (*)(int))on_sig_term_1);
            parent_process(&j, child, fd, parent); // Process that writes parent's pid to the file and asks for children's pids 
            alarm(30); // Trigger a SIGALARM after 30 seconds
        
        } else {  // Child process
            // Signal handler of SIGTERM signal
            signal(SIGTERM, on_sig_term);
            // Signal handler of SIGINT signal
            signal(SIGINT, (void (*)(int))on_sig_term);
            child_process(child, fd ); // Process of each child: Write their pids to the file and send confirmation message to the parent
            
        }
    }

    parent_child_control(N,children, parents,fd); // Process that controls the exchange of messages between parent and children

    //Close all the pipes
    for (int i = 0; i < N; i++)
    {
        //Close read and write pipes(both ends) from every child
        Child *child  = search_child(children, i);
        close(child->pipe_fd_read[1]);
        close(child->pipe_fd_write[0]);
        close(child->pipe_fd_read[0]);
        close(child->pipe_fd_write[1]);
    }
    printf("Closed all the pipes\n");
    
    //Close the file descriptor
    close(fd);
    printf("Closed the file descriptor of parent\n");
    //Free allocated memory
    free_list(children);
    free_parent_list(parents);
    printf("Closed the children list and the parent's nodes list\n");

    return 0;

}


