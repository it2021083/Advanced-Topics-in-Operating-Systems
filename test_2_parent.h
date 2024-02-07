#include "test_2_list_f.h"
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>

#include <signal.h>

//Parent's functions

//parent writes to file
void parent_writes_first_time(int fd,Parent *parent){
    
    // Create buffer for the text the parent will write to the file
    char buffer[60];

    pid_t ppid = getpid();

    //Parent writes his pid in file
    sprintf(buffer, "[parent] -> %d\n", ppid);
    ssize_t bytesWritten = write(fd,buffer,strlen(buffer));
    if (bytesWritten == -1) {
        perror("write");
        close(fd); // Close the file descriptor in case of an error
        exit(1);
    }

    // Clear the buffer of parent that stores what he writes to file
    memset(parent->lastSentToFile, 0, sizeof(parent->lastSentToFile));
    
    //Log what parent did to the console
    printf("Parent wrote to file: %s\n",strcpy(parent->lastSentToFile, buffer));

    //Clean the buffer
    memset(buffer, 0, 60);
}

//Send message to the child through pipes
void sent_message_to_child_first_time(Child * child, Parent *parent){    

    // Create buffer for the message parent will send to the children
    char buffer[60];

    // Parent sends message to children
    sprintf(buffer, "Hello child, I am your father and i call you: <child ");
    sprintf(buffer + strlen(buffer), "%d" , child->id);
    sprintf(buffer + strlen(buffer) , "> \n");
    ssize_t bytesWritten =write(child->pipe_fd_read[1], buffer, sizeof(buffer));
    if (bytesWritten == -1) {
        perror("write");
        close(child->pipe_fd_read[1]); // Close the file descriptor in case of an error
        exit(1);
    }

    // Clear the buffer of parent that stores what parent has sent to pipe
    memset(parent->lastSentToPipe, 0, sizeof(parent->lastSentToPipe));
    
    //Log what parent writes to the console
    printf("Parent wrote to child<%d>: %s\n",child->id, strcpy(parent->lastSentToPipe, buffer));

    //Clean the buffer
    memset(buffer, 0, sizeof(buffer));
}








//Parent control functions

//Send child to sleep
void sent_child_to_sleep(Child *child, Parent *parent, int sleep_duration){
    
    // Create buffer for the message parent will send to the children
    char buffer[60];

    // Parent sends message to children
    sprintf(buffer, "Hello <child ");
    sprintf(buffer + strlen(buffer), "%d" , child->id);
    sprintf(buffer + strlen(buffer) , "> go to sleep for %d\n", sleep_duration);

    ssize_t bytesWritten =write(child->pipe_fd_read[1], buffer, sizeof(buffer));
    if (bytesWritten == -1) {
        perror("write");
        close(child->pipe_fd_read[1]); // Close the file descriptor in case of an error
        exit(1);
    }

    // Clear the buffer of parent that stores what parent has sent to pipe
    memset(parent->lastSentToPipe, 0, sizeof(parent->lastSentToPipe));
    
    //Log what parent writes to the console
    printf("Parent wrote to child<%d>: %s\n",child->id, strcpy(parent->lastSentToPipe, buffer));

    //Clean the buffer
    memset(buffer, 0, sizeof(buffer));

}

