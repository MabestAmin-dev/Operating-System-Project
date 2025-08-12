
struct list_item
{
    int value;
    struct list_item *next;
};

void append( struct list_item *first, int x);
void prepend(struct list_item *first, int x);
void print(struct list_item *first);
void input_sorted(struct list_item *first, int x);
void clear(struct list_item *first);

