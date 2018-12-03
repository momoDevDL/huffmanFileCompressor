#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>

void decompression(char * fichier){
  unsigned int tailleF=0 ;
  unsigned int nbdiff;
  unsigned int binT;
  unsigned int pos;
  unsigned int binV;
  unsigned char buffer[1];
  unsigned char  car;
  FILE *fr=fopen(fichier,"r");
  unsigned char *bin[257];
  for(unsigned int k=0;k<=257;k++){
    bin[k]=NULL;
  }
  unsigned char*binaire=(unsigned char*)malloc(8*sizeof(unsigned char));
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
	   
	  pos++;
	}else{
	  if(pos==2){
	    bin[car]=(unsigned char*)malloc((binT+1)*sizeof(char));
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
	    printf("bin[%c]= %s\n",car,bin[car]);
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
	while(pos2Decompression<cmpt){
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
}
 
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
  }
  
}

