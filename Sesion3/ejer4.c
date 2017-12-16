#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>

int main(){
  int i, estado;
  pid_t PID;
  //creamos los hijos
  for(i=0; i<5; i++){
    if((PID = fork())<0){
      perror("Error en fork\n");
      exit(-1);
    }
    if(PID==0){
      printf("Soy el hijo PID = %i\n", getpid());
      exit(0);
    }
  }
  //esperamos a los hijos
  for(i=4; i>=0; i--){
    PID = wait(&estado);
    printf("Ha finalizado el hijo con PID = %i\n", PID);
    printf("Quedan %i hijos vivos\n", i);
  }
}
