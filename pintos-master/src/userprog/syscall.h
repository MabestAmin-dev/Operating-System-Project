#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

typedef int pid_t;

void syscall_init (void);
static void syscall_handler (struct intr_frame *f UNUSED); 
void halt (void);
bool create (const char *file, unsigned initial_size);
int open (const char *file);
void close (int fd);
int read (int fd, void *buffer, unsigned size);
int write (int fd, const void *buffer, unsigned size); 
void exit (int status);
int wait(pid_t pid);
pid_t exec (const char *cmd_line);
void seek (int fd, unsigned position);
unsigned tell(int fd);
int filesize(int fd);
bool remove (const char *file_name);
bool valid_fd(int fd);
bool valid_pointer(const void* ptr);
bool valid_string(char* string);
bool valid_buffer(void *buffer, const unsigned size);


#endif /* userprog/syscall.h */
