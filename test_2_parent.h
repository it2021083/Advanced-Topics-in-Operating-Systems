#include "test_2_list_f.h"
#include <string.h>

//Parent's functions

//parent process
void parent_process(int *i, Child *child,int fd){

    //Pass the for counter to the subfunctions
    int j = *i;

    //Close read pointer of the one pipe and write pointer from the other pipe
    close(child->pipe_fd_read[0]);
    close(child->pipe_fd_write[1]);
    
    //Parent's buffer
    char buffer[60];

    //Clean the buffer
    memset(buffer, 0, sizeof(buffer));

    if(*i == 0){
        parent_writes(fd, buffer);
    }
            
    //Send message to the child through pipes
    sent_message_to_child(buffer,child,&j);

    //Parent waits for child's respone
    parent_reads(child, buffer);

    //Close read pointer of the one pipe and write pointer from the other pipe
    close(child->pipe_fd_read[1]);
    close(child->pipe_fd_write[0]);
    memset(buffer, 0, sizeof(buffer));

}

//parent writes to file
void parent_writes(int fd, char *buffer){
    
    pid_t ppid = getpid();

    //Parent writes his pid in file
    sprintf(buffer, "[parent] -> %d\n", ppid);
    ssize_t bytesWritten = write(fd,buffer,strlen(buffer));
    if (bytesWritten == -1) {
        perror("write");
        close(fd); // Close the file descriptor in case of an error
        exit(1);
    }
    //Clean the buffer
    memset(buffer, 0, 60);
}

//Send message to the child through pipes
void sent_message_to_child(char *buffer,Child * child,int *j){    

    sprintf(buffer, "Hello child, I am your father and i call you: <child ");
    sprintf(buffer + strlen(buffer), "%d" , *j);
    sprintf(buffer + strlen(buffer) , "> \n");
    ssize_t bytesWritten =write(child->pipe_fd_read[1], buffer, sizeof(buffer));
    if (bytesWritten == -1) {
        perror("write");
        close(child->pipe_fd_read[1]); // Close the file descriptor in case of an error
        exit(1);
    }
    //Clean the buffer
    memset(buffer, 0, sizeof(buffer));
}

//Parent waits for child's respone
void parent_reads(Child *child, char *buffer){

    ssize_t bytesRead = read(child->pipe_fd_write[0], buffer, sizeof(buffer));
    if (bytesRead == -1) {
        perror("read");
        close(child->pipe_fd_write[0]); // Close the file descriptor in case of an error
        exit(1);
    }
    //Clean the buffer
    memset(buffer, 0, sizeof(buffer));
}
