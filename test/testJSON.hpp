#ifndef _TESTJSON_HPP
#define _TESTJSON_HPP

#include "momo/testTools.hpp"
#include "momo/logging.hpp"
#include "../src/JSON.hpp"

class TestJSON {

private:

  std::ofstream *logStream;

public:

  TestJSON();

  int testAll();

  int testParse();


};



#endif // _TESTJON
