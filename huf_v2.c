#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
 
/*!
 * \file huf_v2.c
 * \brief Compresseur de fichier avec la methode de Huffman 
 * \author Antoine Afflatet / Mohamed Masbah Abou Laich
 * \version 0.2
 */

float frequence[256]; 

 char *bin[257];

/*!
 * \brief definition d'une structure Noeud 
 */

typedef struct {
  float freq;
  int fg;
  int fd;
  int pere;
}Noeud;


/*!
 * \brief declaration d'une tableau de Noeud
 * de taille 511 ( caractère + Noeud crée )
 */

Noeud arbre[511];

unsigned int mode = 0; // mode de compression 
unsigned int tailleF=0;

/*!
 * \brief declaration d'une func qui calcule la
 * frequence des caracteres dans les fichiers en 
 * parcourant le fichier passé en parametre
 * et précise le mode de compression
 */

void calculFrequence(char * fichier){
  char buffer[1];
  unsigned int buffer2;
  FILE* fd= fopen(fichier,"r");
  unsigned int nbdif=0;
  unsigned int cmpt = 0;

  if(fd){
    while(fread(buffer,1,1,fd)){
     
      printf("%c %i\n",buffer[0],buffer[0]);
     
      if (buffer[0]<0){
	buffer2=128+(128 + buffer[0]);
      }
      if (buffer[0]==0){
	buffer2=0;
      }
      buffer[0]>=0 ? frequence[buffer[0]]++ : frequence[buffer2]++;
      
      tailleF++; 
    }
    
    while(cmpt<256){
      if(frequence[cmpt] != 0){
	frequence[cmpt]/=tailleF;
	nbdif++;

	printf("ASCII(%i) correspondant au Char(%c) =  %f%% d'apparition dans le fichier. \n",cmpt,cmpt,frequence[cmpt]);
      }
      cmpt++;
    }
    
    if (tailleF<(3*nbdif)){
      mode = 2;
    }else if (nbdif==1){
      mode = 1;
    }else{
      mode = 0;
    }
  }else{
    printf("le flux de lecture de fichier n'a pas ete bien ouvert");
  }
}

/*!
 * \brief declaration d'une func qui modifie 
 * le pere , le fils , et la frequence des minimums 
 * dans l'arbre apres les avoir trouvés
 */

void modifierPereFilsFrequence(int indice1,int indice2,unsigned int k){
  arbre[indice1].pere=k;
  arbre[indice2].pere=k;
  arbre[k].fd= indice1;
  arbre[k].fg= indice2;
  arbre[k].freq= arbre[indice2].freq + arbre[indice1].freq;
}

/*!
 * \brief declaration d'une func qui va initialiser 
 * l'arbre en passant a chaque noeud d'un caractere 
 * existant dans le fichier sa frequence 
 * et initialiser les autres char avec des valeurs par defaut
 */

