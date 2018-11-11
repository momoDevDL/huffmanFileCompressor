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
unsigned int cmpt = 0;//indice pour parcourir le tableau de frequence
                      //et pouvoir assigné une frequence pour chaque char  
unsigned int i=0;//indice pour le nombre de carctere lu
void calculFrequence(char * fichier){
  char buffer[1];
  FILE* fd= fopen(fichier,"r");
  if(fd){
    while(fread(buffer,1,1,fd)){ //je lis dans fd 1 caractere de taile 1 octet sur buffer
      printf("%c %i\n",buffer[0],buffer[0]);// j'affiche le caractere lu et son code ASCII
      frequence[buffer[0]]++;// en lisant le char j'incremente sa cellule
      //associé dans le tablau frequence
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
  printf("le nombre de noeud total est : %u\n",nbNoeuds-255);
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
    if(/*arbre[arbre[arbre[nbNoeuds].pere].fg].fg==-1 &&*/ code[strlen(code)-1]=='0')
      printf("%c->%s\n",arbre[arbre[nbNoeuds].pere].fg,code);
    else{
      /* if(arbre[arbre[arbre[nbNoeuds].pere].fd].fd==-1)*/
	printf("%c->%s\n",arbre[arbre[nbNoeuds].pere].fd,code);
    }
    bin[nbNoeuds]=strdup(code);//j'alloue un espace mémoire de la taille strlen(code)
    //dans la cellule numero nbNoeuds de tableau bin et je copie dedans la chaine
    //de caractère code
  }
}

void compression(char *fichier1 ,char *fichier2){
  char buffer[1]; //buffer de lecture
  char buffer2[262];//buffer d'écriture
  int i;
  int r=0;//entier represantant la chaine de caractere binaire
  //de longueur 8
  FILE*fr=fopen(fichier1,"r"); //pointerof reading file
  FILE*fw=fopen(fichier2,"w");//pointer of writing in the file
  if(fr && fw){
    while(fread(buffer,1,1,fr)){ //premiere boucle de lecture dans le fichier1
      i=0;
      while(i<strlen(bin[buffer[0]])){    //tant que i est inferieur a la 
	buffer2[i]=bin[buffer[0]][i];     // longueur de code binaire associé 
	i++;                              //au caractere lu on copie ce code dans buffer2
	printf("%c ---> lu \n",buffer[0]); 
	}
      while(i>=8){               //je calcule r pour les 8 premieres cases de buffer2
	  for(int j=0;j<8;j++){
	    r+=pow(buffer2[j],2); 
	  }
	  fputc(r,fw);            //ecrir r dans fichier2
	  for(int k=0;k<254;k++){
	    buffer2[k]=buffer2[k+7]; // supprimer les 8 premieres cases 
	  }
	  i-=8; //je diminue alors mon indice de 8 
	}
	if(i!=0){
	  for(int e=i;e<8;e++){
	    buffer2[e]='0';
	  }                                     //apres si il reste des case non vide
	  for(int f=0;f<8;f++){                 //je remplie avec 0 les cases qui reste
	    r+=pow(buffer2[f],2);             // pour atteinde 8cases et je refait l'opretion 
	  }                                    //mentionné ci-dessus
	  fputc(r,fw);
	  for(int w=0;w<254;w++){
	    buffer2[w]=buffer2[w+7];
	  }
	  i-=8;
	}
    }
  }else{
    printf("erreur dans l'ouverture de flux de lecture et d'ecriture de fichier");
  }
  fclose(fr); //enfin la fermeture des flux de lecture
  fclose(fw); // et d'ecriture
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
  initArbre();
  nb=initNoeuds();
  printArbre(nb);
  parcoursCode(nb-1,"");
  compression(argv[1],argv[2]);
  return 0;
}
