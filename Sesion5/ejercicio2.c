#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>

int ordenes[31] = {0};

void cando(int sig){
  //Modifica el contador
  ordenes[sig] = ordenes[sig] +1;
  printf("la señal %d se ha recibido %d veces.\n", sig, ordenes[sig]);

  //Ignora la señal que recibe
  signal(sig, SIG_IGN);
}

int main(){
  printf("Version %f", 1.00);

  //Crea una nueva acción
  struct sigaction sigCan;

  //Elige la función que la va a manejar
  sigCan.sa_handler = cando;

  //Nos aseguramos de que recibimos todas las señales
  //No habrá ninguna a la que se le aplique una mascara
  sigemptyset(&sigCan.sa_mask);
  

  while(1){
    for(int i = 0; i <= 31; i++){
      sigaction(i, &sigCan, NULL);
    }
  }
  
}
