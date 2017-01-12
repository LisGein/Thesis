#include "mixture.h"
#include "chemicalSubstance.h"


Mixture::Mixture(std::map<ChemicalSubstance, double> substance)
{
   mixture_substance_ = substance;
}

Mixture::~Mixture()
{

}


ChemicalSubstance Mixture::calc_emperical_formula()
{
   std::map<Element, double> amounts_elements;

   for(auto const &component : mixture_substance_)
   {
      double component_mass = component.second;
      ChemicalSubstance const &subs = component.first;

      double amount_substance = component_mass / subs.get_molecular_mass();

      std::map<Element, double> amount_elements_substance = subs.get_amount_elements(amount_substance);

      for (auto &amount_element: amount_elements_substance)
      {
         double amount = amount_element.second;
         amounts_elements[amount_element.first] += amount;
      }

   }

   ChemicalSubstance substance;

   for (auto &iter: amounts_elements)
      substance.add_element(iter.first,iter.second);


   return substance;
}



