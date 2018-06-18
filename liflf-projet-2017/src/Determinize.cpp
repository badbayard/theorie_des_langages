//******************************************************************************

#include <iostream>

#include "Determinize.h"
#include "FA_tools.h"
#include "Accept.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////

// Fonction de test qui determine si etatCourants est similaire à un membre de tabCourant
// renvoie true si un membre de tabCourant est similaire a etatCourants
bool appartient (etatset_t etatCourants, map_t tabCourant)
{
	unsigned int compt=0;
	etatset_t secondaire;
	etat_t secondaire2;
	etat_t secondaire3;



	//Parcour de chaque etatset du tableau
	for(std::map<etatset_t,etat_t>::iterator it=tabCourant.begin();it != tabCourant.end();++it)
	{
		secondaire=it->first; 

		//Parcour de chaque etat de l'étatset
		if(secondaire.size() == etatCourants.size()) 		//Si les deux etatset ne font pas la meme taille, ils sont forcement different
		{
			for(std::set<etat_t>::iterator it3=secondaire.begin();it3 != secondaire.end();++it3)
			{
				secondaire2=(*it3);
				//On parcour tout les etats de l'état courant, si l'un d'eu est similaire a secondaire2, on incremente le compteur
				for(std::set<etat_t>::iterator it2=etatCourants.begin();it2 != etatCourants.end();++it2)
				{
					
					secondaire3=(*it2);
					if (secondaire2 == secondaire3)
						compt++;
				}
			}
				//Si on a compté autant d'élement similaire entre l'etat courant et un étatset du tableau, alors on renvoie vrai,
			// sinon on continue le parcour du tableau sur un nouveau etatset
			if(compt == etatCourants.size())
			{
				return true;
			}else {
				compt=0;
			}			
		}
		
		
	}
	return false;
}


//Fonction qui test si un membre de l'état courant est un état final
bool estFinal(etatset_t etatCourants,etatset_t finaux)
{
	etat_t secondaire;
	etat_t secondaire2;
	for(std::set<etat_t>::iterator it=etatCourants.begin();it != etatCourants.end();++it)
	{
		secondaire=(*it);
		for(std::set<etat_t>::iterator it2=finaux.begin();it2 != finaux.end();++it2)
		{
				
			secondaire2=(*it2);
			if (secondaire2 == secondaire)
				return true;
		}
	}
	return false;
}


