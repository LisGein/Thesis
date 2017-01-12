#pragma once
#include <QWidget>


class QLineEdit;

class GuiSubstance
      : public QWidget
{
   Q_OBJECT
public:
   GuiSubstance(QWidget *parent = 0);
   virtual ~GuiSubstance();

private:
   QLineEdit *text_edit_;
};
