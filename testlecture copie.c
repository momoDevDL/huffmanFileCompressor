#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>


int main(int argc,char* argv[]){
  char buffer[1];
  unsigned int i=1;
  unsigned int c;
  unsigned int nbtotal;
  FILE* fd= fopen(argv[1],"r");
  
  if(fd){
    while(fread(buffer,1,1,fd)){
      printf("%c %i\n",buffer[0],buffer[0]);
      if (i==1){
	//truc
      }else if (i==2){
	c=buffer[0];
      }else if (i>=3){
	if (buffer[0]<0){
	  nbtotal+=(128+(128+buffer[0]));
	}else{
	  nbtotal+=buffer[0];
	}
      }
      i++;
    }
    
  }else{
    printf("le flux de lecture de fichier n'a pas ete bien ouvert");
  }
    while (nbtotal > 0){
    printf("%c",c);
    nbtotal--;
    }
  printf("\n");
  printf("FIN\n");
}
