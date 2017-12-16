/*
Reescribir el programa que implemente un encauzamiento de dos órdenes pero
utilizando fcntl. Este programa admitirá tres argumentos. El primer argumento y el tercero
serán dos órdenes de Linux. El segundo argumento será el carácter “|”. El programa deberá
ahora hacer la redirección de la salida de la orden indicada por el primer argumento hacia el
cauce, y redireccionar la entrada estándar de la segunda orden desde el cauce.*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){

  if(argc != 4){
    perror("Error en el número de argumentos.");
    exit(-1);
  }


  //Creamos una pipe para redirigir el trafico
  int fd[2];
  pipe(fd);

  //Orden 1 argv[1]
  //Orden 2 argv[2]

  /*
    hijo --------> padre
   */

  //Creamos dos procesos

  pid_t pid;

  
  if((pid = fork()) < 0 ){
    perror("Error al hacer fork.");
    exit(-1);
  }

  if (pid==0){
    //Estamos en el hijo


    //Cerramos la entrada del cauce y la salida estandar
    close(1);
    close(fd[0]);
    
    //Redireccionamos la salida
    if (fcntl(fd[1], F_DUPFD, 1) == -1){
      perror("Error al cerrar el descriptor en hijo.");
      exit(-1);
    }

    //Ejecutamos el programa
    execlp(argv[1], argv[1], NULL);

    perror("Error al ejecutar el hijo.");
  }else{
    //Estamos en el padre

    //Cerramos la salida del cauce y la entrada estandar
    close(fd[1]);
    close(0);

    //Redireccionamos el canal a la entrada
    if (fcntl(fd[0], F_DUPFD, 0) == -1){
      perror("Error al cerrar el descriptor en padre.");
      exit(-1);
    }

    //Esperamos a que el hijo esté listo
    wait(NULL);
    
    //Ejecutamos el programa
    execlp(argv[3], argv[3], NULL);
    
  }

  exit(0);
	
}
