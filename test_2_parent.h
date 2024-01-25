#include "test_2_list_f.h"
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>

//Parent's functions

//parent writes to file
void parent_writes_first_time(int fd,Parent *parent){
    
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

    memset(parent->lastSentToFile, 0, sizeof(parent->lastSentToFile));
    
    printf("Parent wrote to file: %s\n",strcpy(parent->lastSentToFile, buffer));

    //Clean the buffer
    memset(buffer, 0, 60);
}

//Send message to the child through pipes
void sent_message_to_child_first_time(Child * child, Parent *parent){    

    char buffer[60];

    sprintf(buffer, "Hello child, I am your father and i call you: <child ");
    sprintf(buffer + strlen(buffer), "%d" , child->id);
    sprintf(buffer + strlen(buffer) , "> \n");
    ssize_t bytesWritten =write(child->pipe_fd_read[1], buffer, sizeof(buffer));
    if (bytesWritten == -1) {
        perror("write");
        close(child->pipe_fd_read[1]); // Close the file descriptor in case of an error
        exit(1);
    }

    memset(parent->lastSentToPipe, 0, sizeof(parent->lastSentToPipe));
    
    printf("Parent wrote to child<%d>: %s\n",child->id, strcpy(parent->lastSentToPipe, buffer));

    //Clean the buffer
    memset(buffer, 0, sizeof(buffer));
}

/*//Parent waits for child's respone
void parent_reads_first_time(Child *child, char *buffer){

    ssize_t bytesRead = read(child->pipe_fd_write[0], buffer, sizeof(buffer));
    if (bytesRead == -1) {
        perror("read");
        close(child->pipe_fd_write[0]); // Close the file descriptor in case of an error
        exit(1);
    }
    //Clean the buffer
    memset(buffer, 0, sizeof(buffer));

}*/








//Parent control functions


void sent_message_to_child(Child *child, Parent *parent){

    /*printf("Parent before write -> lastMessage: %s\n",child->lastMessage);
    printf("Parent before write -> buffer: %s\n",child->buffer);
    printf("Parent before write -> id: %d\n",child->id);
    */

    char buffer[60];

    char child_buffer_1[60];
    sprintf(child_buffer_1, "done\n");

    char child_buffer_2[60];
    sprintf(child_buffer_2, "ok\n");

    if(strcmp(parent->lastMessage,child_buffer_1)==0){
        sprintf(buffer, "Received message from child<");
        sprintf(buffer + strlen(buffer), "%d" , child->id);
        sprintf(buffer + strlen(buffer) , ">");
        // Check if the new message is different from the last one
        //if (strcmp(buffer, child->lastMessage) != 0){
        ssize_t bytesWritten =write(child->pipe_fd_read[1], buffer, strlen(buffer));
        if (bytesWritten == -1) {
            perror("write");
            close(child->pipe_fd_read[1]); // Close the file descriptor in case of an error
            exit(1);
        }
    }else if(strcmp(parent->lastMessage,child_buffer_2)==0){
        sprintf(buffer, "Hello child, I am your father and i call you: <child ");
        sprintf(buffer + strlen(buffer), "%d" , child->id);
        sprintf(buffer + strlen(buffer) , "> \n");
        // Check if the new message is different from the last one
        //if (strcmp(buffer, child->lastMessage) != 0){
        ssize_t bytesWritten =write(child->pipe_fd_read[1], buffer, strlen(buffer));
        if (bytesWritten == -1) {
            perror("write");
            close(child->pipe_fd_read[1]); // Close the file descriptor in case of an error
            exit(1);
        }
    }

        memset(parent->lastSentToPipe, 0, sizeof(parent->lastSentToPipe));
        
        printf("Parent wrote to child<%d>: %s\n",child->id, strcpy(parent->lastSentToPipe, buffer));
        /*}else{
        //Clean the buffer
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "Second message to child<");
        sprintf(buffer + strlen(buffer), "%d" , child->id);
        sprintf(buffer + strlen(buffer) , ">");

        if (strcmp(buffer, child->lastMessage) != 0)
        {
            
            ssize_t bytesWritten =write(child->pipe_fd_read[1], buffer, strlen(buffer));
            if (bytesWritten == -1) {
                perror("write");
                close(child->pipe_fd_read[1]); // Close the file descriptor in case of an error
                exit(1);
            }
            printf("Parent writes: %s\n", buffer);
            printf("\n");
        
        }else{
        }
    }*/  
    
    
    //Clean the buffer
    memset(buffer, 0, sizeof(buffer));
    memset(child_buffer_1, 0, sizeof(child_buffer_1));
    memset(child_buffer_2, 0, sizeof(child_buffer_2));

}

void parent_reads(Child *child, Parent *parent){

    char buffer[60];

    ssize_t bytesRead = read(child->pipe_fd_write[0], buffer, sizeof(buffer));
    if (bytesRead == -1) {
        perror("read");
        close(child->pipe_fd_write[0]); // Close the file descriptor in case of an error
        exit(1);
    }
    
    buffer[bytesRead]='\0';
    memset(parent->lastMessage, 0, sizeof(parent->lastMessage));
    
    printf("parent read: %s from child<%d>\n",strcpy(parent->lastMessage,buffer),child->id);
    //printf("parent reads: %s from child<%d>\n",buffer, child->id);
    //ssize_t bytesWritten =write(1, buffer, sizeof(buffer));
    //Clean the buffer
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



void parent_child_control(int N, List *children, List_P *parents){
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
    while (1)
    {
        //sleep(1);
        
        for (int i = 0; i < N; i++)
        {
            
            Child *child  = search_child(children, i);
            Parent *parent  = search_parent(parents, i);

            
            sleep(1);
            /* Watch pipes_read (fd pipes_write[1]) to see when it has output. */
            FD_ZERO(&rfds);
            FD_SET(child->pipe_fd_write[0], &rfds);

            retval = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);
            /* Don’t rely on the value of tv now! */

            if (retval == -1)
                perror("select()");
            else if (retval){
                //printf("Parent can read from the child<%d>.\n",child->id);
                /* FD_ISSET(pipes_write[1], &wfds) will be true. */
                //Parent waits for child's respone
                parent_reads(child, parent);
            } else{
                //printf("Parent cannot read from the child<%d>.\n",child->id);
            }

            sleep(1);
            /* Watch pipes_read (fd pipes_read[1]) to see when it has output. */
            FD_ZERO(&wfds);
            FD_SET(child->pipe_fd_read[1], &wfds);

            retval = select(FD_SETSIZE, NULL, &wfds, NULL, &tv);
            /* Don’t rely on the value of tv now! */

            if (retval == -1)
                perror("select()");
            else if (retval){
                //printf("Parent can write to the child<%d>.\n",child->id);
                /* FD_ISSET(pipes_read[1], &wfds) will be true. */
                //Send message to the child through pipes
                sent_message_to_child(child, parent);
            }else{
                //printf("Parent cannot write from the child<%d>.\n",child->id);
            }
        }
    } 
}
