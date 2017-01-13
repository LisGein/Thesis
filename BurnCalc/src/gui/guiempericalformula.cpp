#include "guiempericalformula.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QDebug>
#include <QPushButton>
#include <cassert>
#include "guisubstance.h"
#include "../chemicalSubstance.h"
#include "../reaction.h"
#include "../runtimeerror.h"


GuiEmpericalFormula::GuiEmpericalFormula(QWidget* parent)
   : QWidget(parent)
   , substances_(new GuiSubstances(this))
   , calculate_(new QPushButton(tr("Calculate"), this))
   , line_out_(new QLineEdit(this))
{
   setLayout(new QVBoxLayout(this));
   layout()->addWidget(substances_);
   layout()->addWidget(calculate_);
   layout()->addWidget(line_out_);

   QObject::connect(calculate_, SIGNAL(released()), this, SLOT(calculate()));
}

GuiEmpericalFormula::~GuiEmpericalFormula()
{

}

void GuiEmpericalFormula::calculate()
{
   std::vector<std::map<std::string, double>>  v  = substances_->get_data();
   assert(v.size() == 2);

   if (v.size() != 2)
      throw "Trouble with read oxidant and fuel!";

   try
   {
      Mixture fuel_mix = create_mix(v[0]);
      auto fuel = fuel_mix.calc_emperical_formula();
      Mixture ox_mix = create_mix(v[1]);
      auto ox = ox_mix.calc_emperical_formula();

      Reaction r(fuel, ox) ;

      Mixture mix(r.get_regrents_mass());
      auto formula = mix.calc_emperical_formula();

      line_out_->setText(QString::fromStdString(formula.to_string()));
   }
   catch(const RunTimeError &error)
   {
      line_out_->setText(tr("Wrong input substance ") + (QString::fromStdString(error.arg())));
   }

   catch(...)
   {
      line_out_->setText(tr("Wrong input substance"));
   }

}

Mixture GuiEmpericalFormula::create_mix(const std::map<std::string, double> &substances)
{
   std::map<ChemicalSubstance, double> mix;
   for (auto &it: substances)
   {

      auto substance = ChemicalSubstance::from_string(it.first);
      mix.emplace(substance, it.second);

   }

   return Mixture(mix);
}






