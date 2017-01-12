#include "element.h"
#include "yaml-cpp/yaml.h"

std::unique_ptr<std::map<int, Element::ElementInfo>> Element::periodic_table_;
std::unique_ptr<std::map<std::string, int>> Element::periodic_table_from_name_;

Element::Element(int id)
   : id_(id)
{
   if (!periodic_table_)
   {
      load_periodic_table();
   }
}

Element::~Element()
{

}

int Element::get_id() const
{
   return 1;
}

double Element::get_mass() const
{
   return periodic_table_->find(id_)->second.mass;
}

int Element::get_valence() const
{
   int v = 0;
   auto iter = periodic_table_->find(id_);
   if (iter != periodic_table_->end())
      v = iter->second.valence;

   return v;
}

std::string Element::to_string() const
{
   std::string str;

   auto iter = periodic_table_->find(id_);
   if (iter != periodic_table_->end())
      str = iter->second.small_name;

   return str;
}

bool Element::operator<(const Element& other) const
{
   return id_ < other.id_;
}

bool Element::operator!=(const Element& other) const
{
   return id_ != other.id_;
}

void Element::load_periodic_table()
{
   periodic_table_.reset(new std::map<int, Element::ElementInfo>);
   periodic_table_from_name_.reset(new std::map<std::string, int>);


   std::map<std::string, int> valence;
   {
      YAML::Node table_valence = YAML::LoadFile("//home//fedor//valence.yml");

      for(auto const &it : table_valence["Valence"])
      {

         std::string small_name = it.first.as<std::string>();
         int v = it.second.as<int>();
         valence.emplace(small_name, v);
      }
   }

   YAML::Node table = YAML::LoadFile("//home//fedor//periodic_table.yml");
   for (auto const &period : table["table"])
   {
      for (auto const &elem : period["elements"])
      {
         try {
            ElementInfo info;
            info.name = elem["name"].as<std::string>();
            info.small_name = elem["small"].as<std::string>();
            info.id = elem["number"].as<int>();
            info.mass = elem["molar"].as<double>();

            auto iter_valence = valence.find(info.small_name);
            if (iter_valence != valence.end())
               info.valence = iter_valence->second;
            else
               info.valence = 0;

            periodic_table_from_name_->insert(std::pair<std::string, int>(info.small_name, info.id));

            periodic_table_->emplace(info.id, info);
         } catch (YAML::BadConversion &e) {
         }

      }
   }




}
