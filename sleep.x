struct child_info {
    int num_children;
    char filename[256]; 
};

struct sleep_request {
    int child_id;
    int sleep_duration;
};

struct store_request {
    int child_id;
    int number_to_store;
};

struct add_result {
    int sum;
};

program SLEEP_PROG {
    version SLEEP_VERS {
        int init(child_info) = 1;
        void sleep(sleep_request) = 2;
        void store(store_request) = 3;
        add_result add() = 4;
    } = 1;
} = 0x31599123;

