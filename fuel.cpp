#include "fuel.h"
#include<QVBoxLayout>
#include<QLabel>
#include<QLineEdit>
#include<QFrame>
#include<QGroupBox>
#include<QSpinBox>


Fuel::Fuel(QWidget *parent)
   : QWidget(parent)
   , add_oxidizer_(new  QPushButton(this))
   ,  remove_oxidizer_(new  QPushButton(this))
   , add_fuel_(new  QPushButton(this))
   ,  remove_fuel_(new  QPushButton(this))
   , layout_(new QHBoxLayout(this))
{
   add_oxidizer_->setText("+");
   remove_oxidizer_->setText("-");
   add_fuel_->setText("+");
   remove_fuel_->setText("-");
   create_frame_oxidizer();
   create_frame_fuel();
   setLayout(layout_);
}

void Fuel::create_frame_oxidizer()
{
   QHBoxLayout* layout = new QHBoxLayout(this);
   QGroupBox *group = new QGroupBox(this);
   group->setTitle("Oxidizer");
   group->setLayout(layout);
   layout_->addWidget(group);

   QVBoxLayout* v_f_layout = new QVBoxLayout(this);
   QHBoxLayout* h_layout = new QHBoxLayout(this);
   QLabel* chemical = new QLabel("Chemical formula");
   h_layout->addWidget(chemical);
   h_layout->addWidget(add_oxidizer_);
   h_layout->addWidget(remove_oxidizer_);

   v_f_layout->addLayout(h_layout);
   f_chemical_oxidizer_ = new QLineEdit(this);
   v_f_layout->addWidget(f_chemical_oxidizer_);
   s_chemical_oxidizer_ = new QLineEdit(this);
   v_f_layout->addWidget(s_chemical_oxidizer_);

   layout->addLayout(v_f_layout);


   QVBoxLayout* v_s_layout = new QVBoxLayout(this);
   QLabel* prt = new QLabel("%");
   v_s_layout->addWidget(prt);
   QSpinBox* fs_line = new QSpinBox(this);
   v_s_layout->addWidget(fs_line);
   QSpinBox* ss_line = new QSpinBox(this);
   v_s_layout->addWidget(ss_line);

   layout->addLayout(v_s_layout);



   QVBoxLayout* v_t_layout = new QVBoxLayout(this);
   QLabel* e = new QLabel("Enthalpy");
   v_t_layout->addWidget(e);
   QLineEdit* ft_line = new QLineEdit(this);
   v_t_layout->addWidget(ft_line);
   QLineEdit* st_line = new QLineEdit(this);
   v_t_layout->addWidget(st_line);

   layout->addLayout(v_t_layout);

   layout_->addLayout(layout);

}

void Fuel::create_frame_fuel()
{
   QHBoxLayout* layout = new QHBoxLayout(this);
   QGroupBox *group = new QGroupBox(this);
   group->setTitle("Oxidizer");
   group->setLayout(layout);
   layout_->addWidget(group);

   QVBoxLayout* v_f_layout = new QVBoxLayout(this);
   QHBoxLayout* h_layout = new QHBoxLayout(this);
   QLabel* chemical = new QLabel("Chemical formula");
   h_layout->addWidget(chemical);
   h_layout->addWidget(add_fuel_);
   h_layout->addWidget(remove_fuel_);

   v_f_layout->addLayout(h_layout);
   f_chemical_fuel_ = new QLineEdit(this);
   v_f_layout->addWidget(f_chemical_fuel_);
   s_chemical_fuel_ = new QLineEdit(this);
   v_f_layout->addWidget(s_chemical_fuel_);

   layout->addLayout(v_f_layout);


   QVBoxLayout* v_s_layout = new QVBoxLayout(this);
   QLabel* prt = new QLabel("%");
   v_s_layout->addWidget(prt);
   QSpinBox* fs_line = new QSpinBox(this);
   v_s_layout->addWidget(fs_line);
   QSpinBox* ss_line = new QSpinBox(this);
   v_s_layout->addWidget(ss_line);

   layout->addLayout(v_s_layout);



   QVBoxLayout* v_t_layout = new QVBoxLayout(this);
   QLabel* e = new QLabel("Enthalpy");
   v_t_layout->addWidget(e);
   QLineEdit* ft_line = new QLineEdit(this);
   v_t_layout->addWidget(ft_line);
   QLineEdit* st_line = new QLineEdit(this);
   v_t_layout->addWidget(st_line);

   layout->addLayout(v_t_layout);

   layout_->addLayout(layout);
}

