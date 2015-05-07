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
  return res;
}

