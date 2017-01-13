#pragma once
#include <QWidget>


class QTableWidget;

class GuiSubstances
      : public QWidget
{
public:
   GuiSubstances(QWidget *parent = 0);
   virtual ~GuiSubstances();

   std::vector<std::map<std::__cxx11::string, double> > get_data();


private:
   QTableWidget *fuel_;
   QTableWidget *oxidant_;


   std::map<std::__cxx11::string, double> get_normalize_data(QTableWidget *w, double k);

   double calc_percent(const std::map<std::string, double> &substances);
};
