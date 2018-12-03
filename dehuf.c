#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>



typedef struct {float freq;int fg; int fd;int pere;}Noeud;
Noeud arbre[511];
unsigned int maxEntete=0;
unsigned int mode=2;
unsigned int tailleF=0;


void  initArbre(){
  
  for(int j=0;j<511;j++){
    
    arbre[j].pere=-1 ;
    arbre[j].fg=-1;
    arbre[j].fd=-1;
    arbre[j].freq=0.0;
    
  }
}

void printArbre(unsigned int nb){
  
  for(unsigned int i=0;i<nb;i++){
    
    printf("%u  : %i %i %i %f\n",i, arbre[i].pere,arbre[i].fg,arbre[i].fd,arbre[i].freq);
    
  }
}


unsigned int decompressionEntete(char* fichier){
  unsigned char buffer[1];
  unsigned int binbuf=0;
  unsigned int i=0;
  unsigned int i2=2;
  unsigned int nb=255;
  unsigned int pereNb=0;
  unsigned int entete=1;
  unsigned int bufPereNb=0;
  unsigned int bufTailleF=0;
  unsigned int verifTailleF=0;
  FILE*fr=fopen(fichier,"r"); 
  
  if(fr){
   
    while((fread(buffer,1,1,fr))&&(entete)){ 
      
      i++;

      if (buffer[0]>=0){
	
	binbuf=buffer[0];
	
      }
      
      //entête décompression-----------------------------------------------
      
      if(i==2){
	
	nb+=binbuf;
	pereNb=nb;
	
      }else if((i>=3)&&!(verifTailleF)){
	
	bufTailleF+=binbuf;
	
        if (binbuf<255){
	  
	  tailleF=bufTailleF;
	  verifTailleF=1;
	  
	}
	
      }else if((arbre[256].fd)!=-1){
	
	entete=0;
	
      }else if((i>3)&&(nb>0)&&(entete)){

	if(i2==4){
	  
	  if(binbuf==255){
	    
	    bufPereNb=binbuf;
	    i2--;
	    
	  }else if (binbuf<255){
	    
	    arbre[pereNb].fg=binbuf;
	    arbre[binbuf].pere=pereNb;
	    i2=i2-2;
	    
	  }
	  
	}else if(i2==3){
	  
	  arbre[pereNb].fg=binbuf+255;
	  arbre[binbuf+255].pere=pereNb;
	  i2--;
	  
        }else if(i2==2){
	  
	  if(binbuf==255){
	    
	    bufPereNb=binbuf;
	    i2--;
	    
	  }else if (binbuf<255){
	    
	    arbre[pereNb].fd=binbuf;
	    arbre[binbuf].pere=pereNb;
	    i2=i2-2;
	    
	  }
	  
        }else if(i2==1){
	  
	  arbre[pereNb].fd=binbuf+255;
	  arbre[binbuf+255].pere=pereNb;
	  i2--;
	  
	}
	
	if(i2==0){
	  
	  pereNb--;
	  i2=4;
	}
      }
      //----------------------------------------------------------------
    }
    
    
  }else{
    
    printf("erreur dans l'ouverture de flux de lecture et d'ecriture de fichier");
  }
  maxEntete=i;
  fclose(fr);
  
  return nb;
  
}

void decompression(char* fichier,unsigned int nb,char* dest){
  char buffer[1];
  unsigned int binbuf=0;
  unsigned int i=0;
  unsigned int j=0;
  unsigned int r=nb;
  unsigned int tailleFA=0;
  FILE*fr=fopen(fichier,"r");
  FILE*fw=fopen(dest,"w");
  
  if(fr){

    printf("-----------------------------------------------\n");
    printf("Debut de la decompression.\n");
    printf("-----------------------------------------------\n");

    while(fread(buffer,1,1,fr)){ 
      
      i++;
      
      if (i>=maxEntete){
	
	if (buffer[0]>=0){
	  
	  binbuf=buffer[0];
	  
	}else if (buffer[0]<0){
	  
	  binbuf=128+(128 + buffer[0]);
	  
	}
	j=8;
	
	while (j>0){
	  
	  j--;
	  
	  if (tailleFA!=tailleF){
	    
	    if((r<256)&&(r>=0)){
	      
	      if(dest!=NULL){

		if (fw){

		  fputc(r,fw);
		  
		}
		
	      }
	      
	      printf("%c",r);
	      r=nb;
	      tailleFA++;
	      
	    }
	    
	    if(binbuf>=(1*pow(2,j))){
	      
	      r=arbre[r].fd;
	      binbuf=binbuf-(1*pow(2,j));
	      
	    }else if ((arbre[r].fg)!=-1){
	      
	      r=arbre[r].fg;
	    }
	  }
	} 
      }
    }
    
    if (tailleFA==tailleF){
      
      printf("\n-----------------------------------------------\n");
      printf("Decompression terminé, le fichier alors généré est de taille %i Octets. \n",tailleFA);
      printf("-----------------------------------------------\n");
      
    }
   
 
  }else{
    
    printf("erreur dans l'ouverture de flux de lecture et d'ecriture de fichier");
    
  }
  
  fclose(fr); 
}

void decompression1(char* fichier){
  char buffer[1];
  unsigned int i=1;
  unsigned int c;
  unsigned int nbtotal=0;
  unsigned int nbTotal=0;
  FILE* fd= fopen(fichier,"r");
  
  if(fd){

    printf("-----------------------------------------------\n");
    printf("Debut de la decompression.\n");
    printf("-----------------------------------------------\n");
    
    while(fread(buffer,1,1,fd)){
      
      if (i==1){
	
      }else if (i==2){
	
	c=buffer[0];
	
      }else if (i>=3){
	
	if (buffer[0]<0){
	  
	  nbtotal+=(128+(128+buffer[0]));
	  
	}else if (buffer[0]>=0){
	  
	  nbtotal+=buffer[0];
	  
	}
      }
      i++;
    }
    
    nbTotal=nbtotal;
    
  }else{
    
    printf("le flux de lecture de fichier n'a pas ete bien ouvert");
    
  }
   while (nbtotal > 0){
     
    printf("%c",c);
    
    nbtotal--;
    }
   
   printf("\n-----------------------------------------------\n");
   printf("Decompression terminé, le fichier alors généré est de taille %i Octets. \n",nbTotal);
   printf("-----------------------------------------------\n");
  
}

int main(int argc,char* argv[]){
  unsigned int nb;
  FILE* fd= fopen(argv[1],"r");
  
  if(fd){
    
    mode=fgetc(fd);
    
  }else{
    
    printf("le flux de lecture de fichier n'a pas ete bien ouvert.\n");
    
  }

  if (mode==0){
    
    initArbre();
    
    nb=decompressionEntete(argv[1]);

    if (argv[2]!=NULL){

      decompression(argv[1],nb-1,argv[2]);
      
      printf("Le fichier a été décompréssé dans: %s \n",argv[2]);
      
    } else {
      
      decompression(argv[1],nb-1,NULL);
      
    }
    
  } else if(mode==1){
    
    decompression1(argv[1]);
    
  } else if(mode==2){
    
    printf("Erreur décompression impossible.\n");
    
  }
}

