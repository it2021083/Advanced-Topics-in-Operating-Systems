#include <sys/types.h>

//Data structures
    //Child/Node struct
    typedef struct Child
    {
        int id; // Id of child process
        pid_t pid; // PID of child process
        int pipe_fd_write[2]; // Child write to this pipe
        int pipe_fd_read[2];    // Child reads from this pipe
        char buffer[60];    // Buffer for temporarily storing the data(message to/from parent and to the file)
        char lastMessage[60];   // Buffer that holds the last received message from parent 
        char lastSent[60];  // Buffer that holds the last message the child sended to the parent(and written to the file)
        struct Child * next; // Pointer to the next child of the children list
    }Child;

    //Parent struct
    typedef struct Parent
    {
        int id; // Id of parent node that must be the same to the corresponding id of a child process
        char lastSentToPipe[60];    // Buffer that holds the last message parent sent to the child
        char lastSentToFile[60];    // Buffer that holds the last data he wrote to the file
        char lastMessage[60];   // Buffer that holds the last received 
        struct Parent * next;   // Pointer to the next parent node of the parent list
    }Parent;

    //List struct
    typedef struct list {
        int size; // Size of the list
        Child* head;    // Head node of the children list
    } List;

    //List struct
    typedef struct list_p {
        int size;   // Size of the parent's nodes list
        Parent* head;   // Parent node of the parent's nodes list
    } List_P;