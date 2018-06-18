//******************************************************************************

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <string>

#include "Minimize.h"
#include "Accept.h"
#include "Determinize.h"

using namespace std;
////////////////////////////////////////////////////////////////////////////////

//V.2.Fonction de test pour savoir si un état est final, renvoi vrai si l'état passé en argument est final (contenu dans finaux)
bool Final(size_t etatCourants,etatset_t finaux)
{
	etat_t secondaire2;
	for(std::set<etat_t>::iterator it2=finaux.begin();it2 != finaux.end();++it2)
	{
			
		secondaire2=(*it2);
		if (secondaire2 == etatCourants)
			return true;
	}

	return false;
}



sAutoNDE Minimize(const sAutoNDE& at){
	sAutoNDE r;

// Premiere étape, on s'assure de travailler avec un automate deterministe
  if(!EstDeterministe(at))
  {
  	r = Determinize(at);
  }else
  {
  	r = at;	
  }

  std::vector<std::map<etat_t,string>> tab;			//tab va contenir le numero de l'état et une chaine de caractère qui represente les groupes accessibles en lisant un caractères

  std::map<etat_t,char> tableaugr;					//tableaugr va contenir chaque état et le groupe auquel il appartient

  tab.resize(2);			


// On fait deux groupes: les états finaux dans tab[1], les non finaux dans tab[0] // chaque numero de tab est associé à un groupe
  for (size_t i=0; i<r.nb_etats; i++)
  {
  	if(Final(i,r.finaux))
  	{
  		tab[1].insert( std::pair<etat_t,string>(i,""));					
  		tableaugr.insert( std::pair<etat_t,char>(i,'b'));				//les états finaux font partie du groupe b
  	}else
  	{
  		tab[0].insert( std::pair<etat_t,string>(i,""));
  		tableaugr.insert( std::pair<etat_t,char>(i,'a'));				//les états non finaux font partie du groupe a
  	}

  }



  ////////////////////////////////////////////////////////////////////////////
std::map<etat_t,string>::iterator it;
std::map<etat_t,string>::iterator it2;
string mot;
int indice = 0;				//L'indice va nous indiquer sur quel groupe on travail
bool change= false;
size_t etat;

while(tab.size() != indice)
{
	if(tab[indice].size() != 1)					//Dans le cas ou une groupe ne contient qu'un seul état, on ne peu pas le reduire
	{

		for(it=tab[indice].begin(); it != tab[indice].end(); it++)			//Pour chaque état du groupe, on inscrit le groupe d'arriver lorsqu'on lit chacun des caractères
		{																	//Pour un automate qui possede trois symbole, on obtiendra par exemple AAB, ou BAB ou AAA ...
			for(size_t j=0; j<r.nb_symbs; j++)
			{
				etat = *r.trans[it->first][j].begin();
				mot=mot + tableaugr.find(etat)->second;
			}
			tab[indice].at(it->first) = mot;
			mot="";
		}



		int compteur = tab.size()+1;

		for(it=tab[indice].begin(); it != tab[indice].end(); it++)				//Lorsqu'on traite un groupe, on commence par mettre tout les états de ce groupe dans un nouveau (donc avec une lettre plus proche de z)
		{
			tableaugr.at(it->first) = 'a' + tab.size();

		}


		for(it=tab[indice].begin(); it != tab[indice].end(); it++)				//On fait la difference entre les états en comparant la chaine de caractère rentré précedement
		{
			for(it2=it; it2 != tab[indice].end(); it2++)
			{

				if(it->second != it2->second)									
				{
					change = true;												//lorsque l'on compare les états, on notifit une difference avec change
					
					if(tableaugr.find(it->first)->second == tableaugr.find(it2->first)->second) //On ne modifie le groupe que des états qui sont encore dans le même groupe, si le groupe de it2 est plus grand, c'est que it1 est similaire à un état deja rencontré,
					{																			// si it2 est plus petit, c'est que it2 est similaire à un état deja rencontré
					
						tableaugr.at(it2->first) = 'a' + compteur;
					}

				}else
				{
					tableaugr.at(it2->first) = tableaugr.find(it->first)->second;
				}
			}
			compteur ++;

		}

		//parcour de tableau gr
		//On mets en place les nouveaux groupes dans tab, par exemple, le groupe C est rangé dans tab[2]...
		if(change)
		{
			int num;
			for (auto& x:tableaugr)
			{
				num = x.second - 'a';
				if(tab.size() <= num+1)
					tab.resize(num+1);

				tab[num].insert( std::pair<etat_t,string>(x.first,""));
			}
		}

		
	}
		indice ++;
}





int nbetat;
std::set<char> tabchar;					
trans_t transtempo;						//tableau de transition temporaire
char petit='z';
int numetat = 0;


bool sortie = true;

for(char i='a'; i<tableaugr.size()+'a'; i++)
{
	do
	{

		//On parcours le tableau associant les états et leur groupe, si un groupe n'a aucun état, on réduit le groupe de tout les états (afin d'avoir des groupes qui se suivent a la fin)

		sortie = true;
		for (auto& x:tableaugr)   
		{
			if(x.second == i)
			{
				sortie = false;
			}
		}

		if(sortie)
		{
			for (auto& x:tableaugr)
			{
				if(x.second > i)
				{
					tableaugr.at(x.first)=x.second-1;
				}
			}
		}
		
	}while(sortie);
}


//On remplis un tableau de transition, on créer un état pour chaque groupe restant
for (auto& x:tableaugr)
{
	if(tabchar.count(x.second) == 0)
	{
		transtempo.resize(transtempo.size()+1);
		tabchar.insert(x.second);
		transtempo[numetat].resize(r.nb_symbs);
		for(size_t j=0; j<r.nb_symbs; j++)
		{
			
			etat = *r.trans[x.first][j].begin();
			transtempo[numetat][j].insert(tableaugr.find(etat)->second - 'a');
		}
		numetat++;
	}



}

r.initial= tableaugr.find(r.initial)->second - 'a';


etatset_t prochainfinal;
std::set<etat_t>::iterator it1;
for(it1 = r.finaux.begin() ; it1 != r.finaux.end(); it1++)
{

	prochainfinal.insert(tableaugr.find(*it1)->second - 'a');
}

r.nb_finaux = prochainfinal.size();
r.finaux = prochainfinal;
r.trans=transtempo;
r.nb_etats=transtempo.size();

  return r;
}

//******************************************************************************
