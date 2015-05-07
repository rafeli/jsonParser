#include "driver.hpp"
#include "parser.tab.hh"

calcxx_driver::calcxx_driver ()
  : trace_scanning (false), trace_parsing (false)
{
  variables["one"] = 1;
  variables["two"] = 2;
}

calcxx_driver::~calcxx_driver ()
{
}

int
calcxx_driver::parse (const std::string &s)
{
//  file = f;
  scan_begin (s, false); // 2nd parameter turns Flex messages on/off
  yy::calcxx_parser parser (*this);
  parser.set_debug_level (trace_parsing);
  int res = parser.parse ();
  return res;
}

void
calcxx_driver::error (const yy::location& l, const std::string& m)
{
  std::stringstream errorMessage;
  errorMessage.str("");
  errorMessage << l << ": " << m ;
  throw errorMessage.str();
}

void
calcxx_driver::error (const std::string& m)
{
  throw "from jsonParser: " + m;
}
