#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>

int main(){

  sigset_t mask;

  //Creamos una mascara con todas las señales
  sigfillset(&mask);

  //Borramos la señal que queremos que desbloquee
  sigdelset(&mask, SIGUSR1);

  //Suspendemos la ejecución del programa
  sigsuspend(&mask);
}
