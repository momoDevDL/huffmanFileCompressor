#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
 
/*!
 * \file dehufv2.c
 * \brief Décompresseur de fichier avec la methode de Huffman 
 * \author Antoine Afflatet / Mohamed Masbah Abou Laich
 * \version 0.2
 */


 /*!
 * \brief declaration de la fonction de decompression 
 * selon le methode 0 
 */
void decompression(char * fichier){
  unsigned int tailleF=0 ;
  unsigned int nbdiff;
  int binT;
  unsigned int binT2;
  unsigned int pos;
  unsigned int binV=0;
  unsigned int binV2=0;
  unsigned char buffer[1];
  unsigned char  car;
  FILE *fr=fopen(fichier,"r");
  unsigned char *bin[257];
  for(unsigned int k=0;k<=257;k++){
    bin[k]=NULL;
  }
  unsigned char*binaire=(unsigned char*)malloc(20*sizeof(unsigned char));
  unsigned char*binaire1=(unsigned char*)malloc(262*sizeof(unsigned char));
  for(int p=0;p<262;p++){
    binaire1[p]='0';
  }
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
    printf("le nbdiff : %d\n",nbdiff);
    
    pos=0;
      
    while(nbdiff>0 && fread(buffer,1,1,fr)){
      /*	if(buffer[0]<0){
		buffer2=128+(128+buffer[0]);
	 
		}else{
		buffer2=buffer[0];
	 
		}*/
      printf("buffer[0]=== %u\n",buffer[0]);
      if(pos==0){
	car=buffer[0];
	pos++;
	 
      }else{
	if(pos==1){
	  binT=buffer[0];
	  binT2=binT; 
	  pos++;
	  bin[car]=(unsigned char*)malloc((binT+1)*sizeof(unsigned char));
	  bin[car][binT]='\0';
	}else{
	  if(pos==2){
	    binV=binV2+8;
	    binV2=binV;
	    while(binV>binV2-8){
	      binaire[binV-1]= buffer[0] %2 + '0';
	      buffer[0]/=2;
	      binV--;
	    }
	    // binV++;
	    binT-=8;
	    if(binT<=0){
	    for(int i=0;i<binT2;i++){
	      bin[car][i]=binaire[i];
	    }
	    //binaire=(unsigned char*)realloc(binaire,30);
	    pos=0;
	    nbdiff--;
	    binV=0;
	    binV2=0;
	    printf("bin[%c]= %s\n",car,bin[car]);
	    }
	  }
	}
      }
    }
    //========================decompression 1 ====================================
    // printf("lol===%u",fgetc(fr));
    //	fgetc(fr);*/
    unsigned int pos2Decompression=0;
    unsigned int pos2=0;
    unsigned int nbr_lecture= 1;
    int cmpt=7;
    unsigned int tmp=0;
    printf("============On commence la phase de decompression============= \n");
    while(fread(buffer,1,1,fr)){
	
	
      car=buffer[0];
      //  printf("car -> %u\n",car);
	
	
	
      if(nbr_lecture==1){
	pos=8;
	while( pos>0 ){
	  binaire1[pos-1]= car % 2+'0';
	  car/= 2;
	  pos--;
	}
	nbr_lecture++;
	}else{
	cmpt=tmp+7;
	while( cmpt>=tmp ){
	  binaire1[cmpt]= car % 2+'0';
	  car/= 2;
	  cmpt--;
	}
	cmpt+=8;
      }
      // printf("binaire== %s\n",binaire1);
      pos=0;
      pos2=0;
      tmp=0;
      pos2Decompression=0;
	while(pos2Decompression<cmpt && tailleF>0){
	  while( pos<=255 && (bin[pos]==NULL || binaire1[pos2Decompression] != bin[pos][pos2] )){
	    pos++;
	  }
	  if( bin[pos] != NULL && binaire1[pos2Decompression]==bin[pos][pos2]){
	    while(pos2<strlen(bin[pos]) && pos2Decompression<cmpt && binaire1[pos2Decompression] == bin[pos][pos2] ){
	      pos2Decompression++;
	      pos2++;
	    }
	    if(pos2==strlen(bin[pos])){
	      if(pos2Decompression!=cmpt && tailleF>0){
	      printf("%c",pos);
	      pos++;
	      pos2=0;
	      tailleF--;
	      }else{
		tmp=pos2+1;
		pos2=pos2Decompression-pos2;
		for(int k=0;k<=254;k++){
		  binaire1[k]=binaire1[k+pos2];
		}
	      }
	    }else{
	      if(pos2Decompression==cmpt && pos2<strlen(bin[pos])){
	       	tmp=pos2+1;
		pos2=pos2Decompression-pos2;
		for(int k=0;k<=254;k++){
		  binaire1[k]=binaire1[k+pos2];
		}
		//pos2=0;
	      }else{
		pos2Decompression-=pos2;
		pos++;
		pos2=0;
	      }
	    }
	    
	  }else{
	    pos=0;
	    pos2=0;
	  }
	}
      
    }
  }else{

    printf("erreur dans l'ouverture de flux de lecture ");
  }
  fclose(fr);
}

 /*!
 * \brief declaration de la fonction de decompression 
 * selon le methode 1 
 */

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


// Programme principal et appel au fonction 
int main(int argc,char** argv){
  FILE*ff=fopen(argv[1],"r");
  unsigned int mode;
  if(ff){
    mode=fgetc(ff);
    printf("le mode est : %u\n",mode);
  }else{
    printf("l'ouverture de ff est erroné");
  }
  if (mode==0){
    decompression(argv[1]);
  }else{
    decompression1(argv[1]);
  }
  fclose(ff);
}

