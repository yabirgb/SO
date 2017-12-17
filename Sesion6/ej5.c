#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main(int argc, char *argv[]){

  //INFORMACIÓN
  //El archivo que queremos copiar lo tenemos en argv[1]


  //VARIABLES
  
  //file descripor del archivo
  int fd;

  //Datos sobre el archivo
  struct stat st;

  //Variable para guardar el puntero a proyección
  char *p;
  char *po;
  //CODIGO
  
  //Intentamos abrir el archivo
  fd = open(argv[1], O_RDONLY);
  
  if (fd == -1){
    perror("Error al abrir el archivo.");
    exit(EXIT_FAILURE);
  }

  //Tomamos sus datos
  if (fstat(fd, &st) == -1){
    perror("Error al recibir los datos del archivo.");
    exit(EXIT_FAILURE);
  }

  //Comprobamos que sea archivo regular
  if(! S_ISREG(st.st_mode)){
    perror("No se intenta copiar un archivo.");
    exit(EXIT_FAILURE);
  }

  //Creamos una proyección del archivo
  p = (char *)mmap (0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);

  //Procedemos a cerrar el archivo

  if(close(fd) == -1){
    perror("Error al cerrar el archivo");
    exit(EXIT_FAILURE);
  }

  //Abrimso el archivo de salida

  fd = open(argv[2], O_CREAT|O_TRUNC|O_RDWR, S_IRUSR|S_IWUSR);
  
  if (fd == -1){
    perror("Error al abrir el archivo.");
    exit(EXIT_FAILURE);
  }

  //Ampliamos el tamaó
  ftruncate(fd, st.st_size+1);
  po = (char *)mmap (0, st.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

  //Copiamos el contenido
  //memcpy(dest, src, strlen(src)+1);
  memcpy(po, p, st.st_size);

  //Cerramos lo que tengamos pendiente
  close(fd);

  if(munmap(p, st.st_size) == -1){
    perror("Error al eliminar la primera proyección.");
    exit(EXIT_FAILURE);
  }

  if(munmap(po, st.st_size) == -1){
    perror("Error al eliminar la segunda proyección");
    exit(EXIT_FAILURE);
  }

  
  exit(EXIT_SUCCESS);
  
}
