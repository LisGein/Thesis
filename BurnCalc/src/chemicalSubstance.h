#pragma once
#include <map>
#include <string>
#include "element.h"


typedef double LowerIndex;

class ChemicalSubstance
{
public:
   ChemicalSubstance();
   ~ChemicalSubstance();

   void insert_element(const std::__cxx11::string& str, const std::__cxx11::string& index);
   static ChemicalSubstance from_string(const std::string &str);


   void add_element(const Element &element, LowerIndex index);

   double get_molecular_mass() const;
   std::map<Element, double> get_amount_elements(double amount_substance) const;

   std::string to_string() const;

   int valence() const;

   bool operator<(const ChemicalSubstance &other) const;

   std::map<Element, LowerIndex> get_substance() const
   {
      return substance_;
   }

protected:
   std::map<Element, LowerIndex> substance_;

private:
   Element find_from_str(const std::string &str);
};
