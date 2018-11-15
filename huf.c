#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>

//declaration d'un tableau de frequence
float frequence[256];

//declaration d'un tableau pour les codes binaires 
 char *bin[257];

//definition de la structure Noeud
typedef struct {float freq;int fg; int fd;int pere;}Noeud;

//declaration d'un tableau de noeud qu'on appelle arbre
Noeud arbre[511];

unsigned int mode = 0;
unsigned int tailleF=0;
//indice pour le nombre de carctere contenu dans le fichier

void calculFrequence(char * fichier){
  char buffer[1];
  unsigned int buffer2;
  FILE* fd= fopen(fichier,"r");
  unsigned int nbdif=0;
  unsigned int cmpt = 0;//indice pour parcourir le tableau de frequence
                      //et pouvoir assigné une frequence pour chaque char
  if(fd){
    while(fread(buffer,1,1,fd)){
      //je lis dans fd 1 caractere de taile 1 octet sur buffer
      printf("%c %i\n",buffer[0],buffer[0]);
      // j'affiche le caractere lu et son code ASCII
      if (buffer[0]<0){
	buffer2=128+(128 + buffer[0]);
      }
      if (buffer[0]==0){
	buffer2=0;
      }
      buffer[0]>=0 ? frequence[buffer[0]]++ : frequence[buffer2]++;
      
      // en lisant le char j'incremente sa cellule
      //associé dans le tablau frequence
      tailleF++; 
    }
    
    while(cmpt<256){
      if(frequence[cmpt] != 0){
	frequence[cmpt]/=tailleF;
	nbdif++;
	//	printf("%u-> %f\n",cmpt,frequence[cmpt]);
	printf("%i --> %f\n",cmpt,frequence[cmpt]);
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


//modifier le pere fils et frequence des minimums et noeuds associé

void modifierPereFilsFrequence(int indice1,int indice2,unsigned int k){
  arbre[indice1].pere=k;
  arbre[indice2].pere=k;
  arbre[k].fd= indice1;
  arbre[k].fg= indice2;
  arbre[k].freq= arbre[indice2].freq + arbre[indice1].freq;
}

//initialisation de mon arbre

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
  
//recherche des mins et creation des noeuds
unsigned int initNoeuds(){
  //calcul des minimums
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
    //printf("le nombre de noeud mnt avant incrementation est : %u\n",nbNoeuds);
    nbNoeuds++;
    // printf("le nombre de noeud mnt apres incrementation est : %u\n",nbNoeuds);
  }
  //printf("le nombre de noeud total est : %u\n",nbNoeuds-255);
  return nbNoeuds;
}


//generation de code binaire(methode recursive)

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
      /* if(arbre[arbre[arbre[nbNoeuds].pere].fd].fd==-1)*/
      printf("%i %c->%s\n",arbre[arbre[nbNoeuds].pere].fd,
	     arbre[arbre[nbNoeuds].pere].fd,code);
    }
    
    if (nbNoeuds==0){
      bin[0]=strdup(code);
    }else{
      bin[nbNoeuds]=strdup(code);//j'alloue un espace mémoire de la taille strlen(code)
      //dans la cellule numero nbNoeuds de tableau bin et je copie dedans la chaine
      //de caractère code
    }
  }
}





void compression(char *fichier1 ,char *fichier2,unsigned int nb){
  char buffer[1]; //buffer de lecture
  char buffer2[262];//buffer d'écriture
  unsigned int ip=0;
  unsigned int i=0;
  unsigned int r=0;//entier represantant la chaine de caractere binaire
  unsigned int binT=0;
  unsigned int binV=0;
  //de longueur 8
  FILE*fr=fopen(fichier1,"r"); //pointer of reading file
  FILE*fw=fopen(fichier2,"w");//pointer of writing in the file
  if(fr && fw){
    //entête-----------------------------------------------
    fputc(mode,fw);
    fputc(tailleF,fw);
    fputc(nb,fw);
    
    for(unsigned int parc;parc<256;parc++){
      if(frequence[parc]!=0){
	
	fputc(parc,fw);
	
	while(binT<strlen(bin[parc])){ 
	  buffer2[binT]=bin[parc][binT];
	  binT++;
	}
	for(int e=binT;e<8;e++){
	buffer2[e]='0';
	}
	for(int j=7;j>=0;j--){
	  const char binbuf=buffer2[j];
	  binV=binV+(pow(2,7-j)*atoi(&binbuf));
	}
	
	fputc(binV,fw);
	binT=0;
	binV=0;
      }
    }
    //-----------------------------------------------------
    while(fread(buffer,1,1,fr)){ //premiere boucle de lecture dans le fichier1
      // printf("%c ---> lu \n",buffer[0]);
      i=0;
      
      while(i<strlen(bin[buffer[0]])){    //tant que i est inferieur a la 
	buffer2[ip]=bin[buffer[0]][i];     // longueur de code binaire associé
	//	printf("%c à pour binaire: %c \n",buffer[0],bin[buffer[0]][i]); //au caractere lu
	i++;
	ip++;//on copie ce code dans buffer2
	
      }
      while(ip>=8){               //je calcule r pour les 8 premieres cases de buffer2
	//	printf("%i \n",ip);
	for(int j=7;j>=0;j--){
	  const char buf=buffer2[j];
	  //	  printf("buf = %c \n",buf);
	  r=r+(pow(2,7-j)*atoi(&buf));
	  //	  printf("%i \n",r);
	}
	//	printf("la valeur écrite dans le fichier est %i \n",r);
	fputc(r,fw);            //ecrir r dans fichier2
	for(int k=0;k<254;k++){
	  buffer2[k]=buffer2[k+8]; // supprimer les 8 premieres cases 
	}
	ip=ip-8; //je diminue alors mon indice de 8
	r=0;
	//	printf("%i \n",ip);
      }
    }
    if(ip!=0){
      for(int e=ip;e<8;e++){
	buffer2[e]='0';
	ip++;
      }                                     //apres si il reste des case non vide
      //   printf("%i \n",ip);
      for(int j=7;j>=0;j--){
	const char buf=buffer2[j];
	//	printf("buf = %c \n",buf);
	r=r+(pow(2,7-j)*atoi(&buf));
	//	printf("%i \n",r);
      }                                   //mentionné ci-dessus
      fputc(r,fw);
      for(int w=0;w<254;w++){
	buffer2[w]=buffer2[w+8];
      }
      ip-=8;
      // printf("%i \n",ip);
    }
  }else{
    printf("erreur dans l'ouverture de flux de lecture et d'ecriture de fichier");
  }
  fclose(fr); //enfin la fermeture des flux de lecture
  fclose(fw); // et d'ecriture
}

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
    //printf("%i %i %i \n",mode,i,tailleF);
    while (tailleF>base){
     fputc(base,fw);
     tailleF=tailleF-base;
     //printf("%c",base);
    }
    fputc(tailleF,fw);
    //printf("%c \n",tailleF);
    
    
  }else{
    printf("erreur dans l'ouverture du flux d'ecriture de fichier");
  }
  
  fclose(fw);
}
 
     
//afficher mon arbre
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
    //Peut être voir autre compression
  } else if (mode==1){
    printf("Le fichier va être compréssé sans utilisé Huffman: le fichier ne contient qu'un charactére différent.\n");
    compression2(argv[1],argv[2]);
    // à faire la representation de la non information
  } else if (mode==0){
    initArbre();
    nb=initNoeuds();
    printArbre(nb);
    parcoursCode(nb-1,"");
    compression(argv[1],argv[2],nb-256);
  } 
  return 0;
}
