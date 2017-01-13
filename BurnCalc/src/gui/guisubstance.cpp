#include "guisubstance.h"
#include <QHBoxLayout>
#include <QTableWidget>


GuiSubstances::GuiSubstances(QWidget* parent)
   : QWidget(parent)
   , fuel_(new QTableWidget(5, 2, this))
   , oxidant_(new QTableWidget(5, 2, this))
{
   setLayout(new QHBoxLayout(this));
   layout()->addWidget(fuel_);
   layout()->addWidget(oxidant_);
   fuel_->setHorizontalHeaderLabels(QString("Substance;Percent").split(";"));
   oxidant_->setHorizontalHeaderLabels(QString("Substance;Percent").split(";"));
}

GuiSubstances::~GuiSubstances()
{

}

std::vector<std::map<std::string, double>> GuiSubstances::get_data()
{
   std::map<std::string, double> fuel = get_normalize_data(fuel_, 1.0);
   double percent = calc_percent(fuel);
   if (qAbs(100.0 - percent) > 0.001)
      fuel = get_normalize_data(fuel_, 100.0 / percent);

   std::map<std::string, double> oxidant = get_normalize_data(oxidant_, 1.0);
   percent = calc_percent(oxidant);
   if (qAbs(100.0 - percent) > 0.001)
      oxidant = get_normalize_data(oxidant_, 100.0 / percent);

   return {fuel, oxidant};

}

std::map<std::string, double> GuiSubstances::get_normalize_data(QTableWidget* w, double k)
{
   std::map<std::string, double> substances;


   for (auto i = 0; i < 5; ++i)
   {
      QTableWidgetItem *item_sub = w->item(i, 0);

      if (item_sub)
      {
         QTableWidgetItem *item_percent = w->item(i, 1);
         double p = 0.0;
         if (item_percent)
         {
            p = item_percent->data(0).toDouble() * k;
            item_percent->setData(0, QVariant(p));
         }
         std::string  name = item_sub->data(0).toString().toStdString();
         substances.emplace(name, p);
      }
   }

   return substances;

}

double GuiSubstances::calc_percent(const std::map<std::__cxx11::string, double>& substances)
{
   double percent = 0.0;
   for (auto &it:substances)
      percent += it.second;

   return percent;
}



