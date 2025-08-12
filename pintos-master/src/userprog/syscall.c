#include <stdio.h>
#include "syscall-nr.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"
#include "syscall.h"
#include "filesys/filesys.h"
#include "devices/input.h"
#include "filesys/file.h"
#include "process.h"
#include "threads/vaddr.h"


#define MAX_OPEN_FILES 128
#define INVALID_FD -1


static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void syscall_handler (struct intr_frame *f UNUSED) {
  if(!valid_pointer(f->esp)){
    exit(-1);
  }

  void *arg0 = f->esp + 4;
  void *arg1 = f->esp + 8;
  void *arg2 = f->esp + 12;
  int syscall_number;
  memcpy(&syscall_number, f->esp, sizeof(int));
  switch(syscall_number) {
    case SYS_HALT:
      halt();
      break;
    case SYS_CREATE:
      if (!valid_pointer(arg0)) exit(-1);
      if (!valid_pointer(arg1)) exit(-1);
      f->eax = create((const char *) *(int *)(arg0), *(unsigned *)(arg1));
      break;
    case SYS_OPEN:
      if (!valid_pointer(arg0)) exit(-1);
      f->eax = open((const char *) *(int *)(arg0));
      break;
    case SYS_CLOSE:
      if (!valid_pointer(arg0)) exit(-1);
      close(*(int *)(arg0));
      break;
    case SYS_READ:
      if (!valid_pointer(arg0)) exit(-1);
      if (!valid_pointer(arg1)) exit(-1);
      if (!valid_pointer(arg2)) exit(-1);
      f->eax = read(*(int*)(arg0), *(void**)(arg1), *(unsigned *)(arg2));
      break;
    case SYS_WRITE:
      if (!valid_pointer(arg0)) exit(-1);
      if (!valid_pointer(arg1)) exit(-1);
      if (!valid_pointer(arg2)) exit(-1);
      f->eax = write(*(int*)(arg0), *(void**)(arg1), *(unsigned *)(arg2));
      break;
    case SYS_EXIT:
      if (!valid_pointer(arg0)) exit(-1);
      exit(*(int *)(arg0));
      break;
    case SYS_WAIT:
      if (!valid_pointer(arg0)) exit(-1);
      f->eax = wait(*(pid_t *)(arg0));
      break;
    case SYS_EXEC:
      if (!valid_pointer(arg0)) exit(-1);
      f->eax = exec((const char*) *(int*)(arg0));
      break;
    case SYS_TELL:
      if (!valid_pointer(arg0)) exit(-1);
      f->eax = tell(*(int*)arg0);
      break;
    
    case SYS_SEEK:
      if (!valid_pointer(arg0)) exit(-1);
      if (!valid_pointer(arg1)) exit(-1);
      seek(*(int*)(arg0),*(unsigned *)(arg1));
      break;
    case SYS_FILESIZE:
      if (!valid_pointer(arg0)) exit(-1);
      f->eax = filesize(*(int*)arg0);
      break;
    case SYS_REMOVE:
      if (!valid_pointer(arg0)) exit(-1);
      f->eax = remove(*(char**)arg0);
    default:
      break;
  }
}


void
halt (void){
  power_off();
}

bool
create (const char *file, unsigned initial_size){
  if(!valid_string(file)){
    exit(-1);
  }
  return filesys_create(file, initial_size);
}

int
open (const char *name){
  if(!valid_string(name)){
    exit(-1);
  }
  struct file *file = filesys_open(name);
  struct thread * thread = thread_current ();
  if (file == NULL){
    return -1;
  }
  //check if the number of files exceeds the limit of 128, checks all the entries
  int fd;
  for (fd = 2; fd < MAX_OPEN_FILES + 2; fd++) {
    if (thread->fd_list[fd] == NULL){
      thread->fd_list[fd] = file;
      return fd;
    }
  }  // if no entry was available return -1
  return -1;
}

void 
close (int fd){
  if(!valid_fd(fd)){
    exit(-1);
  }
  struct thread * current_thread = thread_current();
  struct file* current_file = thread_current()->fd_list[fd];
  if(current_file != NULL){
    file_close(current_file);
    current_thread->fd_list[fd] = NULL;
  }
}

