//******************************************************************************

#include <iostream>
#include <cassert>
#include "FA_tools.h"

#include "Equivalence.h"
#include "Accept.h"
#include "Minimize.h"
#include "Determinize.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////

std::string Automate2ExpressionRationnelle(sAutoNDE at)
{
	std::vector<etatset_t>::iterator it;
	std::vector<etatset_t> etatinitial;
	etatset_t etatTampon;
	etatset_t epsi;
	etat_t secondaire;
	etat_t secondaire2;

	//Creation d'un nouvel etat initial
	etatinitial.resize(at.nb_symbs);

	//On mets le nouvel état initial au debut
	at.trans.insert(at.trans.begin(),etatinitial);

	//Decallage de toutes les transitions pour chaque état

	for(size_t i = 1 ; i < at.trans.size(); i++) // on parcours les états sauf le premier
	{
		for(size_t j=0 ; j<at.nb_symbs ; j++)
		{
			// Etant donné qu'on peut travailler avec des automates non determinés, on peu avoir plusieurs états dans une case de transition
			for(std::set<etat_t>::iterator it =at.trans[i][j].begin() ; it != at.trans[i][j].end(); ++it)
			{
				secondaire = (*it);
				etatTampon.insert(secondaire+1); // On incremente de 1 ici
			}	
			at.trans[i][j]=etatTampon; 
			etatTampon.clear();				
		}
	}


	
	at.trans.push_back(etatinitial);					//Insertion d'un nouvel état final
	at.epsilon.insert(at.epsilon.begin(),epsi);			// On insere un etatset vide dans epsilon afin de décaler le reste du tableau
	at.epsilon[0].insert(at.initial+1);					// On mets en place la transition epsilon entre l'état 0 et l'état initial
	at.epsilon.push_back(epsi);							//On complete avec un etat vide à la fin de at.epsilon

	

	//On décale tout le tableau de epsilon transition
	for(size_t i = 1 ; i < at.epsilon.size(); i++)
	{
			for(std::set<etat_t>::iterator it =at.epsilon[i].begin() ; it != at.epsilon[i].end(); ++it)
			{
				secondaire2 = (*it);
				etatTampon.insert(secondaire2+1);
			}	
			at.epsilon[i]=etatTampon;
			etatTampon.clear();	
	}

	// On rajoute une transition epsilon depuis chaque état final de at vers le nouvel état final
	for (std::set<etat_t>::iterator it3 =at.finaux.begin(); it3 != at.finaux.end();++it3)
	{
		secondaire2=*it3;
		at.epsilon[secondaire2+1].insert(indice);
	}

	at.nb_etats += 2;


  std::string sr;

  std::vector<std::vector<std::vector<string>>> tab; //Vector 3D qui va contenir les R(ijk)

  //Remplissage de tab avec des espaces
  tab.resize(at.nb_etats);
  for(size_t i= 0; i<at.nb_etats; i++ )
  {
  	tab[i].resize(at.nb_etats);
  	for(size_t j= 0; j<at.nb_etats; j++ )
	  {
	  	tab[i][j].resize(at.nb_etats);
	  	for(size_t k=0; k<at.nb_etats; k++)
	  	{
	  		tab[i][j][k] = " ";
	  	}  	
	  }
  }



  for(size_t i=0;i<at.trans.size();i++)
     	{
     		//Remplissage des R(ij0) avec les transitions epsilons
     		if(!at.epsilon[i].empty())
 			{
 				for(std::set<etat_t>::iterator it=at.epsilon[i].begin();it != at.epsilon[i].end();++it)
 				{
 					secondaire = *it;
 					tab[i][secondaire][0] = "e";
 				}

 			}

     		for(size_t j=0;j<at.nb_symbs;j++)
     		{
     			
     			// Remplissage des R(ij0) avec les étiquettes sur les fleches.
     			for(std::set<etat_t>::iterator it=at.trans[i][j].begin();it != at.trans[i][j].end();++it)
     			{

     				secondaire = *it;
     				sr = j+'a';

     				if(tab[i][secondaire][0] == " ")
	     			{
	     				tab[i][secondaire][0] = sr;
	     				
	     			}else
	     			{
	     				tab[i][secondaire][0] = tab[i][secondaire][0] + " | " + sr;
	     				if (i == 3 && secondaire == 1)
	     			}


     			}
     		}
     	}


std::string str,a,b,c,d;
bool change = false;			// Booleen de notification des changements


	//Remplissage du tableau par suppression des états successifs
  for(size_t k= 0; k<at.nb_etats; k++ )
  {
  	for(size_t i= 0; i<at.nb_etats; i++ )
	  {
	  	for(size_t j= 0; j<at.nb_etats; j++ )
	  	{

	  		//Chaque conditon permet d'identifier dans quel cas nous somme,
	  		if(k != 0)
	  		{

		  		if( j !=k && i !=k)
		  		{
			  		a=tab[i][j][k-1];
			  		b=tab[i][k][k-1];
			  		c=tab[k][k][k-1];
			  		d=tab[k][j][k-1];

			  		if (b != " " && d != " ")
			  		{
			  			if(a != " " )
				  		{
				  			if(b != "e" || c !="e" || d!= "e")
				  			{
				  				str = a+'|';
				  				change = true;
				  			}else
				  			{
				  				str = a;
				  				change = true;
				  			}
				  			
				  		}

				  		if(b != "e")
				  		{
				  			if(change)
				  			{
				  				str = str + b;
				  			}else
				  			{
				  				str = b;
				  				change = true;
				  			}
				  			  			
				  		}

				  		if(c != " " && c!= "e")
				  		{
				  			if(change)
				  			{
				  				if(b != "e")
				  				{
				  					str = str +"."+ c + "*";
				  				}else
				  				{
				  					str= str + c + "*";
				  				}
				  				
				  			}else
				  			{
				  				str = c + "*" ;
				  				change = true;
				  			}			  			  			
				  		}

				  		if(d != "e")
				  		{
				  			if(change)
				  			{
				  				if(b != "e" || (c != "e" && c != " "))
				  				{
				  					str = str +"."+ d;
				  				}else
				  				{
				  					str = str + d;
				  				}
				  				
				  			}else{

				  				str = d;
				  				change = true;
				  			}
				  			  			
				  		}
				  		if(change)
				  		{
				  			if(str.length() <= 1)
				  			{
				  				tab[i][j][k]= str;
				  			}else
				  			{
				  				tab[i][j][k]='(' + str + ')'; 
				  			}
				  			
				  		}else
				  		{
				  			tab[i][j][k]="e" ;
				  		}
				  		

			  		}else
			  		{
			  			if (a != " ")
				  		{
				  			if(a.length() <=1)
				  			{
				  				tab[i][j][k]=a;
				  			}else
				  			{
				  				tab[i][j][k]='(' + a + ')';
				  			}
				  			

				  		}
			  		}
			  		str = "e";
			  		change = false;
			  		
		  		}
		  	}else
		  	{
		  		//Dans le cas ou l'expression rationelle sur une fleche étiquetée est composé de plusieurs symboles, on la mets entre paranthèse
		  		if(tab[i][j][k].length() > 1)
		  		{
		  			tab[i][j][k] = '(' + tab[i][j][k] + ')';
		  		}
		  	}
	    }
    }
}

//On place dans sr l'expression rationnelle qui nous interresse vraiment
sr = tab[0][at.nb_etats-1][at.nb_etats-2];

  return sr;
}

