#ifndef CHILD_H
#define CHILD_H

#include "test_2_list_f.h"
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>

//Child's functions

void child_reads_first_time(Child *child){

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
void child_write_file_first_time(Child *child, int fd){

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
void child_write_pipe_first_time(Child *child){
    
    sprintf(child->buffer, "done\n");
    ssize_t bytesWritten = write(child->pipe_fd_write[1], child->buffer, strlen(child->buffer));
    if (bytesWritten == -1) {
        perror("write");
        close(child->pipe_fd_write[1]); 
        exit(1);
    }
    //Clean the buffer
    memset(child->buffer, 0, sizeof(child->buffer));
}




 
void child_reads(Child *child){

    char buffer[60];

    //Read parent's message
    ssize_t bytesRead = read(child->pipe_fd_read[0], buffer, sizeof(buffer));
    if (bytesRead == -1) {
        perror("read");
        close(child->pipe_fd_read[0]); // Close the file descriptor in case of an error
        exit(1);
    }
    buffer[bytesRead]='\0';
    strcpy(child->lastMessage, buffer);
    printf("Child read: %s\n",buffer);
    printf("Child read: %s\n",child->lastMessage);
    //Clean the buffer
    memset(buffer, 0, sizeof(buffer));

}

void child_write_pipe(Child *child){
    
    char buffer[60];
    
        char buffer_1[60];
        sprintf(buffer_1, "First message to child<");
        sprintf(buffer_1 + strlen(buffer_1), "%d" , child->id);
        sprintf(buffer_1 + strlen(buffer_1) , "> \n");
        
        char buffer_2[60];
        sprintf(buffer_2, "Second message to child<");
        sprintf(buffer_2 + strlen(buffer_2), "%d" , child->id);
        sprintf(buffer_2 + strlen(buffer_2) , "> \n");

    sprintf(buffer, "done\n");
    if(strcmp(child->lastMessage, buffer_1 ) != 0){
        ssize_t bytesWritten = write(child->pipe_fd_write[1], buffer, strlen(buffer));
        if (bytesWritten == -1) {
            perror("write");
            close(child->pipe_fd_write[1]); 
            exit(1);
        }
        printf("Child writes: %s\n", buffer);
    }else {
        sprintf(buffer, "done_1\n");

        if(strcmp(child->lastMessage, buffer_2 ) != 0){
            ssize_t bytesWritten = write(child->pipe_fd_write[1], buffer, strlen(buffer));
            if (bytesWritten == -1) {
                perror("write");
                close(child->pipe_fd_write[1]); 
                exit(1);
            }
            printf("Child writes: %s\n", buffer);
        }
    }
    
    
    //Clean the buffer
    memset(buffer, 0, sizeof(buffer));
}

//child process
void child_process(Child *child,int fd){

    //Clean the buffer
    memset(child->lastMessage, 0, sizeof(child->lastMessage));

    // Parameters of select
    fd_set rfds; // For availabillity to read from
    //fd_set wfds; // For availability to write to
    struct timeval tv; 
    int retval;

    /* Wait up to five seconds. */
    tv.tv_sec = 5;
    tv.tv_usec = 0;

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
    child_reads_first_time(child);
    
    //Child process writes to the file
    child_write_file_first_time(child, fd);

    //Child process responds to parent
    child_write_pipe_first_time(child);

    sleep(2);
    // Parent-Child steady communication control
    while (1)
    {   
        sleep(1);
        /* Watch pipes_read (fd pipes_read[0]) to see when it has input. */
        FD_ZERO(&rfds);
        FD_SET(child->pipe_fd_read[0], &rfds);

        retval = select(FD_SETSIZE, &rfds, NULL, NULL, NULL);
        if (retval == -1)
            perror("select()");
        else if (retval){
            printf("Child<%d> can read the message from parent.\n",child->id);
            /* FD_ISSET(pipes_read[0], &rfds) will be true. */
            child_reads(child);
            child_write_pipe(child);
        }else
            printf("Child<%d> cannot read the message from parent.\n",child->id);

        
        sleep(1);
    }

    
    //Close all pipes and file descriptor
    //close(child->pipe_fd_read[0]);
    //close(child->pipe_fd_write[1]);
    //close(fd);

    //Child process terminates
    exit(0);
    
}


#endif //CHILD_H