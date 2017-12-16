/*Escribe un programa que acepte como argumentos el nombre de un programa, sus
argumentos si los tiene, y opcionalmente la cadena “bg”. Nuesto programa deberá ejecutar el
programa pasado como primer argumento en foreground si no se especifica la cadena “bg” y en
background en caso contrario. Si el programa tiene argumentos hay que ejecutarlo con éstos.*/

#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/wait.h>

#include <string.h> //strcmp

int main(int args, char *argv[]){

  if (args < 2){
    printf("Error en el número de argumentos\n");
    exit(-1);
  }

  int bg = 0; //false
  char cadena[]  = "bg";
  pid_t PID, pido;
  int estado;

  char **argumentos = argv;
  argumentos++;


  if(strcmp(cadena, argv[args-1]) == 0)
    bg = 1;

  printf("El valor de bg es %d\n", bg);
  
  if((PID = fork()) < 0){
    perror("Error al hacer fork\n");
  }else if(PID==0){
    //Proceso hijo
    
      printf("Ejecutando en background\n");

      argumentos[args-2] = '\0';
      
      if(execvp(argv[1], argumentos) < 0)
  	perror("Error en la ejececución de la instrucción");
  }else{
    //Proceso padre
    if(bg){
      pido= wait(&estado);
    }
    printf("Programa hijo %d finalizado con estado %d\n", PID, estado);
    printf("Programa padre %d finalizado con estado \n", getpid());
    exit(0);

  }

  

  
}
