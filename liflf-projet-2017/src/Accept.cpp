//******************************************************************************

#include <iostream>
#include <algorithm>

#include "Determinize.h"
#include "Accept.h"
#include "FA_tools.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////

bool EstDeterministe(const sAutoNDE& at){
  size_t i,j;

  // On parcours le tableau de transition, si à partir d'un etat on a plusieur possibilités en lisant un symbole alors il n'est pas deterministe
  for(i=0;i<at.nb_etats;i++)
  {
  	for(j=0;j<at.nb_symbs;j++)
  	{
  		if (at.trans[i][j].size()!=1)
  		{
  			return false;
  		}
  	}
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////////
// e contient un état ou plus, on y ajoute les états accessibles depuis ces derniers sans lire de caractère.
void Fermeture(const sAutoNDE& at, etatset_t& e){
	etat_t r,etat;
  etatset_t temp;
  // Si il n'y a pas de transition epsilon, il n'y a pas de fermeture a faire.
	if(!at.epsilon.empty())
  	{	
      while(temp.size() != e.size())
      {
        temp=e;
        for(std::set<etat_t>::iterator i = e.begin() ; i != e.end() ; ++i)
        {
         etat=(*i);
         for(std::set<etat_t>::iterator it = at.epsilon[etat].begin() ; it != at.epsilon[etat].end() ; ++it)
          {
           r=(*it);
           e.insert(r); // on insert les etats accessibles par transition epsilon dans e
          }       
      
        }
      }
    }	
}







////////////////////////////////////////////////////////////////////////////////
//e contient un état ou plus, on renvoie tout les états accessible à partir d'un élement de e en lisant le caractere placé dans c
etatset_t Delta(const sAutoNDE& at, const etatset_t& e, symb_t c){
  etatset_t etatPossible;
  size_t numsymb=c-'a';
  etat_t travail;
  etat_t etatSecondaire;
  
  
  if (numsymb>=at.nb_symbs) {return etatPossible;}  //Si le symbol n'est pas de l'alphabet utilisé, on renvoie un état vide
  
  for(std::set<etat_t>::iterator it=e.begin();it != e.end();++it)
  {
  	travail =(*it);
  	for(std::set<etat_t>::iterator it2=at.trans[travail][numsymb].begin();it2 != at.trans[travail][numsymb].end();++it2)
  	{
  		etatSecondaire =(*it2);
  		etatPossible.insert(etatSecondaire);    
  	}
  }
  Fermeture(at,etatPossible); 	//On effectue une fermeture dans le cas ou on arrive sur un état qui posssede des transition epsilon

  return etatPossible;
}


////////////////////////////////////////////////////////////////////////////////
// Determine si l'automate at accepte la chaine de caractere str
bool Accept(const sAutoNDE& at, std::string str){
	
	// 1er cas, si l'automate est deterministe
	if (EstDeterministe(at))
	{
		etat_t etatCourant=at.initial;
		etat_t etatSecondaire;
		while(str.size() != 0) 											//tant qu'il y a un caractère à lire, on effectue des transitions
		{
			size_t numsymb= str[0] -'a';
			etatCourant=(*at.trans[etatCourant][numsymb].begin()); 		//Conversion d'un etatset en etat
			str.erase(str.begin()); 									//on enleve un caractere de la chaine
		}
		//Si après avoir lu tous les caractères on se trouve sur un état final, alors le mot est accepté
		for(std::set<etat_t>::iterator it=at.finaux.begin();it != at.finaux.end();++it)
		{
			etatSecondaire=(*it);
			if (etatCourant==etatSecondaire) {return true;}
		}
		return false;
	}	//2eme cas, si l'automate n'est pas deterministe
    else {
    	sAutoNDE r;
    	etatset_t etatCourants;
    	etat_t etatSecondaire1;
    	etat_t etatSecondaire2;

    	//On commence par inseré l'état initial dans etatcourant qui est ici un étatset et non un état, puis on fait la fermeture
    	etatCourants.insert(at.initial);
    	Fermeture(at,etatCourants);

    	while(str.size() != 0)							//Tant qu'il y a un caractère à lire on appelle Delta avec le premier caractère
    	{
    		etatCourants = Delta(at,etatCourants,str[0]);
    		str.erase(str.begin());
    	}

    	//Si après avoir lu tous les caractères l'un des états de etatCourant est un état final, alors le mot est accepté
		for(std::set<etat_t>::iterator it1=etatCourants.begin();it1 != etatCourants.end();++it1)
		{
			for(std::set<etat_t>::iterator it2=at.finaux.begin();it2 != at.finaux.end();++it2)
			{
				etatSecondaire1=(*it1);
				etatSecondaire2=(*it2);
				if (etatSecondaire1==etatSecondaire2) {return true;}
			}
		}
		return false;
    }
}

//******************************************************************************
