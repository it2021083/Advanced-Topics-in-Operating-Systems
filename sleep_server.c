/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <string.h>
#include <sys/select.h>
#include <sys/time.h>

#include <signal.h>
#include <sys/signal.h>

#include "test_2_parent.h"
#include "test_2_child.h"

#include "sleep.h"

//Create a linked list for children.
	List *children ;
	//Create a linked list for parent's nodes
	List_P *parents ;


int *
init_1_svc(child_info *argp, struct svc_req *rqstp)
{
	static int  result;

	printf("Received INITILIZE request with %d number of children\n and \nfilename:%s\n", argp->num_children, argp->filename);

	//True Functionality

	pid_t fork_pid = -1; // Global

        //Made everything global so signal handlers can work

        /*//Create a linked list for children.
        List *children ;
        //Create a linked list for parent's nodes
        List_P *parents ;
        *///Number of child processes
        int N ;
        // File descriptor
        int fd ;

	//Open File
	const char *filename = argp->filename;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if(fd == -1){
		perror("File not found");
		exit(EXIT_FAILURE);
	} 
	
	//String to integer(Number of child processes)
	N = argp->num_children;

	//Create a linked list for children.
	children = create_child_list(N);
	printf("Created children list\n");
	
	//Create a linked list for parent's nodes
	parents = create_parent_list(N);
	printf("Created parent's nodes list\n");

	//Create the pipes of children
	edit_list(children->head);
	printf("All children piped\n");

	// Fork a child process
    for ( int j = 0; j < N; j++)
    {
        //Attach child process to child node
        Child *child  = search_child(children, j);

        //Choose the right parent node that corresponds to each child
        Parent *parent  = search_parent(parents, j);

        // Create the child processes
        fork_pid = fork();
        if (fork_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);

        }
        
        //Processes

        if (fork_pid > 0) {  // Parent process
            parent_process(&j, child, fd, parent); // Process that writes parent's pid to the file and asks for children's pids 
           
        } else {  // Child process
            child_process(child, fd ); // Process of each child: Write their pids to the file and send confirmation message to the parent
            
        }
    }

    //parent_child_control(N,children, parents,fd); // Process that controls the exchange of messages between parent and children


	return &result;
}

void *
sleep_1_svc(sleep_request *argp, struct svc_req *rqstp)
{
	static char * result;

	printf("Received SLEEP request with %d duration of sleep\n for \nchild:%d\n", argp->sleep_duration, argp->child_id);

	//True functionallity
	int sleep_duration;
	int child_id;

	sleep_duration = argp->sleep_duration;
	child_id = argp->child_id;

	//Attach child process to child node
	Child *child  = search_child(children, child_id);
	child->sleep_duration = sleep_duration;

	//Choose the right parent node that corresponds to each child
	Parent *parent  = search_parent(parents, child_id);
	
	sent_child_to_sleep(child, parent, sleep_duration);
	child_reads(child);
	child_write_pipe(child);

	return (void *) &result;
}

void *
store_1_svc(store_request *argp, struct svc_req *rqstp)
{
	static char * result;

	printf("Received STORE request with number_to_process: %d\n for \nchild: %d\n", argp->number_to_store, argp->child_id);

	/*
	 * insert server code here
	 */

	return (void *) &result;
}

add_result *
add_1_svc(void *argp, struct svc_req *rqstp)
{
	static add_result  result;

	printf("Received ADD request\n");

	/*
	 * insert server code here
	 */

	return &result;
}