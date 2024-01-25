#ifndef CHILD_H
#define CHILD_H

#include "test_2_list_f.h"
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>

//Child's functions

/*
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
*/


//Child_write_file
void child_write_file_first_time(Child *child, int fd){

    sprintf(child->buffer, "<child %d> -> <child %d>\n", child->pid, child->id);
    ssize_t bytesWritten = write(fd, child->buffer, strlen(child->buffer));
    if (bytesWritten == -1) {
        perror("write");
        close(fd); // Close the file descriptor in case of an error
        exit(1);
    }

    memset(child->lastSent, 0, sizeof(child->lastSent));
    
    printf("Child wrote to file: %s\n", strcpy(child->lastSent, child->buffer));

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

    memset(child->lastSent, 0, sizeof(child->lastSent));
    
    printf("Child sent to parent: %s\n", strcpy(child->lastSent, child->buffer));

    //Clean the buffer
    memset(child->buffer, 0, sizeof(child->buffer));
}





//Child control functions

 
void child_reads(Child *child){

    //Clean the buffer
    memset(child->buffer, 0, sizeof(child->buffer));

    //Read parent's message
    ssize_t bytesRead = read(child->pipe_fd_read[0], child->buffer, sizeof(child->buffer));
    if (bytesRead == -1) {
        perror("read");
        close(child->pipe_fd_read[0]); // Close the file descriptor in case of an error
        exit(1);
    }
    child->buffer[bytesRead]='\0';
    //Clean the buffer
    memset(child->lastMessage, 0, sizeof(child->lastMessage));
    
    printf("Child read: %s\n",strcpy(child->lastMessage, child->buffer));
    
    //Clean the buffer
    memset(child->buffer, 0, sizeof(child->buffer));

}

/*//Child_write_file
void child_write_file(Child *child, int fd){

    //Buffer that contains the message of the parent to know what message to sent
    char buffer_1[60];
    sprintf(buffer_1, "First message to child<");
    sprintf(buffer_1 + strlen(buffer_1), "%d" , child->id);
    sprintf(buffer_1 + strlen(buffer_1) , ">");

    //Buffer that contains the message of the parent to know what message to sent
    char buffer_2[60];
    sprintf(buffer_2, "Received answer");
    

    // Compare the message child received with the first message parent sent him
    if(strcmp(child->lastMessage,buffer_1) == 0){
        sprintf(child->buffer, "done\n");
        ssize_t bytesWritten = write(fd, child->buffer, strlen(child->buffer));
        if (bytesWritten == -1) {
            perror("write");
            close(fd); // Close the file descriptor in case of an error
            exit(1);
        }
        //Clean the buffer
        memset(child->lastSent, 0, sizeof(child->lastSent));
        strcpy(child->buffer,child->lastSent); // Copy the message the child write to file to the lastSent message of child
        printf("Child wrote to file: %s\n", child->lastSent);
    }else {

    }

    
    //Clean the buffer
    memset(child->buffer, 0, sizeof(child->buffer));
    memset(buffer_1, 0, sizeof(buffer_1));
}*/

void child_write_pipe(Child *child){

    //printf("Child before write -> lastMessage: %s\n",child->lastMessage);
    
    //Clean the buffer
    memset(child->buffer, 0, sizeof(child->buffer));
    
    char buffer_1[60];
    sprintf(buffer_1, "Hello child, I am your father and i call you: <child ");
    sprintf(buffer_1 + strlen(buffer_1), "%d" , child->id);
    sprintf(buffer_1 + strlen(buffer_1) , "> \n");
    
    char buffer_2[60];
    sprintf(buffer_2, "Received message from child<");
    sprintf(buffer_2 + strlen(buffer_2), "%d" , child->id);
    sprintf(buffer_2 + strlen(buffer_2) , ">");

    if(strcmp(child->lastMessage,buffer_1) == 0){    
        sprintf(child->buffer, "done\n");
        //printf("%d\n",strcmp(child->lastMessage, buffer_1 ));
        //if(strcmp(child->lastMessage, buffer_1 ) == 0){
            
        ssize_t bytesWritten = write(child->pipe_fd_write[1], child->buffer, strlen(child->buffer));
        if (bytesWritten == -1) {
            perror("write");
            close(child->pipe_fd_write[1]); 
            exit(1);
        }
    } 
    else if(strcmp(child->lastMessage,buffer_2) == 0){    
        sprintf(child->buffer, "ok\n");
        //printf("%d\n",strcmp(child->lastMessage, buffer_1 ));
        //if(strcmp(child->lastMessage, buffer_1 ) == 0){
            
        ssize_t bytesWritten = write(child->pipe_fd_write[1], child->buffer, strlen(child->buffer));
        if (bytesWritten == -1) {
            perror("write");
            close(child->pipe_fd_write[1]); 
            exit(1);
        }
    }

    memset(child->lastSent, 0, sizeof(child->lastSent));
    
    printf("Child sent to parent: %s\n", strcpy(child->lastSent,child->buffer));
    
    /*}else {
        sprintf(child->buffer, "done_1\n");

        if(strcmp(child->lastMessage, buffer_2 ) == 0){
            ssize_t bytesWritten = write(child->pipe_fd_write[1], child->buffer, strlen(child->buffer));
            if (bytesWritten == -1) {
                perror("write");
                close(child->pipe_fd_write[1]); 
                exit(1);
            }
            printf("Child writes: %s\n", child->buffer);
        }
    }*/
    
    
    //Clean the buffer
    memset(child->buffer, 0, sizeof(child->buffer));
    memset(buffer_1, 0, sizeof(buffer_1));
    memset(buffer_2, 0, sizeof(buffer_2));
}

//child process
void child_process(Child *child,int fd){

    //Clean the buffer
    memset(child->lastMessage, 0, sizeof(child->lastMessage));

    // Parameters of select
    fd_set rfds; // For availabillity to read from
    fd_set wfds; // For availabillity to write to
    struct timeval tv; 
    int retval;
    int res;

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
    //child_reads_first_time(child);
    child_reads(child);
    
    //Child process writes to the file
    //child_write_file_first_time(child, fd);
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
            perror("select()_read");
        else if (retval){
            
            printf("Child<%d> can read the message from parent.\n",child->id);
            /* FD_ISSET(pipes_read[0], &rfds) will be true. */
            child_reads(child);
            
            FD_ZERO(&wfds);
            FD_SET(child->pipe_fd_write[1], &wfds);
            
            res = select(FD_SETSIZE, NULL, &wfds, NULL, &tv);
            if(res == -1){
                perror("select()_write");
            }
            else if(res){
                //child_write_file(child,fd);
                child_write_pipe(child);
            }
            else{
                printf("Child<%d> cannot write message to parent.\n",child->id);    
            }
        
        }else
            printf("Child<%d> cannot read the message from parent.\n",child->id);

        
        sleep(1);
    }

    //Child process terminates
    exit(0);
    
}


#endif //CHILD_H