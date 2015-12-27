#include<stdio.h>
#include<sys/types.h>
#include<sys/inotify.h>


#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUFFER (1024 *(EVENT_SIZE + 16))

int main(int argv,char **argc)	// Command line argument.
{
  int length,wd,fd,i=0;
  char buffer[BUFFER];
  fd=inotify_init();	//Creating inotify instance
  if(fd<0)
  {
    printf("Error creating file descriptor.");
  }
  //inotify_add_watch is used to add the path of the directory which needs to be monitored
  wd=inotify_add_watch(fd,"reboot/files",IN_MODIFY);
  //length stores the number of bytes read by the system call
  length=read(fd,buffer,BUFFER);
  if(length<0)
  {
    printf("Error............");
  }
  /*This is the most crucial part of the program which takes the actual action on any modification in the file.
  Pointer of structure type is pointed to buffer array.*/
  while(i<length)			
  {
    struct inotify_event *task=(struct inotify_event *) &buffer[i];
    if(task->len)
    {
      if(task->mask & IN_MODIFY)
      {
        /*system is used to pass the command to the shell when invoked*/
        system("init 6");
      }
    }
  i+=EVENT_SIZE+task->len;
  }
  inotify_rm_watch(fd,wd);
  close(fd);
  return 0;
}
