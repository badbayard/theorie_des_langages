//******************************************************************************

#include "ToGraph.h"
#include <fstream>
#include <iostream>
 
using namespace std;

////////////////////////////////////////////////////////////////////////////////
//Création et remplissage d'un fichier au format .gv afin d'être utilisable par Graphviz
bool ToGraph(sAutoNDE& at, std::string path){
	etat_t etatSecondaire1;
	etat_t etatSecondaire2;
	char lettre;
  	ofstream fichier (path.c_str());
    if(fichier.is_open())				//On test si le fichier a bien été ouvert
    {
    	//Ligne de base qui definisse le le graph : digraph signifie qu'un noeud peu avoir plusieur fleche (couche)
    	// rankdir defini le sens du dessin(ici left to right), size est la taille max du dessin en inches
    	fichier<<"digraph finite_state_machine {" <<endl;
    	fichier<<"	rankdir=LR;"<<endl;
    	fichier<<"	size=\"10,10\""<<endl<<endl;

    	fichier<<"	node [shape = doublecircle]; ";									//répertorie les noeuds avec un double cercle (états finaux)
    	for(std::set<etat_t>::iterator it2=at.finaux.begin();it2 != at.finaux.end();++it2)
			{
				etatSecondaire1=(*it2);
				fichier<<etatSecondaire1<<" ";
			}
		fichier<<";"<<endl;

		fichier<<"	node [shape = point ]; q;"<<endl;								//L'état q montre le point initial
		fichier<<"	node [shape = circle];"<<endl<<endl;							//Rond pour chaque noeud du graph
		

		fichier<<"	q -> "<<at.initial<<";"<<endl;									//On commence par indiquer l'état initial

																					//On note chaque transition de l'automate
		for (size_t i=0;i<at.nb_etats;i++)
			for(size_t j=0;j<at.nb_symbs;j++)
				for(std::set<etat_t>::iterator it=at.trans[i][j].begin(); it!=at.trans[i][j].end(); ++it)
				{
					etatSecondaire2=(*it);
					lettre=j+'a';

					fichier<<"	"<< i <<" -> "<< etatSecondaire2 <<" [label = \""<< lettre <<"\"];"<<endl;
				}

																					//On note à la suite toute les transitions epsilons
		fichier<<endl;
		if(!at.epsilon.empty())
		for (size_t k=0;k<at.nb_etats;k++)
			if(!at.epsilon[k].empty())
			{
				for(std::set<etat_t>::iterator it3=at.epsilon[k].begin(); it3!=at.epsilon[k].end(); ++it3)
				{
					etatSecondaire2=(*it3);
					fichier<<"	"<< k <<" -> "<< etatSecondaire2 <<" [label = \"ε\"];"<<endl;
				}
			}

		fichier<<endl;
		fichier<<"}";
		return true;

    }else 
    cout<<"erreur lors de l'ouverture"<<endl;

  return false;
}

//******************************************************************************