////////////////////////////////////////////////////////////////////////////////

// Fonction de pseudo-équivalence récursive, test pour un mot puis fait un appel sur chaque nouveau mot possible à partir de celui donné en argument
bool EstPseudoEqui(const sAutoNDE& a1, const sAutoNDE& a2, unsigned int word_size_max, string mot)
{
	//Cas d'arret, le mot a atteint la limite de taille
	if (mot.size() == word_size_max)
	{
		return true;
	}

	string motest;
	char cara;
	// retourne faux si le mot n'est pas accepté de la même façon par les deux automates
	if(mot.size() > 0)
	{
		if (Accept(a1,mot) != Accept(a2,mot))
		{
			return false;
		}
	}
	
	
	//Nouveau test pour chaque mot possible
	for (size_t i=0; i<a1.nb_symbs; i++)
	{
		cara = 'a'+i;
		motest = mot + cara;
		if (!EstPseudoEqui(a1,a2,word_size_max,motest))
			{
				return false;
			}
	}

	return true;
}

bool PseudoEquivalent(const sAutoNDE& a1, const sAutoNDE& a2, unsigned int word_size_max) {

	string mot; 	//Chaine de caractère qui va contenir le mot testé.
	bool un = false;
	bool deux = false;
	

	//Il n'est pas nécessaire de tester l'équivalence de deux automates qui n'ont pas le même nombre de symbole
	if(a1.nb_symbs != a2.nb_symbs)
	{
		return false;
	}

	// Choix d'un fonctionnement par récurrence
	//On test avant si le mot vide est accepté de la même facon par les deux automates
	etat_t second = *a1.finaux.find(a1.initial);
	if (second != a1.finaux.end())
	{
		un = true;
	}
	etat_t trois = *a2.finaux.find(a2.initial);
	if (trois != a2.finaux.end())
	{
		deux = true;
	}

	if(un == deux)
	{
		return EstPseudoEqui(a1,a2,word_size_max,mot);
	}
	return false;
	

}

