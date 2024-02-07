#ifndef CHILD_H
#define CHILD_H

#include "test_2_list_f.h"
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>

#include <signal.h>

//Child's functions

//Child_write_file
void child_write_file_first_time(Child *child, int fd){

    // Child writes its pid and id to the file
    sprintf(child->buffer, "<child %d> -> <child %d>\n", child->pid, child->id);
    ssize_t bytesWritten = write(fd, child->buffer, strlen(child->buffer));
    if (bytesWritten == -1) {
        perror("write");
        close(fd); // Close the file descriptor in case of an error
        exit(1);
    }

    // Clear the buffer of parent that stores what the child sends to file/parent
    memset(child->lastSent, 0, sizeof(child->lastSent));
    
    //Log what child did to the console
    printf("Child<%d> wrote to file: %s\n",child->id, strcpy(child->lastSent, child->buffer));

    // Clear the temporal buffer of children 
    memset(child->buffer, 0, sizeof(child->buffer));
}


//Child sends message to pipe
void child_write_pipe_first_time(Child *child){
    
    // Child sends message to parent
    sprintf(child->buffer, "done\n");
    ssize_t bytesWritten = write(child->pipe_fd_write[1], child->buffer, strlen(child->buffer));
    if (bytesWritten == -1) {
        perror("write");
        close(child->pipe_fd_write[1]); 
        exit(1);
    }

    // Clear the buffer of parent that stores what the child sends to file/parent
    memset(child->lastSent, 0, sizeof(child->lastSent));
    
    //Log what child did to the console
    printf("Child<%d> sent to parent: %s\n",child->id, strcpy(child->lastSent, child->buffer));

    // Clear the temporal buffer of children 
    memset(child->buffer, 0, sizeof(child->buffer));
}





//Child control functions

// Child reads from the pipe
void child_reads(Child *child){

    //Ensure that the temporal child's buffer is clear
    memset(child->buffer, 0, sizeof(child->buffer));

    //Read parent's message and store it to the temporal buffer
    ssize_t bytesRead = read(child->pipe_fd_read[0], child->buffer, sizeof(child->buffer));
    if (bytesRead == -1) {
        perror("read");
        close(child->pipe_fd_read[0]); // Close the file descriptor in case of an error
        exit(1);
    }

    // Put the end of string character after the last received byte
    child->buffer[bytesRead]='\0';
    
    // Clear the buffer that holds the last received message from parent
    memset(child->lastMessage, 0, sizeof(child->lastMessage));
    
    //Log what child did to the console
    printf("Child<%d> read: %s\n", child->id, strcpy(child->lastMessage, child->buffer));
    
    // Clear the temporal buffer of children 
    memset(child->buffer, 0, sizeof(child->buffer));

}

//Send message to the parent through pipes
void child_write_pipe(Child *child){

    //Ensure that the temporal child's buffer is clear
    memset(child->buffer, 0, sizeof(child->buffer));
    
    // Buffer that contains 1 of the messages from parent
    char buffer_1[60];
    sprintf(buffer_1, "Hello child, I am your father and i call you: <child ");
    sprintf(buffer_1 + strlen(buffer_1), "%d" , child->id);
    sprintf(buffer_1 + strlen(buffer_1) , "> \n");
    
    // Buffer that contains the 2nd of the messages from parent
    char buffer_2[60];
    sprintf(buffer_2, "Received message from child<");
    sprintf(buffer_2 + strlen(buffer_2), "%d" , child->id);
    sprintf(buffer_2 + strlen(buffer_2) , ">");

    // Buffer that contains the 23rd of the messages from parent
    char buffer_3[60];
    sprintf(buffer_3, "Hello <child ");
    sprintf(buffer_3 + strlen(buffer_3), "%d" , child->id);
    sprintf(buffer_3 + strlen(buffer_3) , "> go to sleep for %d\n", child->sleep_duration);

    // Checks what is the last received message from parent 
    if(strcmp(child->lastMessage,buffer_1) == 0){    
        // Fill the temporal buffer to send a message to parent
        sprintf(child->buffer, "done\n");
        // Sends the message to the parent through pipe
        ssize_t bytesWritten = write(child->pipe_fd_write[1], child->buffer, strlen(child->buffer));
        if (bytesWritten == -1) {
            perror("write");
            close(child->pipe_fd_write[1]); 
            exit(1);
        }
    } 
    else if(strcmp(child->lastMessage,buffer_2) == 0){    
        sprintf(child->buffer, "ok\n");
          
        ssize_t bytesWritten = write(child->pipe_fd_write[1], child->buffer, strlen(child->buffer));
        if (bytesWritten == -1) {
            perror("write");
            close(child->pipe_fd_write[1]); 
            exit(1);
        }
    }
    else if(strcmp(child->lastMessage,buffer_2) == 0){    
        /*implementation  of sleep*/
        printf("I am child< %d > and i am going to sleep for %d \n", child->id, child->sleep_duration);
        sleep(child->sleep_duration); 
        printf("Heyy, i am child< %d > and i woke up\n", child->id);
        
    }

    // Clear the buffer that stores what chuld sends to pipes
    memset(child->lastSent, 0, sizeof(child->lastSent));
    
    //Log what child did to the console
    //printf("Child<%d> sent to parent: %s\n",child->id, strcpy(child->lastSent,child->buffer));
    
    
    //Clear all the buffers
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

    // Parent-Child steady communication control
    while (1)
    {   
        /* Watch pipes_read (fd pipes_read[0]) to see when it has input. */
        FD_ZERO(&rfds);
        FD_SET(child->pipe_fd_read[0], &rfds);

        retval = select(FD_SETSIZE, &rfds, NULL, NULL, NULL);
        if (retval == -1)
            perror("select()_read");
        else if (retval){
            
            //printf("Child<%d> can read the message from parent.\n",child->id);
            /* FD_ISSET(pipes_read[0], &rfds) will be true. */
            //child_reads(child);
            
            FD_ZERO(&wfds);
            FD_SET(child->pipe_fd_write[1], &wfds);
            
            res = select(FD_SETSIZE, NULL, &wfds, NULL, &tv);
            if(res == -1){
                perror("select()_write");
            }
            else if(res){
                child_write_pipe(child);
            }
            else{
                //printf("Child<%d> cannot write message to parent.\n",child->id);    
            }
        
        }else{
           //printf("Child<%d> cannot read the message from parent.\n",child->id);
        }
        
    }

    //Child process terminates
    exit(0);
    
}


#endif //CHILD_H