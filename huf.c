#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>


float frequence[256];
char *bin[257];
typedef struct {float freq;int fg; int fd;int pere;}Noeud;
Noeud arbre[511];
unsigned int mode = 0;
unsigned int tailleF=0;
unsigned int tailleFA=0;

void calculFrequence(char * fichier){
  unsigned char buffer[1];
  unsigned int buffer2;
  FILE* fd= fopen(fichier,"r");
  unsigned int nbdif=0;
  unsigned int cmpt = 0;
  
  if(fd){
    
    while(fread(buffer,1,1,fd)){
     
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
    
    
    if (tailleF<(2+(tailleF/255)+(2*nbdif)+(tailleF)/8)){
  
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


void modifierPereFilsFrequence(int indice1,int indice2,unsigned int k){
  arbre[indice1].pere=k;
  arbre[indice2].pere=k;
  arbre[k].fd= indice1;
  arbre[k].fg= indice2;
  arbre[k].freq= arbre[indice2].freq + arbre[indice1].freq;
}


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
  
unsigned int initNoeuds(){
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
    
    if (nbNoeuds==0){
      
      bin[0]=strdup(code);
      
    }else{
      
      bin[nbNoeuds]=strdup(code);
    }
  }
}





void compression(char *fichier1 ,char *fichier2,unsigned int nb){
  unsigned char buffer[1]; 
  char buffer2[262];
  unsigned int ip=0;
  unsigned int i=0;
  unsigned int r=0;
  unsigned int TailleF=tailleF;
  float fTailleF;
  float fTailleFA;
  unsigned int base=255;
  unsigned int pereNb=0;
  FILE*fr=fopen(fichier1,"r"); 
  FILE*fw=fopen(fichier2,"w");
  
  if(fr && fw){
    
    //entête-----------------------------------------------
    fputc(mode,fw);
    tailleFA++;
    fputc(nb,fw);
    tailleFA++;
    pereNb=nb+255;
    
    while (TailleF>=255){
      
      fputc(base,fw);
      tailleFA++;
      TailleF-=255;
      
    }
    
    fputc(TailleF,fw);
    tailleFA++;
    
    while (pereNb>255){
      
      if ((arbre[pereNb].fg)>=255){
	
	fputc(base,fw);
	tailleFA++;
	fputc(((arbre[pereNb].fg)-255),fw);
	tailleFA++;
	
      } else {
	
	fputc((arbre[pereNb].fg),fw);
	tailleFA++;
      }
      
      if ((arbre[pereNb].fd)>=255){
	
	fputc(base,fw);
	tailleFA++;
	fputc(((arbre[pereNb].fd)-255),fw);
	tailleFA++;
	
      } else {
	
	fputc((arbre[pereNb].fd),fw);
	tailleFA++;
      }
      pereNb--;    }
    
    //-----------------------------------------------------
    while(fread(buffer,1,1,fr)){

      
      i=0;
      
      while(i<strlen(bin[buffer[0]])){    
	
	buffer2[ip]=bin[buffer[0]][i];    
	i++;
	ip++;
	
      }
      
      while(ip>=8){              
	
	for(int j=7;j>=0;j--){
	  
	  const char buf=buffer2[j];
	  r=r+(pow(2,7-j)*atoi(&buf));
	  
	}
	
	fputc(r,fw);            
	tailleFA++;
	
	for(int k=0;k<254;k++){
	  
	  buffer2[k]=buffer2[k+8]; 
	}
	
	ip=ip-8; 
	r=0;
   
      }
    }
    
    if(ip!=0){
      
      for(int e=ip;e<8;e++){
	
	buffer2[e]='0';
	ip++;
	
      }                                  
      
      for(int j=7;j>=0;j--){
	
	const char buf=buffer2[j];
	r=r+(pow(2,7-j)*atoi(&buf));
	
      }                                
      
      fputc(r,fw);
      tailleFA++;
      
      for(int w=0;w<254;w++){
	
	buffer2[w]=buffer2[w+8];
	
      }
      
      ip-=8;
      
    }
    
  }else{
    
    printf("erreur dans l'ouverture de flux de lecture et d'ecriture de fichier.\n");
  }
  fclose(fr); 
  fclose(fw);

  fTailleF=tailleF;
  fTailleFA=tailleFA;
  printf("Taille originelle : %i; taille compressée : %i; gain : %f%% !\n",tailleF,tailleFA,((1-(fTailleFA/fTailleF))*100));
  
}
  


void compression2(char *fichier1 ,char *fichier2){
  unsigned int i=0;
  unsigned int base=255;
  unsigned int TailleF=tailleF;
  float fTailleF;
  float fTailleFA;
  FILE*fw=fopen(fichier2,"w");
  
  if(fw){
    
    while(frequence[i] == 0){
      
      i++;
    }
    
    fputc(mode,fw);
    tailleFA++;
    fputc(i,fw);
    tailleFA++;
    
    while (TailleF>base){
      
     fputc(base,fw);
     tailleFA++;
     TailleF=TailleF-base;
     
    }
    fputc(TailleF,fw);
    tailleFA++;
    
    
  }else{
    
    printf("erreur dans l'ouverture du flux d'ecriture de fichier.\n");
    
  }
  
  fclose(fw);

  fTailleF=tailleF;
  fTailleFA=tailleFA;
  printf("Taille originelle : %i; taille compressée : %i; gain : %f%% !\n",tailleF,tailleFA,((1-(fTailleFA/fTailleF))*100));
  
}
 

void printArbre(unsigned int nb){
  
  for(unsigned int i=0;i<nb;i++){
    printf("%u  : %i %i %i %f\n",i, arbre[i].pere,arbre[i].fg,arbre[i].fd,arbre[i].freq);
    
  }
}

void printCodeCharEtMoyenne(){
  float m1=0;
  unsigned int nb1=0;
  float m2=0;
  float nb2=0;
  
  for(unsigned int i=0;i<256;i++){
    
    if (bin[i]!=NULL){
      
      printf("codeChar(%c)=%s \n",i,bin[i]);
      m1+=strlen(bin[i]);
      nb1++;
      m2+=(strlen(bin[i])*(frequence[i]*100));
      nb2+=(frequence[i]*100);
      
    }  
  }
  
  printf("Par rapport au nombre de Char différent sans tenir compte de leur fréquence réspective:\n");
  printf("Longueur moyenne de codage = %f bit(s).\n",(m1/nb1));
  
  printf("Par rapport au nombre de Char différent en tenant compte de leur fréquence réspective:\n");
  printf("Longueur moyenne de codage = %f bit(s).\n",(m2/nb2));
  
}


int main(int argc,char* argv[]){
  unsigned int nb;
  bin[256]=NULL;
  
  calculFrequence(argv[1]);
  
  if (mode==2){
    
    printf("Le fichier ne peut être compréssé par cet algorithme: le fichier compréssé serai alors plus grand que le fichier source.\n");
    
    //compression par dictionnaire ou arbreHuff basé sur l'utilisation des lettre dans la langue française.
    
  } else if (mode==1){
    
    printf("Le fichier va être compréssé sans utilisé Huffman: le fichier ne contient qu'un charactére différent.\n");
    
    compression2(argv[1],argv[2]);
    
  } else if (mode==0){
    
    initArbre();
    nb=initNoeuds();
    printArbre(nb);
    parcoursCode(nb-1,"");
    printCodeCharEtMoyenne();   
    compression(argv[1],argv[2],nb-255);
    } 
  return 0;
}
