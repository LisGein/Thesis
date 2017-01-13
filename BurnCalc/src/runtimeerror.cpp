#include "runtimeerror.h"

RunTimeError::RunTimeError(const std::__cxx11::string& what_arg, const std::__cxx11::string& message)
   : std::runtime_error(what_arg)
   , arg_(what_arg)
   , message_(message)
{

}

std::__cxx11::string RunTimeError::arg() const
{
   return arg_;
}

std::__cxx11::string RunTimeError::message() const
{
   return message_;
}
