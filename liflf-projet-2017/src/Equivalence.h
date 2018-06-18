//******************************************************************************
#ifndef EQUIVALENCE_H
#define EQUIVALENCE_H
//******************************************************************************

#include "FA_types.h"

////////////////////////////////////////////////////////////////////////////////

std::string Automate2ExpressionRationnelle(sAutoNDE at);

////////////////////////////////////////////////////////////////////////////////

// détermine la pseudo équivalence par comparaison de tous les mots de Sigma* de longueur < à word_size_max
bool PseudoEquivalent(const sAutoNDE& a1, const sAutoNDE& a2, unsigned int word_size_max);

////////////////////////////////////////////////////////////////////////////////

// détermine l'équivalence par "égalité" des automates :
//   - même nombre d'états
//   - même état initial
//   - mêmes états finaux
//   - même table de transition
// à un renommage des états près
bool Equivalent(const sAutoNDE& a1, const sAutoNDE& a2);

//******************************************************************************
#endif // EQUIVALENCE_H
//******************************************************************************
