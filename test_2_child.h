#ifndef CHILD_H
#define CHILD_H

#include "test_2_list_f.h"
#include <string.h>

//Child's functions

//child process
void child_process(int *i, Child *child,int fd){

    //set the file descriptor at the end of the file
    off_t offset = lseek(fd , 0 , SEEK_END);
    if(offset == -1){
        perror("Error seeking the end of the file");
        close(fd);
        exit(1);
    }

    //Close pipes' ends that are used from parent
    close(child->pipe_fd_read[1]);
    close(child->pipe_fd_write[0]);
    
    //store process' pid
    child->pid = getpid();
    
    //Clean the buffer
    memset(child->buffer, 0, sizeof(child->buffer));
    
    //Read parent's message
    child_reads(child);
    
    //Child process writes to the file
    child_write_file(child, fd);

    //Child process responds to parent
    child_write_pipe(child);
    
    //Close all pipes and file descriptor
    close(child->pipe_fd_read[0]);
    close(child->pipe_fd_write[1]);
    close(fd);

    //Child process terminates
    exit(0);
    
}

void child_reads(Child *child){

    //Read parent's message
    ssize_t bytesRead = read(child->pipe_fd_read[0], child->buffer, sizeof(child->buffer));
    if (bytesRead == -1) {
        perror("read");
        close(child->pipe_fd_read[0]); // Close the file descriptor in case of an error
        exit(1);
    }
    //Clean the buffer
    memset(child->buffer, 0, sizeof(child->buffer));
}

//Child_write_file
void child_write_file(Child *child, int fd){

    sprintf(child->buffer, "<child %d> -> <child %d>\n", child->pid, child->id);
    ssize_t bytesWritten = write(fd, child->buffer, strlen(child->buffer));
    if (bytesWritten == -1) {
        perror("write");
        close(fd); // Close the file descriptor in case of an error
        exit(1);
    }
    //Clean the buffer
    memset(child->buffer, 0, sizeof(child->buffer));
}

//Child sends message to pipe
void child_write_pipe(Child *child){
    
    sprintf(child->buffer, "done\n");
    ssize_t bytesWritten = write(child->pipe_fd_write[1], child->buffer, sizeof(child->buffer));
    if (bytesWritten == -1) {
        perror("write");
        close(child->pipe_fd_write[1]); 
        exit(1);
    }
    //Clean the buffer
    memset(child->buffer, 0, sizeof(child->buffer));
}

#endif //CHILD_H