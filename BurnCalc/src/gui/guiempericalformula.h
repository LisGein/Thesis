#pragma once
#include <QWidget>
#include <QTableWidget>
#include "../mixture.h"

class GuiSubstances;
class QPushButton;
class QDoubleSpinBox;

class GuiEmpericalFormula
      : public QWidget
{
   Q_OBJECT
public:
   GuiEmpericalFormula(QWidget *parent = 0);
   virtual ~GuiEmpericalFormula();

private slots:
   void calculate();

private:
   GuiSubstances *substances_;
   QPushButton *calculate_;
   QLineEdit *line_out_;
   QLineEdit *line_enthalpy_;
   QDoubleSpinBox *factor_excess_oxidant_;

   Mixture create_mix(const std::map<std::__cxx11::string, double>& substances);

};
