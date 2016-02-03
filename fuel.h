#pragma once
#include <QWidget>
#include <QPushButton>

class QHBoxLayout;
class QLineEdit;
class Fuel : public QWidget
{
   Q_OBJECT
public:
   Fuel(QWidget* parent = 0);

private:
   QPushButton* add_oxidizer_;
   QPushButton* remove_oxidizer_;
   QLineEdit* f_chemical_oxidizer_;
   QLineEdit* s_chemical_oxidizer_;

   QPushButton* add_fuel_;
   QPushButton* remove_fuel_;
   QLineEdit* f_chemical_fuel_;
   QLineEdit* s_chemical_fuel_;

   QHBoxLayout* layout_;

   void create_frame_oxidizer();
   void create_frame_fuel();

};

