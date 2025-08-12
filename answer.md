QUESTIONS LAB 1:

• What is the macro corresponding to the syscall number in this case?
SYS_WRITE


What is the second parameter related to and what does it mean in this case?
File descriptor.

• Use GDB to print the string that the pointer in the third parameter refers to. 
"You got it, use your debugging skills during the labs!\n"


<<<<<<< HEAD
QUESTIIONS LAB 2:

=======
>>>>>>> e3bf86d (finished project)
4. Why can a user program not simply call functions in filesys directly instead of
performing system calls?

system calls are used to separate the user program from the underlying operating system,
providing abstraction and protection. Directly calling functions in the file system would
bypass the system call interface, breaking the abstraction and potentially compromising the
stability and security of the system. Additionally, system calls provide a standardized interface
for accessing the file system, which allows user programs to interact with the file system in a
consistent and well-defined manner, independent of the implementation details of the file system.

5. When a file is opened, a file id is returned to the user program, which is used to
refer to a specific opened file when doing file operations. Explain how to generate file identifiers and map them to struct file pointers that are created
in the kernel.

file identifiers (or file descriptors) are typically generated using a process-local file descriptor table.
The table is an array of pointers to struct file objects, with each entry representing a unique open file.
When a file is opened, the kernel selects an unused entry in the table, creates a new struct file object,
and initializes it with information about the opened file. The index of the selected entry in the table is
then returned as the file identifier.

When the user program performs file operations, it passes the file identifier to the kernel through a system
call. The kernel then looks up the corresponding entry in the file descriptor table and retrieves the
associated struct file object. The struct file object contains information about the file, such as its
position, flags, and underlying device, and is used by the kernel to perform the requested file operation.

The mapping of file identifiers to struct file objects is maintained by the kernel and is protected from
interference by user programs. This allows the kernel to ensure the integrity and consistency of the file
system, even in the presence of multiple concurrent file operations by different user programs.