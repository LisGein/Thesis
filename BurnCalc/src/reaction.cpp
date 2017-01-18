#include "reaction.h"
#include <cassert>

Reaction::Reaction(const ChemicalSubstance& fuel, const ChemicalSubstance& oxidant, double factor_excess_oxidant)
   : fuel_(fuel)
   , oxidant_(oxidant)
   , factor_excess_oxidant_(factor_excess_oxidant)
{

}

std::map<ChemicalSubstance, double> Reaction::get_regrents_mass() const
{
   auto molar = get_molar_fractions();
   assert(molar.size() == 2);

   std::map<ChemicalSubstance, double> regents;
   regents.emplace(fuel_, molar[0] * fuel_.get_molecular_mass());
   regents.emplace(oxidant_, molar[1] * oxidant_.get_molecular_mass() *factor_excess_oxidant_);

   return regents;
}

std::vector<double> Reaction::get_molar_fractions() const
{
   return {1.0f, -1.0f * fuel_.valence() / oxidant_.valence()};
}
