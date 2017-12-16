#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>

int main(){
  int i, estado;
  const int hijos = 6;
  int vivos = 6;

  int pids[5];
  //creamos los hijos
  for(i=0; i<hijos; i++){
    if((pids[i] = fork())<0){
      perror("Error en fork\n");
      exit(-1);
    }
    if(pids[i] ==0){
      printf("Soy el hijo PID = %i\n", getpid());
      exit(0);
    }
  }
  //esperamos a los hijos
  for(i=0; i<hijos; i+=2){
    waitpid(pids[i],&estado);
    printf("Ha finalizado el hijo con PID = %i\n", pids[i]);
    printf("Quedan %i hijos vivos\n", vivos--);
  }

  for(i=1; i<hijos; i+=2){
    waitpid(pids[i],&estado);
    printf("Ha finalizado el hijo con PID = %i\n", pids[i]);
    printf("Quedan %i hijos vivos\n", vivos--);
  }

  exit(0);
}
