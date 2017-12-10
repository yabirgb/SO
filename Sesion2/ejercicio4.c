#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#define _XOPEN_SOURCE 500
#include <ftw.h>

int contador = 0;
long int sum = 0;

int info(const char* path, const struct stat* atr, int flags, struct FTW* ft){
  if(S_ISREG(atr->st_mode)){
    //Compare with the mask to get permission for execution
    if(atr->st_mode & S_IXOTH && atr->st_mode & S_IXGRP){
      printf("%s : %ld\n",path,atr->st_ino);
      //Increment global variables
      contador++;
      sum += atr->st_size;
    }
      
  }
  return 0;
}


int main(int argc, char *argv[]){
  
  if (nftw(argv[1], info, 10, 0) != 0) {
    perror("Error con la funcion nftw\n");
  }
  printf("Total de archivos %d. Tamaño total: %ld\n", contador, sum );
}
