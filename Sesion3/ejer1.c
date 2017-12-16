/*Ejercicio 1. Implementa un programa en C que tenga como argumento un número entero. Este
programa debe crear un proceso hijo que se encargará de comprobar si dicho número es un
número par o impar e informará al usuario con un mensaje que se enviará por la salida estándar.
A su vez, el proceso padre comprobará si dicho número es divisible por 4, e informará si lo es o
no usando igualmente la salida estándar.*/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]){


  if (argc != 2){
    perror("\n Número invalido de argumentos.");
    exit(-1);
  }

  pid_t pid;
  int number = atoi(argv[1]);

  pid = fork();
    
  if(pid < 0){
    perror("\n Error al crear el proceso hijo");

  }else if (pid == 0){

    //Estamos en el proceso hijo
    printf("\nProceso hijo de %d con pid %d ", getppid(), getpid());
    
    if (number % 2 == 0)
      printf("\nEl número introducido es par");
    else
      printf("\nEl número introducido es impar");
    

  }else{
    printf("\nProceso padre de %d con pid %d.", pid, getpid());

    if(number % 4 == 0)
      printf("\n4 divide al número introducido");
    else
      printf("\n4 no divide al número introducido");
  }

  printf("\n");
  exit(0);

}
