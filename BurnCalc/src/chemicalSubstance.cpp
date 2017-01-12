#include "chemicalSubstance.h"
#include <numeric>
#include <locale>



ChemicalSubstance::ChemicalSubstance()
{

}

ChemicalSubstance::~ChemicalSubstance()
{

}

void ChemicalSubstance::add_element(const Element& element, LowerIndex index)
{
   auto iter = substance_.find(element);
   if (iter != substance_.end())
      iter->second += index;

   else
      substance_.emplace(element, index);
}

double ChemicalSubstance::get_molecular_mass() const
{
   double mass = 0.0;

   for (auto &v : substance_)
      mass += v.first.get_mass() * v.second;

   return mass;
}/*

double ChemicalSubstance::calc_amount(double molecular_mass)
{
   return
         mass_substance_/molecular_mass;
}*/

std::map<Element, double> ChemicalSubstance::get_amount_elements(double amount_substance) const
{
   std::map<Element, double> amount_elements;
   for (auto &v : substance_)
      amount_elements.emplace(v.first, amount_substance * v.second);

   return amount_elements;
}

std::string ChemicalSubstance::to_string() const
{
   std::string str;
   for(auto &iter: substance_)
      str += iter.first.to_string();

   return str;
}

void ChemicalSubstance::insert_element(const std::string& str, const std::string& index)
{
   double digit = (index.empty()) ? 1.0 : std::stod(index);
   Element element = find_from_str(str);
   add_element(element, digit);
}

ChemicalSubstance ChemicalSubstance::from_string(const std::__cxx11::string& str)
{
   ChemicalSubstance s;
   if (!Element::periodic_table_)
      Element::load_periodic_table();


   std::string current_str;
   std::string current_digit;

   std::locale loc;


   for (auto i = str.begin(); i != str.end(); ++i)
   {
      if (std::isalpha(*i, loc))
      {
         if (!current_digit.empty()|| (!std::islower(*i, loc) && !current_str.empty()))
         {
            s.insert_element(current_str, current_digit);
            current_str.clear();
            current_digit.clear();

         }

         current_str += (*i);

      }
      else
         current_digit += (*i);

   }
   if (!current_str.empty())
      s.insert_element(current_str, current_digit);

   return s;

}

int ChemicalSubstance::valence() const
{
   int v = 0;

   for (auto &it : substance_)
      v += it.first.get_valence() * it.second;

   return v;
}

bool ChemicalSubstance::operator<(const ChemicalSubstance& other) const
{
   auto it_other = other.substance_.begin();
   for (auto &it : substance_)
   {
      if (it_other == other.substance_.begin())
         return true;

      if (it.first != it_other->first)
         return it.first < it_other->first;
      ++it_other;

   }
   return false;

}

Element ChemicalSubstance::find_from_str(const std::__cxx11::string& str)
{
   Element elem;
   auto it = Element::periodic_table_from_name_->find(str);
   if (it != Element::periodic_table_from_name_->end())
      elem = Element(it->second);

   return elem;

}

