typedef struct {
    int value;
    int key;
    struct hlist_node next;
} kvs_entry_t;

void run_tests(void);

void put(int value, int key);

int get(int key);
