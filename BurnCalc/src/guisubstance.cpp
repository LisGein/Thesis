#include "guisubstance.h"
#include <QHBoxLayout>

#include <QLineEdit>

GuiSubstance::GuiSubstance(QWidget* parent)
   : QWidget(parent)
   , text_edit_(new QLineEdit(this))
{
   setLayout(new QHBoxLayout(this));

   layout()->addWidget(text_edit_);
}

GuiSubstance::~GuiSubstance()
{

}
