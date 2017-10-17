#ifndef _TESTJsonParser_HPP
#define _TESTJsonParser_HPP

#include "momo/testTools.hpp"
#include "momo/logging.hpp"
#include "../src/json.hpp"

class TestJsonParser {

private:
  std::ofstream *logStream;

public:

  TestJsonParser();

  ~TestJsonParser();

  int testAll();

  int testParseString();


};



#endif // _TESTJsonParser_HPP
