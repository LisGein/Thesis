#include <iostream>

#include "gtest/gtest.h"
#include "../element.h"
#include "../chemicalSubstance.h"
#include "../mixture.h"
#include "../reaction.h"

bool compare_emperical(const ChemicalSubstance &first, const ChemicalSubstance &second)
{
   std::map<Element, LowerIndex> f_substance = first.get_substance();
   std::map<Element, LowerIndex> s_substance = second.get_substance();

   bool result = true;

   double min = 1;
   double max = 1;
   if (f_substance.size() != s_substance.size())
      result = false;
   else
   {
      auto it_s = s_substance.begin();
      for(auto &it_f: f_substance)
      {
         if (it_f.first != it_s->first)
            result = false;
         else
         {
            double k = it_f.second/it_s->second;

            if (it_s == s_substance.begin())
            {
               min = k;
               max = k;
            }


            min = std::min(min, k);
            max = std::min(max, k);


         }

         ++it_s;
      }
   }

   if (max - min > 0.1)
      result = false;

   return result;
}

TEST(molecular_mass_test_case, molecular_mass_test)
{
    //ChemicalSubstance subs(300, true);
    //subs.add_element(Element(1), 2);
    //ASSERT_NEAR(subs.calc_molecular_mass(), 2., 0.1);

    //subs.add_element(Element(2), 3);
    //ASSERT_NEAR(subs.calc_molecular_mass(), 14., 0.1);


   ChemicalSubstance fuel1 = ChemicalSubstance::from_string("CH4"); //30%
   auto fuel2 = ChemicalSubstance::from_string("C2H5OH"); //70%
   auto ox = ChemicalSubstance::from_string("O2");//100%

   std::map<ChemicalSubstance, double> f_mix;

   f_mix.emplace(fuel1, 300.0);
   f_mix.emplace(fuel2, 700.0);

   Mixture fuel_mix(f_mix);
   auto fuel = fuel_mix.calc_emperical_formula();

   Reaction r(fuel, ox) ;

   Mixture mix(r.get_regrents_mass());
   auto formula = mix.calc_emperical_formula();

   ASSERT_TRUE(compare_emperical(formula, ChemicalSubstance::from_string("C13.42925H45.40346O49.56023")));
}
