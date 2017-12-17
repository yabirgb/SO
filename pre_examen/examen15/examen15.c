#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/mman.h>
#include <signal.h>

int main(int argc, char *argv[]){

  //Archivo en argv[1]
  const int SIZE = 200;
  char fifo[] = "archivoFifo";
  pid_t pid;

  int fd; //Descriptor del fifo

  //Ignoramos el ctrl+c
  signal(SIGINT,SIG_IGN);		


  if(mkfifo(fifo, O_CREAT|S_IRWXU) < 0){
    perror("Error al crear fifo de comunicación.");
    unlink(fifo);
    exit(EXIT_FAILURE);
  }

  
  if( (fd = open(fifo, O_RDWR)) < 0){
    perror("Erro al abrir archivo fifo");
    exit(EXIT_FAILURE);
  }

  pid = fork();
  
  if(pid == -1){
    perror("Error al hacer fork");
    exit(EXIT_FAILURE);
  }

  if (pid > 0){
    char buffer[SIZE]; //Buffer para guardar el nombre
    int read_size; //Tamaño de lo que leemos
    char *m; //Puntero a la proyección
    struct stat st;
    int ad; //Archivo que vamos a leer
    
    //Leemos el nombre de archivo y lo almacenamos en buffer
    read_size = read(fd,buffer, SIZE);
      
    if(read_size == -1){
      perror("Error en la lectura");
      exit(EXIT_FAILURE);
    }
    
    //Quitamos el FIFO. Ya no lo necesitamo

    //Ahora lo proyectamos en memoria

    //Almacenamos su tamaño
    if(stat(buffer, &st) < 0){
      perror("Erro al leer el archvo");
      exit(EXIT_FAILURE);
    }

    //Abrimos el archivo
    if( (ad = open(buffer, O_RDWR|O_EXCL, S_IRWXU)) < 0){
      perror("Erro al abrir archivo a leer.");
      exit(EXIT_FAILURE);
    }
    
    ftruncate(ad,st.st_size + strlen("REVISADO\n"));

    //lo proyectamos
    printf("%lu\n", st.st_size);
    m = (char *) mmap (0, st.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, ad, 0);
    
    if (m == MAP_FAILED) {
      perror("Fallo la proyeccion");
      exit(EXIT_FAILURE);
    }

    //Lo cerramos ya que no lo necesitamos más
    close(ad);

    //Añadimos la cadena al final
    sprintf(m + strlen(m),"REVISADO\n");
    
    printf("HIJO: He terminado\n");
    unlink(fifo);
    exit(EXIT_SUCCESS);
  }else{
    //Estamos en el proceso padre

    //Estructuras para leer el directorio
    DIR *dir;
    struct dirent *dent;
    struct stat atributos;

    //Variable para marcar si hemos encontrado el archivo
    //que buscamos
    int encontrado = 1;

    //Abrimos el directorio actual
    dir = opendir(".");

    //Leemos el directorio

    while ((dent=readdir(dir)) != NULL && encontrado){
      if(stat(dent->d_name, &atributos) < 0){
	perror("Erro al leer el archvo");
	exit(EXIT_FAILURE);
      }

      //Comprobamos que se trate de un archivo regular
      if(S_ISREG(atributos.st_mode)){

	//Si es un archivo regular comprobamos si es el que buscamos

	if(strcmp(dent->d_name, argv[1]) == 0){
	  //Si se trata del archivo que buscamos le damos permisos de
	  //escritura y mandamos su nombre

	  if( (atributos.st_mode & 0400) != 0400) {
	    int pr = atributos.st_mode;
	    pr |= 0400;
	    chmod(argv[1], pr);
	  }
	  
	  if(write(fd, &dent->d_name, SIZE) < 0){
	    perror("Error al escribir el nombre");
	    exit(EXIT_FAILURE);
	  }

	  //Imprimimos inodo y tamaño en bytes

	  printf("Archivo %s: Inodo %lu con un tamaño de %lu bytes\n", dent->d_name, atributos.st_ino, atributos.st_size);

	  encontrado = 0;
	  
	}
	
      }
      
    }

    exit(EXIT_SUCCESS);
  }

  exit(EXIT_SUCCESS);

}