sAutoNDE Determinize(const sAutoNDE& at){
        //Declaration des éléments necessaires

        sAutoNDE r;   	//Reservation d'espace pour le nouvelle automate qu'on va créer  
    	etatset_t etatCourants;		//Destiné à contenir l'état courant avec lequel on remplis l'automate
    	map_t tabCourant;			//Destiné à stocker les differents états selon leur ordre d'arrivé 
    	size_t indice=0;				//Indice qui indique le numero de l'état qu'on va remplir
    	size_t indiceetat=0;			//Indice qui indique le prochain numero de l'état qu'on va mettre dans état courant
    	bool poubelle=false;		// Booleen qui indique si il y a besoin d'un etat poubelle
    	char str;					// Caractere qui va contenir les symboles utilisés, permet la conversion d'un nombre en symbole pour la fonction Delta
    	etatset_t etatSecondaire;	//Destiné à contenir le retour de Delta, contient plusieurs états accessible depuis un même symbole
    	etat_t etatSecondaire2;		//Destiné à contenir chaque état de etatSecondaire (permet la conversion d'un iterator vers un etat_t)
    	


    	etatCourants.insert(at.initial);	//Le premier état courant est l'état initial
    	Fermeture(at,etatCourants);			// On rajoute dans étatCourant tout les états accessible sans lire de symbole

    	if(estFinal(etatCourants,at.finaux))
    	{
    		r.finaux.insert(0);
    	}

    	tabCourant.insert ( std::pair<etatset_t, etat_t>(etatCourants,indice) ); //Etant donné que c'est le premier état sur lequel on travail
    																			//on l'insert dans le tableau sans verification

    	//
		for(size_t i=0;i<at.nb_symbs;i++) // Pour chaque symbole, on remplis les transition de l'état initial
		{
			str=i+'a';
	        etatSecondaire = Delta(at,etatCourants,str); //On place dans EtatSecondaire les états atteints en lisant le symbole str
	        if (etatSecondaire.size() == 0)
	        {
	        	poubelle = true;						// Si en lisant un caractere on a aucune reponse, il faut créer un "état poubelle"
	            	
	            //Mise a jour de la taille de r.trans, même si on mets rien, il faut reserver une case
	            if(r.trans.size() <= indice)
	            {
	            	std::vector<etatset_t> steack1;
	            	r.trans.push_back(steack1);
	            }
	            if(r.trans[indice].size() <= i)
	            {
	            	etatset_t steack;
	            	r.trans[indice].push_back(steack);

	            }
	        }
	        else
	        {
	            for(std::set<etat_t>::iterator it=etatSecondaire.begin();it != etatSecondaire.end();++it)
	            {
	            	etatSecondaire2=(*it);

	            	//Mise a jour de la taille de r.trans, 
	            	if(r.trans.size() <= indice)
	            	{
	            		std::vector<etatset_t> steack1;
	            		r.trans.push_back(steack1);
	            	}
	            	if(r.trans[indice].size() <= i)
	            	{
	            		etatset_t steack;
	            		r.trans[indice].push_back(steack);
					}

	            	r.trans[indice][i].insert(etatSecondaire2); //Une fois assuré que le tableau de transition est assez grand, on insert tout les états atteints
	            		
	            }

	        }	                    
	    }
	    indice++; 
	    					//Incrementer indice signifie qu'on va changer d'état courant et donc remplir les transitions d'un autre état

    	do
        {
			for(size_t j=0;j<at.nb_symbs;j++) //Parcour de chaques états atteints par les états deja tester (indiceetat) 
            {								//Lorsque indiceetat est a zero, on parcour les états atteint par l'état initial précedement.
            								// c'est pourquoi l'état initial est a part
            
            	if(r.trans[indiceetat][j].size() != 0) 
            	{
            		etatCourants=r.trans[indiceetat][j]; //On charge dans etatCourant le prochain etat sur lequel on va travailler
            		
	            	if(appartient(etatCourants,tabCourant)) //Si l'état courant a deja été etat courant alors appartient renvoi vrai, dans ce cas on saute cet état
	            	{
	            		//On ne fait rien
	            	}
	            	else
	            	{
	            		tabCourant.insert ( std::pair<etatset_t, etat_t>(etatCourants,indice) ); // Etant donné que c'est un nouvelle état, on le mets dans le tableau 
	            		
		                // Même chose que au dessus pour l'état initial
		                for(size_t i=0;i<at.nb_symbs;i++)
		                {
		                    str=i+'a';
		                    etatSecondaire = Delta(at,etatCourants,str);
		                    if (etatSecondaire.size() == 0)
		                    {
		                    	poubelle = true;
		                    	if(r.trans.size() <= indice)
		                    	{
		                    		std::vector<etatset_t> steack1;
		                    		r.trans.push_back(steack1);
		                    	}
		                    	if(r.trans[indice].size() <= i)
		                    	{
		                    		etatset_t steack;
		                    		r.trans[indice].push_back(steack);

		                    	}
		                    }
		                    else
		                    {
		                    	for(std::set<etat_t>::iterator it=etatSecondaire.begin();it != etatSecondaire.end();++it)
		                    	{
		                    		etatSecondaire2=(*it);
		                    		if(r.trans.size() <= indice)
		                    		{
		                    			std::vector<etatset_t> steack1;
		                    			r.trans.push_back(steack1);
		                    		}
		                    		if(r.trans[indice].size() <= i)
		                    		{
		                    			etatset_t steack;
		                    			r.trans[indice].push_back(steack);

		                    		}
		                    			r.trans[indice][i].insert(etatSecondaire2);
		                    	}
		                    }	                    
		                }
						indice++;
					}
	            }
	        }	        
            indiceetat++; 		//Incrementer indiceetat siginifie qu'on a terminer avec l'étatCourant précedent
            if(r.trans.size() <= indiceetat) break; //Quand l'indice d'état est aussi grand que la taille du tableau de transition alors on a travaillé sur tout les états possibles
            
            etatCourants=r.trans[indiceetat][0];

        }while (1); 

        //Si on a un état poubelle, on créer un nouvel état a la fin du tableau de transition, chaque transition depuis cette état arrive sur lui même
        if(poubelle)
        {
        	
        	for(size_t j=0;j<at.nb_symbs;j++)
        	{
        		if(r.trans.size() <= indiceetat)
	            {
	                std::vector<etatset_t> steack;
	                r.trans.push_back(steack);
	            }
	            if(r.trans[indiceetat].size() <= j)
	            {
	                etatset_t steack;
	                r.trans[indiceetat].push_back(steack);
				}
        		r.trans[indiceetat][j].insert(99);
        	}
        }

       // parcours de tout le tableau de trans pour y placer de nouveau numero d'états
     	std::map<etatset_t,etat_t>::iterator it;
     	for(size_t i=0;i<r.trans.size();i++)
     	{
     		for(size_t j=0;j<at.nb_symbs;j++)
     		{
     			//Si on a pas d'état d'arriver, on va dans la poubelle
     			if(r.trans[i][j].size() == 0) 
     			{
	     			r.trans[i][j].insert(99);
	     			etatset_t tempo;
	     			tempo.insert(99);
	     			tabCourant.insert ( std::pair<etatset_t, etat_t>(tempo,indiceetat) );
	     		}
	     		// On accede à l'état i j
	     		it=tabCourant.find(r.trans[i][j]);
	     		// On l'ajoute aux états finaux si il contient un état final
	     		if(estFinal(r.trans[i][j],at.finaux))
	     		{
	     			r.finaux.insert(it->second);
	     		}

	     		
	     		
	     		//On supprime l'ancien contenu qui pouvait être plusieur état pour le remplacer par un nouveau numero d'état.
	     		
	     		r.trans[i][j].clear();
	     		r.trans[i][j].insert(it->second);
	     	}
	    }

	     	//On finit de remplir les differents arguments
	     	r.initial=0;
	     	r.nb_symbs=at.nb_symbs;
	     	r.nb_etats=tabCourant.size();
	     	r.nb_finaux=r.finaux.size();



  return r;
}

//******************************************************************************	
