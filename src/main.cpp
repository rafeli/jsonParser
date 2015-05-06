#include <iostream>
#include "driver.hpp"


int
main (int argc, char *argv[])
{
  std::string test = "{ abc : 889 }";
  int res = 0;
  calcxx_driver driver;
  driver.parse(test);
  std::cout << driver.result << std::endl;
//  for (int i = 1; i < argc; ++i)
//    if (argv[i] == std::string ("-p"))
//      driver.trace_parsing = true;
//    else if (argv[i] == std::string ("-s"))
//      driver.trace_scanning = true;
//    else if (!driver.parse (argv[i]))
//      std::cout << driver.result << std::endl;
//    else
//      res = 1;
  return res;
}

