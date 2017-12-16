#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>

int main(){
  int i;
  pid_t childpid;
  int nprocs = 20;
  for (i=1; i < nprocs; i++) {
    if ((childpid= fork()) == -1) {
      fprintf(stderr, "Could not create child %d: %s\n",i,strerror(errno));
      exit(-1);
    }
    if (childpid){
      printf("Hijo con PID = %d padres %d\n", getpid(), getppid());
      break;
    }
    //printf("Hijo con PID = %d padres %d\n", getpid(), getppid());
  }

  exit(0);
}
