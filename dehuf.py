#!/usr/bin/env python3

import os, sys, re, codecs, copy, time


def recreerDos (fichier) :

    arbo = []
    arboTitre = []
    found = 0
    tailleTitreFichier = 0
    tailleTitreDossier = 0
    indiceDossier = 0
    separateurDeFichier= ['&','&','H','&','U','&','F','&']
    dernier_8_Octets = []
    posDansLeFichier = 0
    titre = ""
    etatd = 0
    etatf = 0
    mode = 0
    pos = 0
    fr = codecs.open(fichier, 'r', 'utf-8')

    while 1 :
        
        pos+=1
        
        buf = fr.read(1)

        if pos==1 :
            
            if buf=="&" :
                
                if (indiceDossier==0) :
                    
                    mode = 0
                    print ("mode : ")
                    print(mode)
                    
                elif (indiceDossier>0) :
                    
                    if arbo.count(indiceDossier)==1 :
                        
                        print("Avant destruction arbo  = ")
                        print(arbo[:])
                        print(arboTitre[:])
                        arbo.remove(arbo[-1])
                        arboTitre.remove(arboTitre[-1])
                        print("Aprés dest arbo[:] = ")
                        print(arbo[:])
                        print(arboTitre[:])
                        pos = 0
                        
                        if arbo[:] == [] :
                            break
                    
                        
                    elif arbo.count(indiceDossier)==0 :
                        
                        arbo.append(indiceDossier)
                        print("arbo[:] = ")
                        print(arbo[:])
                        mode = 1
                        print ("mode : ")
                        print(mode)
                        
                    indiceDossier = 0
                    
            else :
                
                indiceDossier=(10*indiceDossier)+int(buf)
                print("indiceDossier : ")
                print(indiceDossier)
                pos = 0
                    
                    
        elif pos>1 :
            
            if mode==0 :
                
                if not(found) :
                    
                    if buf=="_" :
                        
                        found = 1
                        
                    else :
                        
                        tailleTitreFichier=(10*tailleTitreFichier)+int(buf)
                        print ("tailleTitreFichier = ")
                        print (tailleTitreFichier)

                elif found :
            
                    if tailleTitreFichier > 0 :

                        titre=titre+buf
                        tailleTitreFichier-=1
                        

                    elif tailleTitreFichier==0 :

                        posDansLeFichier += 1

                        if posDansLeFichier == 1 :

                            verif = copy.deepcopy(arboTitre)
                            trajet = "/".join(verif)+"/"
                            os.popen("touch "+trajet+titre)
                            time.sleep(0.001)
                            print("-------")
                            print (trajet+titre)
                            print("-------")
                            verif[:] = []
                            trajet = ""
                        
                        if (len(dernier_8_Octets))==8 :
                            
                            verif = copy.deepcopy(arboTitre)
                            trajet = "/".join(verif)+"/"
                            fw = open(trajet+titre , 'a')
                            verif[:] = []
                            trajet = ""
                            fw.write(dernier_8_Octets[0])
                            del dernier_8_Octets[0]

                        dernier_8_Octets.append(buf)
                        
                        if dernier_8_Octets == separateurDeFichier :
                        
                            titre = ""
                            found = 0
                            pos = 0
                            dernier_8_Octets[:] = []
                            posDansLeFichier = 0
                                
            elif mode==1 :
                
                if not(found) :
                    
                    if buf=="_" :
                        
                        found = 1
                        print("found")
                        
                    else :
                        
                        tailleTitreDossier=(10*tailleTitreDossier)+int(buf)
                        print("tailleTitreDossier = ")
                        print(tailleTitreDossier)
                        
                elif found :
                    
                    if tailleTitreDossier > 0 :

                        titre=titre+buf
                        tailleTitreDossier-=1

                    elif tailleTitreDossier==0 :
                        verif = copy.deepcopy(arboTitre)
                        trajet = "/".join(verif)+"/"
                        
                        if trajet != "/" :
                            
                            os.popen("mkdir "+trajet+titre)

                        else :
                            
                            os.popen("mkdir "+titre)
                            
                        arboTitre.append(titre)
                        verif[:] = []
                        trajet = ""
                        titre = ""
                        found = 0
                        pos = 0
                        
                
        if not buf: break


recreerDos(sys.argv[1])
