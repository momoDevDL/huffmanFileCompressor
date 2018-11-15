#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>

void decompression(char* fichier){
  char buffer[1];
  unsigned int binbuf;
  unsigned int i=0;
  unsigned int j=0;
  unsigned int tailleF=0;
  unsigned int nb=0;
  unsigned int trouve=0;
  int tb=0;
  char tc;
  unsigned int ii=0;
  char *bin[257];
  bin[256]=NULL;
  char* code="";
  FILE*fr=fopen(fichier,"r"); //pointer of reading file
  if(fr){
   
    while(fread(buffer,1,1,fr)){ //premiere boucle de lecture dans le fichier1
      i++;
      if (buffer[0]<0){
	binbuf=128+(128 + buffer[0]);
      }else{
	binbuf=buffer[0];
      }
      //entête décompression-----------------------------------------------
      if(i==2){
	tailleF=binbuf;
      }else if(i==3){
        nb=binbuf;
      }else if((i>3)&&(nb>0)){
	if(ii==0){
	  tc=binbuf;
	}else if(ii==1){
	  tb=binbuf;
	}else if(ii>1){
	  j=8;
	  while ((tb>0)||(j>0)){
	    j--;
	    char *ncode=(char*)malloc(strlen(code)+1);
	    strcpy(ncode,code);
	    if(binbuf>=(1*pow(2,j))){
	      ncode[strlen(code)]='1';
	      binbuf=binbuf-(1*pow(2,j));
	    }else{
	      ncode[strlen(code)]='0';
	    }
	    ncode[strlen(code)+1]='\0';
	    tb--;
	  }
	}else if(tb<=0){
	  bin[tc]=strdup(code);
	  ii=0;
	  nb--; 
	}
	//-----------------------------------------------------------------
      }else if((i>3)&&(nb<=0)){
	 j=8;
	 trouve=0;
	 while (j>0){
	    j--;
	    char *ncode=(char*)malloc(strlen(code)+1);
	    strcpy(ncode,code);
	    if(binbuf>=(1*pow(2,j))){
	      ncode[strlen(code)]='1';
	      binbuf=binbuf-(1*pow(2,j));
	    }else{
	      ncode[strlen(code)]='0';
	    }
	    ncode[strlen(code)+1]='\0';
	    
	    while ((trouve==0)&&(j<256)){
	      if ((strcmp(bin[j],code))==0){
		trouve=1;
	      }
	      j++;
	    }
	    if (trouve!=0){
	      printf("%c",j);
	      trouve=0;
	      code=(char*)malloc(1);
	      code="";
	    }
	 } 
      }
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
  unsigned int nbtotal;
  FILE* fd= fopen(fichier,"r");
  
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
    
    printf("%i\n",nbtotal);
  }else{
    printf("le flux de lecture de fichier n'a pas ete bien ouvert");
  }
  /* while (nbtotal > 0){
    printf("%c",c);
    nbtotal--;
    }*/
  printf("\n");
  printf("FIN\n");
}


int main(int argc,char* argv[]){
  unsigned int mode=2;
  FILE* fd= fopen(argv[1],"r");
  if(fd){
    mode=fgetc(fd);
  }else{
    printf("le flux de lecture de fichier n'a pas ete bien ouvert");
  }

  if (mode==1){
    decompression1(argv[1]);
  } else if(mode==0){
    decompression(argv[1]);
  }
}

