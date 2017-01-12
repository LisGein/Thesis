#include "reaction.h"


Reaction::Reaction(const ChemicalSubstance& fuel, const ChemicalSubstance& oxidant)
   : fuel_(fuel)
   , oxidant_(oxidant)
{

}

std::map<ChemicalSubstance, double> Reaction::get_regrents_mass() const
{
   std::map<ChemicalSubstance, double> regents;

   std::vector<double> mass = get_mass_fractions();
   regents.emplace(fuel_, mass[0]);
   regents.emplace(oxidant_, mass[1]);

   return regents;
}

std::vector<double> Reaction::get_molar_fractions() const
{
   return {1.0f, -1.0f * fuel_.valence() / oxidant_.valence()};
}

std::vector<double> Reaction::get_mass_fractions() const
{
   auto molar = get_molar_fractions();

   molar[0] *= fuel_.get_molecular_mass();
   molar[1] *= oxidant_.get_molecular_mass();

   return molar;
}
