#include <sys/types.h>

//Data structures
    //Child/Node struct
    typedef struct Child
    {
        int id;
        pid_t pid;
        int pipe_fd_write[2];
        int pipe_fd_read[2];
        char buffer[60];
        char lastMessage[60];
        char lastSent[60];
        struct Child * next;
    }Child;

    //Parent struct
    typedef struct Parent
    {
        int id;
        char lastSentToPipe[60];
        char lastSentToFile[60];
        char lastMessage[60];
        struct Parent * next;
    }Parent;

    //List struct
    typedef struct list {
        int size;
        Child* head;
    } List;

    //List struct
    typedef struct list_p {
        int size;
        Parent* head;
    } List_P;