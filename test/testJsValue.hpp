#ifndef _TESTJsValue_HPP
#define _TESTJsValue_HPP

#include "momo/testTools.hpp"
#include "momo/logging.hpp"
#include "../src/jsValue.hpp"

class TestJsValue {

private:

  std::ofstream *logStream;

public:

  TestJsValue();

  ~TestJsValue();

  int testAll();

  int testArray();

  int testObject();

  int testConstructors();

};



#endif // _TESTJsValue_HPP
