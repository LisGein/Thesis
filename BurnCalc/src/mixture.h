#pragma once
#include <vector>

#include "chemicalSubstance.h"

class Mixture
{
public:
   Mixture(std::map<ChemicalSubstance, double> substance);
   ~Mixture();



   ChemicalSubstance calc_emperical_formula();



private:
   std::map<ChemicalSubstance, double> mixture_substance_;

};
