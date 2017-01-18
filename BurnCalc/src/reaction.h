#pragma once
#include "chemicalSubstance.h"
#include <vector>


class Reaction
{
public:
   Reaction(const ChemicalSubstance &fuel, const ChemicalSubstance &oxidant, double factor_excess_oxidant = 1.0);

   std::map<ChemicalSubstance, double> get_regrents_mass() const ;

private:
   std::vector<double> get_molar_fractions() const ;

   ChemicalSubstance fuel_;
   ChemicalSubstance oxidant_;
   double factor_excess_oxidant_;
};
