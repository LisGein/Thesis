#pragma once
#include <stdexcept>

class RunTimeError
      : public std::runtime_error
{
public:
   explicit RunTimeError(const std::string& what_arg, const std::string &message);
   std::string arg() const;
   std::string message() const;

private:
   std::string arg_;
   std::string message_;

};
