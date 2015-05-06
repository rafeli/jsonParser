#ifndef _TESTJsonParser_HPP
#define _TESTJsonParser_HPP

#include "testTools.hpp"
#include "momo/logging.hpp"

class TestJsonParser {

private:
  std::ofstream *logStream;

public:

  TestJsonParser();

  ~TestJsonParser();

  void testAll();

  void testParseString();


};



#endif // _TESTJsonParser_HPP
