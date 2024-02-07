/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */
/*
#include "sleep.h"
#include <stdlib.h>

void sleep_prog_1(char *host, int procedure_type,int child_id,char *filename,int argument_4) {
    CLIENT *clnt;
    int *result_1;
    child_info init_1_arg;
    strncpy(init_1_arg.filename,filename,sizeof(init_1_arg.filename)-1);
    init_1_arg.filename[sizeof(init_1_arg.filename)-1] = '\0';
    //init_1_arg.filename = filename;
    init_1_arg.num_children = argument_4;
    void *result_2;
    sleep_request sleep_1_arg;
    sleep_1_arg.child_id = child_id;
    sleep_1_arg.sleep_duration = argument_4;
    void *result_3;
    store_request store_1_arg;
    store_1_arg.child_id = child_id;
    store_1_arg.number_to_store = argument_4;
    add_result *result_4;
    char *add_1_arg;

#ifndef DEBUG
    clnt = clnt_create(host, SLEEP_PROG, SLEEP_VERS, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }*/
//#endif /* DEBUG */

/*    switch (procedure_type) {
        case 1: // Initialization procedure
            // Set the initialization procedure arguments
            // (you need to fill init_1_arg with actual values)
            result_1 = init_1(&init_1_arg, clnt);
            break;

        case 2: // Sleep procedure
            // Set the sleep procedure arguments
            // (you need to fill sleep_1_arg with actual values)
            result_2 = sleep_1(&sleep_1_arg, clnt);
            break;

        case 3: // Store procedure
            // Set the store procedure arguments
            // (you need to fill store_1_arg with actual values)
            result_3 = store_1(&store_1_arg, clnt);
            break;

        case 4: // Add procedure
            // Set the add procedure arguments
            // (you need to fill add_1_arg with actual values)
            result_4 = add_1((void *)&add_1_arg, clnt);
            break;

        default:
            printf("Invalid procedure type\n");
            exit(1);
    }

#ifndef DEBUG
    clnt_destroy(clnt);
*///#endif /* DEBUG */
/*}

int main(int argc, char *argv[]) {
    char *host;

    if (argc < 3) {
		printf("1 -> Initialize\n2 -> Sleep\n3 -> Store\n4 -> Add\n\n");
        printf("Initialize => usage: %s server_host procedure_type filename num_children\n", argv[0]);
		printf("Sleep => usage: %s server_host procedure_type child_id sleep_duration\n", argv[0]);
		printf("Store => usage: %s server_host procedure_type child_id number\n", argv[0]);
		printf("Add => usage: %s server_host procedure_type\n", argv[0]);
        exit(1);
    }

    host = argv[1];
    int procedure_type = atoi(argv[2]);

	// Argument 3 can be the name of the file that will be used or the child_id depending the procedure
	int child_id = atoi(argv[3]);
	char *filename=argv[3];

	// Argument 4 can be either the number of the children or the sleep_duration or the number the child has to process
	int argument_4 = atoi(argv[4]);


    sleep_prog_1(host, procedure_type, child_id, filename, argument_4);

    exit(0);
}
*/

/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "sleep.h"
#include <stdlib.h>

void sleep_prog_1(char *host, int procedure_type, int child_id, char *filename, int argument_4) {
    CLIENT *clnt;
    int *result_1;
    child_info init_1_arg;
    strncpy(init_1_arg.filename, filename, sizeof(init_1_arg.filename) - 1);
    init_1_arg.filename[sizeof(init_1_arg.filename) - 1] = '\0';
    init_1_arg.num_children = argument_4;
    void *result_2;
    sleep_request sleep_1_arg;
    sleep_1_arg.child_id = child_id;
    sleep_1_arg.sleep_duration = argument_4;
    void *result_3;
    store_request store_1_arg;
    store_1_arg.child_id = child_id;
    store_1_arg.number_to_store = argument_4;
    add_result *result_4;
    char *add_1_arg;

#ifndef DEBUG
    clnt = clnt_create(host, SLEEP_PROG, SLEEP_VERS, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror(host);
        exit(1);
    }
#endif /* DEBUG */

    printf("Calling procedure type: %d\n", procedure_type);

    switch (procedure_type) {
        case 1: // Initialization procedure
            printf("Calling INITIALIZATION with %d number of children and filename: %s\n", init_1_arg.num_children, init_1_arg.filename);
            result_1 = init_1(&init_1_arg, clnt);
            break;

        case 2: // Sleep procedure
            printf("Calling SLEEP with %d duration of sleep for child: %d\n", sleep_1_arg.sleep_duration, sleep_1_arg.child_id);
            result_2 = sleep_1(&sleep_1_arg, clnt);
            break;

        case 3: // Store procedure
            printf("Calling STORE with number_to_process: %d for child: %d\n", store_1_arg.number_to_store, store_1_arg.child_id);
            result_3 = store_1(&store_1_arg, clnt);
            break;

        case 4: // Add procedure
            printf("Calling ADD procedure\n");
            result_4 = add_1((void *)&add_1_arg, clnt);
            break;

        default:
            printf("Invalid procedure type\n");
            exit(1);
    }

#ifndef DEBUG
    clnt_destroy(clnt);
#endif /* DEBUG */
}

int main(int argc, char *argv[]) {
    char *host;

    if (argc < 3) {
        printf("1 -> Initialize\n2 -> Sleep\n3 -> Store\n4 -> Add\n\n");
        printf("Initialize => usage: %s server_host procedure_type filename num_children\n", argv[0]);
        printf("Sleep => usage: %s server_host procedure_type child_id sleep_duration\n", argv[0]);
        printf("Store => usage: %s server_host procedure_type child_id number\n", argv[0]);
        printf("Add => usage: %s server_host procedure_type\n", argv[0]);
        exit(1);
    }

    host = argv[1];
    int procedure_type = atoi(argv[2]);

    // Argument 3 can be the name of the file that will be used or the child_id depending on the procedure
    int child_id = atoi(argv[3]);
    char *filename = argv[3];

    // Argument 4 can be either the number of children or the sleep_duration or the number the child has to process
    int argument_4 = atoi(argv[4]);

    sleep_prog_1(host, procedure_type, child_id, filename, argument_4);

    exit(0);
}