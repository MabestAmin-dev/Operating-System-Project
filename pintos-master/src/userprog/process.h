#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

struct parent_child
{
   tid_t child_tid;
   int exit_status;
   int alive_count;
   struct semaphore threadSema;
   struct semaphore waitSema;
   struct lock count_lock;
   struct list_elem parent_child_elem;
   char *file_name1;

};

#endif /* userprog/process.h */
