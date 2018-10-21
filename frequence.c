#include<stdlib.h>
#include<stdio.h>

//definition d'un tableau de frequence
float frequence[256];

//definition de la structure Noeud
typedef struct {float freq;int fg;int fd;int pere;}Noeud;

//declaration d'un tableau de noeud qu'on appelle arbre
Noeud arbre[511];

void calculFrequence(char * fichier){
  char buffer[1];
  FILE* fd= fopen(fichier,"r");
  unsigned int i=0;
  unsigned int cmpt = 0;
  if(fd){
    while(fread(buffer,1,1,fd)){ //je lis dans fd 1 caractere de taile 1octet dans buffer
      printf("%c %i\n",buffer[0],buffer[0]);// j'affiche le caractere lu et son code ASCII
      frequence[buffer[0]]++;
      i++;
    }
    while(cmpt<256){
      if(frequence[cmpt] != 0){
	frequence[cmpt]/=i;
	printf("%u-> %f\n",cmpt,frequence[cmpt]);
      }
      cmpt++;
    }
  }else{
    printf("le flux de lecture de fichier n'a pas ete bien ouvert");
  }
}

//initialisation de mon arbre
int j=0;
void initArbre(){
  for(j;j<511;j++){
    arbre[j].pere=-1 ;
    arbre[j].fg=-1;
    arbre[j].fd=-1;
    if(j<256){
      arbre[j].freq=frequence[j];
    }else{
      arbre[j].freq=0.0;
    }
  }
}

//afficher mon tableau arbre
void printArbre(){
  for(int i=0;i<511;++i){
    printf("%i  : %i %i %i %f\n",i, arbre[i].pere,arbre[i].fg,arbre[i].fd,arbre[i].freq);
  }
}
//modifier les pere des minimum
int dexiemepartDearbre=256;
void modifierPereEtFils(int indice1,int indice2){
  
  arbre[indice1].pere=dexiemepartDearbre;
  arbre[indice2].pere=dexiemepartDearbre;
  arbre[dexiemepartDearbre].fd= indice2;
  arbre[dexiemepartDearbre].fg= indice1;
  arbre[dexiemepartDearbre].freq= frequence[indice2]+frequence[indice1];
  dexiemepartDearbre++;
}
int main(int argc,char* argv[]){
  calculFrequence(argv[1]);
  initArbre();
  //calcul des minimum
  float min1=1.0;
  float min2=1.0;
  int k=0; int indice1,indice2;
  
  while(k<256){
    if( arbre[k].freq<min1 && arbre[k].freq != 0.0 && arbre[k].pere==-1 ){
      min2=min1;
      min1=arbre[k].freq;
      indice1=k;
    
    }else if(arbre[k].freq<min2 && arbre[k].freq!=0.0 && arbre[k].pere==-1){
      min2=arbre[k].freq;
      indice2=k;
    }
    k++;
  }
  printf("(minimum1,minimum2)->(%f,%f)\n",min1,min2);
  modifierPereEtFils(indice1,indice2);
  printArbre();

  return 0;
}
