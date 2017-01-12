#pragma once

#include <memory>
#include <map>
#include <string>

/// class Element represents chemical element from periodic table
class Element
{
public:
   Element(int id = -1);
   ~Element();

   ///\return element id in Periodic table
   int get_id() const;

   ///\return atomic mass of chemical element
   double get_mass() const;

   int get_valence() const;


   std::string to_string() const;



   bool operator<(Element const &other) const;
   bool operator!=(Element const &other) const;

private:
   struct ElementInfo
   {
      int id;
      std::string name;
      std::string small_name;
      double mass;
      int valence;
   };


private:
   int id_;

public:
   static void load_periodic_table();


   static std::unique_ptr<std::map<int, ElementInfo>> periodic_table_;
   static std::unique_ptr<std::map<std::string, int>> periodic_table_from_name_;
};
