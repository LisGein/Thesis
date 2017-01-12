#pragma once
#include "chemicalSubstance.h"
#include <vector>


class Reaction
{
public:
   Reaction(const ChemicalSubstance &fuel, const ChemicalSubstance &oxidant);

   std::map<ChemicalSubstance, double> get_regrents_mass() const ;
   std::vector<double> get_molar_fractions() const ;
   std::vector<double> get_mass_fractions() const ;

private:
   ChemicalSubstance fuel_;
   ChemicalSubstance oxidant_;
};
