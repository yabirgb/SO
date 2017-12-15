#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <wait.h>
#include <signal.h>

int continueReading = 1;

char nombree[200], nombres[200];
void stop(int signum){

    printf("Cerrando servidor\n");
    continueReading = 0;
    unlink(nombree);
    unlink(nombres);
    exit(-1);

}

int main(int argc, char *argv[]){

  int fifoe, fifos;
  int input;

  //Descriptor del fichero de bloqueo
  int fdbloqueo;
  int bytesleidos;

  //Comprabamos que los datos están bien.
  if (argc != 2){
    printf("Formato erroneo: ./server <nombre_fifo>");
    exit(-1);
  }

  signal(SIGINT, stop);
  //signal(SIGCHILD, SIG_IGN);
  
  if((fdbloqueo = open("/tmp/bloqueo", O_CREAT|O_TRUNC|O_WRONLY, S_IRWXU)) == -1) {
    perror("\nServidor: error al crear el archivo de bloqueo");
    exit(EXIT_FAILURE);
  }

  //Guardamos los nombres para los fifos
  sprintf(nombree, "%se", argv[1]);
  sprintf(nombres, "%ss", argv[1]);

  //Creamos los fifos de entrada y salida

  umask(0);
  
  if(mkfifo(nombree, O_CREAT|S_IRWXU)<0){
    perror("Error al crear fifo entrada");
    unlink(nombree);
  }

  if(mkfifo(nombres, O_CREAT|S_IRWXU) < 0){
    perror("Error al crear fifo salida");
    unlink(nombree);
    unlink(nombres);
  }

  //Abrimos los fifos

  //Descriptores de los archivos fifo
  int fds, fde;
  
  //Fifo de entrada
  
  
  if ( (fde = open(nombree, O_RDONLY)) < 0 ){
    perror ("Error en la creacion de channel. ");
  }

  //Fifo de salida
  if ( (fds = open(nombres, O_RDWR)) < 0 ){
    perror ("Error en la creacion de channel. ");
  }

  while((bytesleidos = read(fde, &input, sizeof(int))) || continueReading){

    if (bytesleidos == -1) {
      if (errno == EINTR) {
        continue;
      } else {
        perror("Servidor: error al leer del fifo conocido de entrada");
        exit(EXIT_FAILURE);
      }
    }

    if (bytesleidos == 0){
      continue;
    }

    printf("Mensaje recibido - %d\n", bytesleidos);

    pid_t pid;
    if( (pid = fork()) < 0 ){
      perror("Error al hacer fork");
      exit(-1);
    }else if(pid == 0){

      //Obtenemos el pid del proceso hijo
      pid_t proxypid = getpid();

      //Creamos el fifo con el que se van a comunicar cliente y proxy
      char proxyfifo[200];
      sprintf(proxyfifo, "fifo.%d", proxypid);

      if( mkfifo(proxyfifo, O_CREAT|S_IRWXU) == -1){
	perror("Server: Error al crear fifo de comunicación con cliente");
	unlink(proxyfifo);
	exit(-1);
      }

      printf("Server: Fifo de comunicación creado\n");

      //Le decimos al client cual es el pid del hijo

      if( write(fds, &proxypid, sizeof(int)) < 0 ){
	perror("Server: Error en la escritura del pid hijo");
      }

      //Abrimos el fifo de comunicación para cambiar la salida estandar

      int fdcom;
      if ( (fdcom = open(proxyfifo, O_RDONLY)) < 0 ){
	perror ("Server: Error al abrir el fifo de comunicacion. ");
      }

      if (dup2(fdcom, STDIN_FILENO) == -1){
	perror("Server: Error al redireccionar la salida.");
      }

      //HAcemos el fd del archivo de bloqueo una cadena char
      char archivo_bloqueo[100];
      sprintf(archivo_bloqueo, "%d", fdbloqueo);
      execlp("./proxy", "proxy", archivo_bloqueo, NULL);

      perror("Server: Error al llamar a proxy.");
      
    }
    
  }

  unlink(nombree);
  unlink(nombres);

}
