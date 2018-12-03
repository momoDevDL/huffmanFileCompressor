#!/usr/bin/env python3

import os, sys, re, codecs


def debDos(fichier, dest, nom, explo) :
    
    fw = open(dest , 'a')
    longueur = str(len(nom))
    afficheExplo= str(explo)
    fw.write(afficheExplo)
    fw.write("&")
    fw.write(longueur)
    fw.write("_")
    fw.write(nom)


def finDos(fichier, dest, nom, explo) :
    
    fw = open(dest , 'a')
    afficheExplo= str(explo)
    fw.write(afficheExplo)
    fw.write("&")
                

def ecritf(fichier, dest, nom) :
    
    File=os.popen("file "+fichier)
    
    for ligne in File :
        
        typeF = re.search("^[^ ]+[^A-Z]+([^ ]+)",ligne)
        
        if typeF :
            
            print("\"file "+fichier+" == "+typeF.group(1))
            longueur = str(len(nom))
            
            if ((typeF.group(1)=="ASCII")or(typeF.group(1)=="ISO-8859")or(typeF.group(1)=="ISO-8859-15")or(typeF.group(1)=="ISO-8859-1")) :

                
                fr = open(fichier, 'r')
                fc = fr.read()
                fw = open(dest , 'a')
                fw.write("0")
                fw.write("&")
                fw.write(longueur)
                fw.write("_")
                fw.write(nom)
                fw.write(fc)
                fw.write("&&H&U&F&")
                fw.close()
                
            elif (typeF.group(1)=="UTF-8") :
                
                print ("Le fichier était en UTF-8")
                fr = codecs.open(fichier, 'r', 'utf-8')
                fc = fr.read()
                fw = open(dest , 'a')
                fw.write("0")
                fw.write("&")
                fw.write(longueur)
                fw.write("_")
                fw.write(nom)
                fw.write(fc)
                fw.write("&&H&U&F&")
                fw.close()
                             
                
            else :
                
                print ("Format de fichier non compatible avec le compresseur")
                print(typeF.group(1))
                
def parcours (repertoire, dest, explo) :
    
    print("Je suis dans " + repertoire)
    liste = os.listdir(repertoire)
    
    for fichier in liste :
        
        if os.path.isdir(repertoire + "/" + fichier)and os.access(repertoire + "/" + fichier,os.X_OK) :
            
            explo+=1
            debDos(repertoire + "/" + fichier, dest, fichier, explo)
            parcours(repertoire + "/" + fichier , dest, explo)
            finDos(repertoire + "/" + fichier, dest, fichier, explo)
            
        else :
            
            if os.path.isfile( repertoire + "/" + fichier ) :
                
                ecritf(repertoire + "/" + fichier, dest, fichier)
                
explo = 1

debDos(sys.argv[1],(sys.argv[1]+".provisoire"),sys.argv[1], explo)
parcours(sys.argv[1],(sys.argv[1]+".provisoire"),explo)
finDos(sys.argv[1],(sys.argv[1]+".provisoire"),sys.argv[1], explo)
os.popen("iconv -t iso-8859-15 "+sys.argv[1]+".provisoire  > "+sys.argv[1]+".provisoire2")
os.popen("./huf "+sys.argv[1]+".provisoire2 "+sys.argv[1]+".huf")
os.popen("rm "+sys.argv[1]+".provisoire")
os.popen("rm "+sys.argv[1]+".provisoire2 ")
