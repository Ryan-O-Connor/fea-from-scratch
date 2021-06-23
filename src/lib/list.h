/*
 * Dynamically resizing array
 * implementation of a list.
 * Similar to a python list.
 */

struct list{
  void** array;
  int nitems;
  int array_size;
};

struct list* new_list();
void append(struct list* l, void* item);
void* get(struct list* l, int i);
void free_items(struct list* l, void (*free_item)(void*));
void free_list(struct list* l);

