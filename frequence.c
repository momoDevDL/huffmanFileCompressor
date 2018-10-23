#include<stdlib.h>
#include<stdio.h>

//definition d'un tableau de frequence
float frequence[256];

//definition de la structure Noeud
typedef struct {float freq;int fg; int fd;int pere;}Noeud;

//declaration d'un tableau de noeud qu'on appelle arbre
Noeud arbre[511];
unsigned int cmpt = 0;
unsigned int i=0;
void calculFrequence(char * fichier){
  char buffer[1];
  FILE* fd= fopen(fichier,"r");
  
  if(fd){
    while(fread(buffer,1,1,fd)){ //je lis dans fd 1 caractere de taile 1 octet sur buffer
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

//modifier les pere des minimum

void modifierPereFilsFrequence(int indice1,int indice2,int k){
  arbre[indice1].pere=k;
  arbre[indice2].pere=k;
  arbre[k].fd= indice2;
  arbre[k].fg= indice1;
  arbre[k].freq= arbre[indice2].freq + arbre[indice1].freq;
  arbre[indice1].freq=0.0;
  arbre[indice2].freq=0.0;
  k++;
}

//initialisation de mon arbre

unsigned int initArbre(){
  int j=0;
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
  
  //recherche des mins et creation des noeuds
   //calcul des minimums
  float min1=1.0;
  float min2=1.0;
  unsigned int k=0;
  unsigned int indice1,indice2;
  unsigned int nbNoeuds=256;
  
  while(arbre[nbNoeuds-1].freq!=1.0){
    k=0;
    while(k<nbNoeuds){
      if( arbre[k].freq<min1 && arbre[k].freq != 0.0 && arbre[k].pere==-1 ){
	min2=min1;
	min1=arbre[k].freq;
	indice2=indice1;
	indice1=k;
    
      }else if(arbre[k].freq<min2 && arbre[k].freq!=0.0 && arbre[k].pere==-1){
	min2=arbre[k].freq;
	indice2=k;
      }
      k++;
    }
    modifierPereFilsFrequence(indice1,indice2,k);
    nbNoeuds++;
  }
  return nbNoeuds;
}

//afficher mon tableau arbre
void printArbre(unsigned int nb){
  for(unsigned int i=0;i<nb;++i){
    printf("%u  : %i %i %i %f\n",i, arbre[i].pere,arbre[i].fg,arbre[i].fd,arbre[i].freq);
  }
}


int main(int argc,char* argv[]){
  unsigned int nb;
  calculFrequence(argv[1]);
  nb= initArbre();
  printf("%u\n",i);
  printArbre(nb);

  return 0;
}
