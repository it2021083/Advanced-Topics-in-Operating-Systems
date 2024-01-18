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
        struct Child * next;
    }Child;
    //List struct
    typedef struct list {
        int size;
        Child* head;
    } List;