int 
read (int fd, void *buffer, unsigned size){
  //checks if keyboard input
  if (!valid_buffer(buffer, size)){
    exit(-1);
  }
  if(fd == 1) return -1;

  if (fd == 0){
    for (unsigned i = 0; i < size; i++){
      *((uint8_t*)buffer) = input_getc();
      buffer++;
    }
    return size;
  }
  else if (2 <= fd && fd <= MAX_OPEN_FILES+2){
    if(!valid_fd(fd)) return -1;
    struct thread * thread = thread_current ();
    struct file *file = thread->fd_list[fd];
    if (file == NULL){
      return -1;
    }
    int bytes_read = (int)file_read(file, buffer, size);
    if(bytes_read >= 0){
      return bytes_read;
    }
    else{
      return -1;    
    }
  }
  return -1;
}

int write (int fd, const void *buffer, unsigned size) {
  //printf("WRITE");
  if (!valid_buffer(buffer, size)){
    exit(-1);
  }
  if(fd == 0) return -1;

  unsigned i;
  if (fd == 1) { // writes to the console
    putbuf(buffer, size);
    return size;
  } 
  else if (2 <= fd && fd <= MAX_OPEN_FILES+2) { 
    if(!valid_fd(fd)) return -1;
    struct thread *current_thread = thread_current();
    struct file *file = current_thread->fd_list[fd];

    if (file == NULL) {
      return -1;
    }

    int bytes_written = (int)file_write(file, buffer, size); // write to the file
    if (bytes_written >= 0) {
      return bytes_written;
    } 
    else {
      return -1;
    }
  }
  return -1;
}


pid_t 
exec (const char *cmd_line) {
  if(!valid_string(cmd_line)) exit(-1);
  return (pid_t) process_execute(cmd_line);
}

int 
wait(pid_t pid){
  if (pid == -1) {
    exit(-1);
  }

  return process_wait(pid);
}

void exit (int status){
  thread_current()->parentRealtion->exit_status = status;
  thread_exit();
}

void 
seek (int fd, unsigned position){
  if(!valid_fd(fd)) exit(-1);
  
  if(2<=fd && fd <= MAX_OPEN_FILES+2){
    struct file *seek_file = thread_current()->fd_list[fd];
    if(seek_file != NULL){
      if(file_length(seek_file) <= position){
        file_seek(seek_file, file_length(seek_file)-1); //sets the file pos to a new pos, file_length(seek_file)- = newpos
      }else{
      file_seek(seek_file, position);
      }
    }
  }
}

unsigned tell(int fd){
  if(!valid_fd(fd))return -1;

  if(2 <= fd && fd <= MAX_OPEN_FILES+2){
    struct file *tell_file = thread_current()->fd_list[fd];
    if(tell_file != NULL)
    return file_tell(tell_file);
  }
  return 0;
}

int filesize(int fd){
  if(!valid_fd(fd))return -1;
  if(2 <= fd && fd <= 130){

    struct file *file = thread_current()->fd_list[fd];
    if(file != NULL)
    return(file_length(file));
  }
}

bool remove (const char *file_name){
  if(!valid_string(file_name)) exit(-1);
  return filesys_remove(file_name);
}

bool
valid_pointer(const void* pointer){
   return ((pointer != NULL) &&
	  is_user_vaddr(pointer) &&               
	  (pagedir_get_page(thread_current()->pagedir, pointer) != NULL));
}

//lägg in dessa i h filen
bool
valid_string(char* string){
  if(!valid_pointer(string)){
    return false;
  }
  while(*string != '\0'){
    if(!valid_pointer(string)){
      return false;
    }
    string++;
  }
}

bool valid_fd(int fd)
{
  return ((fd < 130) && (fd > 1));
}

bool valid_buffer(void *buffer, const unsigned size){
  if(!valid_pointer(buffer))return false;

  for(unsigned i = 0; i < size; i++) {
    if(!valid_pointer(buffer)) return false;

    buffer++;
  }
  return true;
}