//Send message to the child through pipes
void sent_message_to_child(Child *child, Parent *parent){

    // Create buffer for the message parent will send to the children
    char buffer[60];

    // Buffer that contains 1 of the messages from child
    char child_buffer_1[60];
    sprintf(child_buffer_1, "done\n");

    // Buffer that contains the 2nd of the messages from child
    char child_buffer_2[60];
    sprintf(child_buffer_2, "ok\n");

    // Checks what is the last received message from children 
    if(strcmp(parent->lastMessage,child_buffer_1)==0){
        // Fill the buffer to send a message to child
        sprintf(buffer, "Received message from child<");
        sprintf(buffer + strlen(buffer), "%d" , child->id);
        sprintf(buffer + strlen(buffer) , ">");
        // Sends the message to the child through pipe
        ssize_t bytesWritten =write(child->pipe_fd_read[1], buffer, strlen(buffer));
        if (bytesWritten == -1) {
            perror("write");
            close(child->pipe_fd_read[1]); // Close the file descriptor in case of an error
            exit(1);
        }
    }else if(strcmp(parent->lastMessage,child_buffer_2)==0){
        // Fill the buffer to send a message to child
        sprintf(buffer, "Hello child, I am your father and i call you: <child ");
        sprintf(buffer + strlen(buffer), "%d" , child->id);
        sprintf(buffer + strlen(buffer) , "> \n");
        // Sends the message to the child through pipe
        ssize_t bytesWritten =write(child->pipe_fd_read[1], buffer, strlen(buffer));
        if (bytesWritten == -1) {
            perror("write");
            close(child->pipe_fd_read[1]); // Close the file descriptor in case of an error
            exit(1);
        }
    }
    // Clear the buffer that stores what parent send to pipes
    memset(parent->lastSentToPipe, 0, sizeof(parent->lastSentToPipe));
    
    //Log what parent writes to the console
    printf("Parent wrote to child<%d>: %s\n",child->id, strcpy(parent->lastSentToPipe, buffer));

    //Clear all the buffers
    memset(buffer, 0, sizeof(buffer));
    memset(child_buffer_1, 0, sizeof(child_buffer_1));
    memset(child_buffer_2, 0, sizeof(child_buffer_2));

}

// Parent reads from pipes
void parent_reads(Child *child, Parent *parent){

    // Buffer that will have the message received
    char buffer[60];

    // Parent reads the message from pipe, stores it at the buffer
    ssize_t bytesRead = read(child->pipe_fd_write[0], buffer, sizeof(buffer));
    if (bytesRead == -1) {
        perror("read");
        close(child->pipe_fd_write[0]); // Close the file descriptor in case of an error
        exit(1);
    }
    
    // Put the end of string character at the last received byte
    buffer[bytesRead]='\0';
    
    // Clear the buffer that holds the last received message from child
    memset(parent->lastMessage, 0, sizeof(parent->lastMessage));
    
    // Log the message parent received and store it to the the right buffer
    printf("parent read: %s from child<%d>\n",strcpy(parent->lastMessage,buffer),child->id);
    
    // Clear the buffer that was used from parent to write what he read from pipe
    memset(buffer, 0, sizeof(buffer));

}








//parent process
void parent_process(int *i, Child *child,int fd, Parent *parent){


    //Close read pointer of the one pipe and write pointer from the other pipe
    close(child->pipe_fd_read[0]);
    close(child->pipe_fd_write[1]);
    


    if(*i == 0){
        parent_writes_first_time(fd, parent);
    }
            
    //Send message to the child through pipes
    sent_message_to_child_first_time(child, parent);

    //Parent waits for child's respone
    parent_reads(child, parent);


}


void parent_child_control(int N, List *children, List_P *parents, int fd){
    // For select control


    // Parameters of select
    fd_set rfds; // For availabillity to read from
    fd_set wfds; // For availability to write to
    struct timeval tv; 
    int retval;
    /* Wait up to five seconds. */
    tv.tv_sec = 5;
    tv.tv_usec = 0;


    //Steady communication between father and children
    while(1){
        sleep(1);
        
        for (int i = 0; i < N; i++)
        {
            
            Child *child  = search_child(children, i);
            Parent *parent  = search_parent(parents, i);

            
            /* Watch pipes_read (fd pipes_write[1]) to see when it has output. */
            FD_ZERO(&rfds);
            FD_SET(child->pipe_fd_write[0], &rfds);

            retval = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);
            /* Don’t rely on the value of tv now! */

            if (retval == -1)
                perror("select()");
            else if (retval){
                
                //Parent waits for child's respone
                //parent_reads(child, parent);
            } else{
                //printf("Parent cannot read the message from child<%d>.\n",child->id);
            }

            /* Watch pipes_read (fd pipes_read[1]) to see when it has output. */
            FD_ZERO(&wfds);
            FD_SET(child->pipe_fd_read[1], &wfds);

            retval = select(FD_SETSIZE, NULL, &wfds, NULL, &tv);
            /* Don’t rely on the value of tv now! */

            if (retval == -1)
                perror("select()");
            else if (retval){
                
                //Send message to the child through pipes
                //sent_message_to_child(child, parent);
            }else{
                //printf("Parent cannot write a message to child<%d>.\n",child->id);
            }
        }
    } 
}
