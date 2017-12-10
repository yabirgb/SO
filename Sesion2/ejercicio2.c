//Ejercicio 2
//Cambiar los permisos a todos los archivos de un directorio


#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<dirent.h>
#include <string.h>


int main(int argc, char *argv[]){

  if(argc != 3){
    printf("Número de argumentos erroneo. DEP\n");
    exit(EXIT_FAILURE);
  }


  DIR *dir; //Data struture for a dir
  struct dirent *dent; //Struct containing dir info
  struct stat atributos; //Struct containing file info
  int fd;

  char *ptr; //Pointer used by strtol to convert to base 8
  mode_t perm = strtol(argv[2],&ptr, 8); //Permission in octal base
  mode_t old; //Old permissions

  //Open the dir
  dir = opendir(argv[1]);

  //Access the dir as the name is relative to pah
  if (chdir(argv[1]) < 0){
    perror("Error al acceder al directorio\n");
  }
  
  //If we got the dir
  if(dir!=NULL){
    //Iterate over the elements in dir
    while((dent=readdir(dir))!=NULL){

      //Get the info of the file and store it
      if(stat(dent->d_name,&atributos)>= 0){
	if(S_ISREG(atributos.st_mode)){
	  //save old mask
	  old = atributos.st_mode;

	  //change permissions
	  if (chmod(dent->d_name, perm) < 0){
	    printf("%s: ", dent->d_name);
	    perror("Error al cambiar permisos\n");
	  }else{
	    //refresh the new permissions
	    stat(dent->d_name,&atributos);
	    //print the output
	    printf("%s: %o -> %o\n", dent->d_name, old, atributos.st_mode);
	  }
	}
      }else{
	perror("Error al comprobar la información del archivo.\n");
	exit(-1);
      }
    }
  }else{
    perror("Error al abrir el directorio");
  }
  //Close the dir when finish
  closedir(dir);

  return EXIT_SUCCESS;

}
