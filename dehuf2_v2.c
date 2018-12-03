#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>


void decompression1(char* fichier){
  char buffer[1];
  unsigned int i=1;
  char car;
  unsigned int nbtotal=0;
  FILE* fd= fopen(fichier,"r");
  
  if(fd){
    while(fread(buffer,1,1,fd)){
      printf("%c %i\n",buffer[0],buffer[0]);
      if (i==2){
	car=buffer[0];
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

  while (nbtotal > 0){
    printf("%c",car);
    nbtotal--;
    }
  printf("\n");
  printf("FIN\n");
  fclose(fd);
}

void decompression(char * fichier){
  unsigned int tailleF=0 ;
  unsigned int nbdiff;
  unsigned int binT;
  unsigned int pos;
  unsigned int pos2;
  unsigned int binV;
  unsigned char buffer[1];
  unsigned char  car;
  unsigned int tmp;
  unsigned int  tmp2;
  unsigned char buffer2;
  FILE *fr=fopen(fichier,"r");
  char *bin[257];
  for(unsigned int k=0;k<=256;k++){
    bin[k]=NULL;
    }
  unsigned char*binaire=(unsigned char*)malloc(8*sizeof(char));
  unsigned char* binaire1 ;
  //============ENTETE============================================
  if(fr){
    printf("====================on commence la phase de decompression de l'entete======================== \n");
    fseek(fr,1,SEEK_CUR);
    unsigned int pos_lecture=fgetc(fr);
    while(pos_lecture==255){
      tailleF+=pos_lecture;
      pos_lecture=fgetc(fr);    
    }
    
    tailleF+=pos_lecture;
    printf("la tailleF : %d\n",tailleF);
    
    nbdiff=fgetc(fr);
    printf("le nbdiff : %d\n",nbdiff);
    
    pos=0;
      
    while(nbdiff>0 && fread(buffer,1,1,fr)){
      printf("buffer[0]=== %u\n",buffer[0]);
      if(pos==0){
	car=buffer[0];
	pos++;
	 
      }else{
	if(pos==1){
	  binT=buffer[0];
	  pos++;
	}else{
	  if(pos==2){
	    bin[car]=(char*)malloc((binT+1)*sizeof(char));
	    bin[car][binT]='\0';
	    binV=8;
	    while(binV>0){
	      binaire[binV-1]= buffer[0] %2 + '0';
	      buffer[0] /=2;
	      binV--;
	    }
	    for(int i=0;i<binT;i++){
	      bin[car][i]=binaire[i];
	    }
	    binaire=(unsigned char*)realloc(binaire,8);
	    pos=0;
	    nbdiff--;
	    printf("bin[%u]= %s\n",car,bin[car]);
	  }
	}
      }
    }
    //========================decompression 1 ====================================
    
 printf("============On commence la phase de decompression============= \n");
    binaire1=(unsigned char*)malloc((8*tailleF)*sizeof(unsigned char));
    tmp=7;
    while(fread(buffer,1,1,fr)){
      buffer2=buffer[0];
      printf("buffer2 -> %u\n",buffer2);
      if(pos==0){
	while( pos<=tmp ){
	  binaire1[tmp-pos]= buffer2 % 2+'0';
	  buffer2/= 2;
	  pos++; 
	}
	tmp+=8;	
      }else{
	tmp2=tmp;
	while(pos<=tmp){
	  binaire1[tmp]=buffer2 %2+'0';
	  buffer2 /=2;
	  tmp--;
	}
	pos+=8;
	tmp=tmp2+8;
      }
    }
    printf("%s\n",binaire1);
    for(unsigned int k=0;k<256;k++){
      printf("bin[%u]==%s\n",k,bin[k]);
    }
    tmp=0;
    tmp2=0;
    pos2=0;
    unsigned int cmpt=0;
    printf("==========le tableau bianire1 a bien été créé============\n");
    printf("\n");
    while((cmpt<tailleF && tmp<(tailleF*8))){
      while( pos2<=255 &&( bin[pos2]==NULL || binaire1[tmp]!=bin[pos2][tmp2])){
	  pos2++;
	}
      if(bin[pos2]!=NULL && binaire1[tmp]==bin[pos2][tmp2]){
	  while(tmp2<strlen(bin[pos2]) && binaire1[tmp]==bin[pos2][tmp2]){
	    tmp++;
	    tmp2++;
	  }
	  if(tmp2==strlen(bin[pos2])){
	    printf("%c",pos2);
	    cmpt++;
	    tmp2=0;
	    pos2++;
	  }else{
	    tmp-=tmp2;
	    tmp2=0;
	    pos2++;
	  }
      }else{
	  pos2=0;
	}
      }
    printf("\n");
    printf("==============le fichier a bien été décompréssé=============================\n");
  }else{

    printf("erreur dans l'ouverture de flux de lecture ");
  }
}


int main(int argc,char* argv[]){
  unsigned int mode=2;
  FILE* fd= fopen(argv[1],"r");
  if(fd){
    mode=fgetc(fd);
  }else{
    printf("le flux de lecture de fichier n'a pas ete bien ouvert");
  }
  fclose(fd);
  if (mode==1){
    decompression1(argv[1]);
  } else if(mode==0){
    decompression(argv[1]);
  }
}

