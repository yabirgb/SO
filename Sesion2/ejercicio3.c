#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<dirent.h>
#include <string.h>


int contador = 0;
long int sum = 0;

void search(char dirname[]){
  DIR *dir;

  //Open the dir
  if((dir = opendir(dirname)) < 0){
    perror("Error con opendir\n");
  }
  
  //Create structures to save the info
  struct dirent *dent;
  struct stat atr;

  //Access the dir
  if (chdir(dirname) < 0){
    perror("Error al acceder al directorio\n");
  }

  
  //For each file
  while((dent=readdir(dir)) != NULL){

    //Get its info
    if(stat(dent->d_name, &atr) < 0){
      printf("%s := Error\n", dent->d_name);
    }

    //Check if is a regular file
    if(S_ISREG(atr.st_mode)){
      //Compare with the mask to get permission for execution
      if(atr.st_mode & S_IXOTH && atr.st_mode & S_IXGRP){
	printf("%s : %ld\n",dent->d_name,dent->d_ino);
	//Increment global variables
	contador++;
	sum += atr.st_size;
      }
      
    }

    //Dont have a elif because i will have to indent
    //Check that is not . nor ..
    if(S_ISDIR(atr.st_mode) && strcmp(dent->d_name, ".") != 0 && strcmp(dent->d_name, "..") != 0 ){
      //Mark it as the next target
      char next[500];
      sprintf(next, "%s", dent->d_name);
      //Recursivity party
      search(next);
    }
    
  }

  //Return one level up
  chdir("..");
  //Close the opened dir
  closedir(dir);
}

int main(int argc, char *argv[]){

  search(argv[1]);
  printf("Total de archivos %d. Tamaño total: %ld\n", contador, sum );
}