void  initArbre(){
  
  for(int j=0;j<511;j++){
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
  
/*!
 * \brief declaration d'une func qui va parcourir tout les noeuds
 * de l'arbre tant qu'on a pas atteint le Noeud 511 ou arrivé à 
 * un noeud dans la frequence vaut 1 ( tout les caracteres de fichiers 
 * ont été traité) tout en prenant deux mins et modifier leur Noeud 
 * et creer un nouveau Noeus a partir de ces deux mins
 * Return le nombre de Noeud total dans l'arbre .
 */

unsigned int ParcoursMinNoeud(){

  float min1;
  float min2;
  unsigned int k;
  unsigned int indice1=0,indice2;
  unsigned int nbNoeuds=256;
  
  while(arbre[nbNoeuds-1].freq<1.0 && nbNoeuds<511){
    k=0; min1=1.0; min2=1.0;
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

/*!
 * \brief declaration d'une func qui va parcourir l'arbre créé
 * en générant le code binaire associé a chaque car
 * et le mettre dans un tableau de binaire 
 */

void parcoursCode(int nbNoeuds,char *code){
  if(arbre[nbNoeuds].fg!=-1){
    char *ncode=(char*)malloc(strlen(code)+1);
    strcpy(ncode,code);
    ncode[strlen(code)]='0';
    ncode[strlen(code)+1]='\0';
    parcoursCode(arbre[nbNoeuds].fg,ncode);
    ncode[strlen(code)]='1';
    ncode[strlen(code)+1]='\0';
    parcoursCode(arbre[nbNoeuds].fd,ncode);
  }else{
    
    if( code[strlen(code)-1]=='0'){
      printf("%i %c->%s\n",arbre[arbre[nbNoeuds].pere].fg,
	     arbre[arbre[nbNoeuds].pere].fg,code);
    }else{

      printf("%i %c->%s\n",arbre[arbre[nbNoeuds].pere].fd,
	     arbre[arbre[nbNoeuds].pere].fd,code);
    }
    
    if (nbNoeuds==0){
      bin[0]=strdup(code);
    }else{
      bin[nbNoeuds]=strdup(code);
    }
  }
}


/*!
 * \brief declaration d'une func qui va d'abord creer une entete
 * a paritir de l'arbre en utilisant le cheminement suivant :
 * MODE TAILLEF CAR TAILLE_BIN CODE_BIN 
 * Puis parcourir le fichier source en mettant dans un buffer 
 *le code binaire de chaque caractere . Et une fois la taille de buffer
 * vaut 8 on commence le calcul de l'entier pour les 8 premieres
 * case de buffer .
 * source et cr
 */



void compression(char *fichier1 ,char *fichier2,unsigned int nb){
  unsigned char buffer[1];
  unsigned char buffer2[262];
  unsigned int i=0;
  unsigned char car;
  unsigned int binT=0;
  unsigned int tete2Lecture=0;
  unsigned int binV=0;
  unsigned int ftailleF=0;
  unsigned int TailleF=tailleF;
  unsigned int base=255;
  
  FILE*fr=fopen(fichier1,"r");
  FILE*fw=fopen(fichier2,"w");
  if(fr && fw){
    //entête-----------------------------------------------
    fputc(mode,fw);
    ftailleF++;
    while(TailleF>=base){
      fputc(base,fw);
      ftailleF++;
      TailleF-=base;
    }
    fputc(TailleF,fw);
    ftailleF++;
    fputc(nb,fw);
    ftailleF++;
    
    for(unsigned int parcours=0;parcours<256;parcours++){
      if(frequence[parcours]!=0){
	
	fputc(parcours,fw);
	ftailleF++;
	while(binT<strlen(bin[parcours])){ 
	  buffer2[binT]=bin[parcours][binT];
	  binT++;
	}
	
	fputc(binT,fw);
	ftailleF++;
	
	while(binT>=8){
	  for(int j=7;j>=0;j--){
	    binV+=pow(2,7-j)*(buffer2[j]-'0');
	  }
	  
	  fputc(binV,fw);
	  ftailleF++;
	  binT-=8;
	  binV=0;
	  for(int k=0;k<254;k++){
	    buffer2[k]=buffer2[k+8];
	  }
	}
	
	if(binT<8 && binT>0){
	  for(int e=binT;e<8;e++){
	    buffer2[e]='0';
	  }
	
	  for(int j=7;j>=0;j--){
	  
	    binV+=pow(2,7-j)*(buffer2[j]-'0');
	  }
	
	  fputc(binV,fw);
	  ftailleF++;
	  binT=0;
	  binV=0;
	}
      }
    }
    //-----------------------------------------------------
    while(fread(buffer,1,1,fr)){
      car=buffer[0];
      
      while(binT<strlen(bin[car])){ 
	buffer2[tete2Lecture]=bin[car][binT];
	binT++;
	tete2Lecture++;
      }
      while(tete2Lecture>=8){
	for(int j=7;j>=0;j--){
	  binV+=pow(2,7-j)*(buffer2[j]-'0');
	}
	fputc(binV,fw);
	ftailleF++;
	tete2Lecture-=8;
	binV=0;
	for(int k=0;k<254;k++){
	  buffer2[k]=buffer2[k+8];
	}
      }
      
      binT=0;
    }
    if(tete2Lecture!=0){
      for(int e=tete2Lecture;e<8;e++){
	buffer2[e]='0';
	tete2Lecture++;
      }                              
      for(int j=7;j>=0;j--){
	
	
	binV+=pow(2,7-j)*(buffer2[j]-'0');
	
      }                                  
      fputc(binV,fw);
      ftailleF++;
      tete2Lecture-=8;
     
    }
    
  }else{
    printf("erreur dans l'ouverture de flux de lecture et d'ecriture de fichier");
  }
  fclose(fr);
  fclose(fw); 
  
  printf("Taille originelle : %i; taille compressée : %i; gain : %f%% !\n",tailleF,ftailleF,((1-(((float)ftailleF/(float)tailleF)))*100));
}

/*!
 * \brief declaration d'une func qui compresse le fichier 
 * avec le mode 1 si le fichier ne contient qu'un seul caractere repété 
 * n fois 
 */

void compression2(char *fichier1 ,char *fichier2){
  unsigned int i=0;
  unsigned int base=255;
  FILE*fw=fopen(fichier2,"w");
  if(fw){
    while(frequence[i] == 0){
      i++;
    }
    fputc(mode,fw);
    fputc(i,fw);

    while (tailleF>base){
      fputc(base,fw);
      tailleF=tailleF-base;

    }
    fputc(tailleF,fw);

    
    
  }else{
    printf("erreur dans l'ouverture du flux d'ecriture de fichier");
  }
  
  fclose(fw);
}
 
     
/*!
 * \brief declaration d'une func d'affichage 
 * de l'arbre
 */

void printArbre(unsigned int nb){
  for(unsigned int i=0;i<nb;i++){
    printf("%u  : %i %i %i %f\n",i, arbre[i].pere,arbre[i].fg,arbre[i].fd,arbre[i].freq);
  }
}


//programme principal et appel au fonction

int main(int argc,char* argv[]){
  unsigned int nb;
  bin[256]=NULL;
  calculFrequence(argv[1]);
  if (mode==2){
    printf("Le fichier ne peut être compréssé par cet algorithme: le fichier compréssé serai alors plus grand que le fichier source.\n");
    
  } else if (mode==1){
    printf("Le fichier va être compréssé sans utilisé Huffman: le fichier ne contient qu'un charactére différent.\n");
    compression2(argv[1],argv[2]);
    
  } else if (mode==0){
    if(argc<3){
      printf("-------usage : ./huf2 nomFichierSource nomFichierHuf -------\n");
    }else{
      initArbre();
      nb=ParcoursMinNoeud();
      printArbre(nb);
      parcoursCode(nb-1,"");
      compression(argv[1],argv[2],nb-255);
    } 
   
  }
   return 0;
}
