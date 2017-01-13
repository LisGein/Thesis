#include <iostream>

#include "gtest/gtest.h"
#include "../element.h"
#include "../chemicalSubstance.h"
#include "../mixture.h"
#include "../reaction.h"


TEST(molecular_mass_test_case, molecular_mass_test)
{
    auto subs = ChemicalSubstance::from_string("H2");
    ASSERT_NEAR(subs.get_molecular_mass(), 2., 0.1);

    subs.add_element(Element(2), 3);
    ASSERT_NEAR(subs.get_molecular_mass(), 14., 0.1);
}

TEST(emperical_test_case, emperical_test)
{

   auto fuel1 = ChemicalSubstance::from_string("CH4"); //30%
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

   ASSERT_TRUE(formula.compare_part_emperical(ChemicalSubstance::from_string("C13.42925H45.40346O49.56023")));
}