////////////////////////////////////////////////////////////////////////////////
// Fonction de test d'équivalence, on minimize puis on test le tableau de transiton
bool Equivalent(const sAutoNDE& a1, const sAutoNDE& a2) {

	//Il n'est pas nécessaire de tester l'équivalence de deux automates qui n'ont pas le même nombre de symbole
	if(a1.nb_symbs != a2.nb_symbs)
	{
		return false;
	}


	sAutoNDE automate1;
	sAutoNDE automate2;

	automate1 = Minimize(a1);
	automate2 = Minimize(a2);

	//Si une fois minimizé les deux automates n'ont pas le même nombre d'état, ils ne sont pas équivalent
	if(automate1.nb_etats != automate2.nb_etats)
	{
		return false;
	}

	// Deux états n'ont pas forcement le meme numero lorsqu'on les compare
	std::map<etat_t,etat_t> coresp;		//association de deux etats suivant leur correspondance
	etatset_t stock;					//stock va contenir les états dans lesquels on n'est pas encore allé, un etat est ajouté si il n'est pas encore dans la liste de correspondance
	etat_t courant1;
	etat_t courant2;
	courant1 = automate1.initial;
	courant2 = automate2.initial;
	etat_t secondaire1;
	etat_t secondaire2;

	coresp.insert(std::pair<etat_t,etat_t>(courant1,courant2));   //On insert une correspondance d'état de cette manière
	
	//cas particulier du cas initial
	for(size_t i=0; i<automate1.nb_symbs; i++)
		{
			secondaire1 = *automate1.trans[courant1][i].begin();
			secondaire2 = *automate2.trans[courant2][i].begin();
			if(coresp.find(secondaire1) != coresp.end())
			{
				if(coresp.find(secondaire1)->second != secondaire2)
				{
					return false;
				}
			}else
			{
				coresp.insert(std::pair<etat_t,etat_t>(secondaire1,secondaire2));
				stock.insert(secondaire1);

			}
		}

// Boucle tant qu'on a des état à traiter;
while (stock.size() != 0)
{

	courant1 = *stock.begin();
	courant2 = coresp.find(courant1)->second;
	stock.erase(courant1);

		for(size_t i=0; i<automate1.nb_symbs; i++)
		{
			secondaire1 = *automate1.trans[courant1][i].begin();
			secondaire2 = *automate2.trans[courant2][i].begin();
			if(coresp.find(secondaire1) != coresp.end())
			{
				if(coresp.find(secondaire1)->second != secondaire2)
				{
					return false;
				}
			}else
			{
				coresp.insert(std::pair<etat_t,etat_t>(secondaire1,secondaire2));
				stock.insert(secondaire1);

			}
		}
}
	
	




  return true;
}

//******************************************************************************
