#include<stdio.h>
#include<stdlib.h>

int main(int argc,char** argv){
  
  long int i;
  int cmpt=0;
  printf("veuillez entrez un nombre entier :\n");
  scanf("%li",&i);
  //  printf("\n%li\n",i);
  long int n= i;
  //printf("%li",n);
  
  while(n!=0){
    cmpt++;
    n=n/10;
  }
 char * chaine=(char*)malloc((cmpt+1)*sizeof(char));
 
 chaine[cmpt]='\0';
 printf("%s \n",chaine);
 printf("%i\n",cmpt);
  while( cmpt!=0){
    n=i;
    n%=10;
    chaine[cmpt-1]= n +'0';
    i/=10;
    cmpt--;
  }
 
  printf("this is the result %s\n",chaine);

  return 0;
}
