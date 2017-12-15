#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <wait.h>


void cerrajear(int bloqueo, int operacion){
  struct flock mylock;

  mylock.l_type = operacion;
  mylock.l_whence = SEEK_SET;
  mylock.l_start = 0;
  mylock.l_len = 0;

  if (fcntl(bloqueo, F_SETLKW ,&mylock) == -1){
    perror("Error con el cerrojo");
    exit(-1);
  }

}

int main(int argc, char *argv[]){

  FILE *tmp;
  int max_size = 1024;
  int readbytes;
  char lectura[max_size], nombrefifo[300];
  int read_size;
  int fd;

  fd = atoi(argv[1]);
  
  //printf("Creando archivo temporal\n");
  if( (tmp = tmpfile()) == NULL ){
    perror("Error en la creación de un archivo intermedio.");
    exit(-1);
  }


  //Leemos los datos del cliente a través del fifo
  //printf("Proxy %d: Comienza lectura.\n", getpid());

  while((read_size = read(STDIN_FILENO, &lectura, max_size)) > 0){
    
    if (fwrite(&lectura, sizeof(char), max_size, tmp) == -1 ) {
      perror("Proxy: error en la lectura de datos del fifo.\n");
    }
    
    if(read_size == -1){
      perror("FIFO: Error en la lectura desde el stdin");
    }
    
    //printf("LEYENDO:%d\n",  read_size);
  }
  
  //printf("Bloqueamos\n");
  
  cerrajear(fd, F_WRLCK);

  rewind(tmp);
  while ((read_size = fread(&lectura, sizeof(char), max_size, tmp)) > 0) {
    if (write(STDOUT_FILENO, lectura, read_size) == -1) {
      perror("\nError de escritura en proxy");
      exit(-1);
    }
  }

  cerrajear(fd, F_UNLCK);

  //Cerramos el fifo del proxy
  sprintf(nombrefifo, "fifo.%d", getpid());
  if (unlink (nombrefifo) == -1) {
    perror("\nError al destruir el fifo del proxy");
    exit(EXIT_FAILURE);
  }

  //printf("Proxy: termina ejecución %d\n", getpid());
  //El archivo temporal se destruye automáticamente al terminar el programa
  exit(0);
  
}
