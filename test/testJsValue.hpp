#ifndef _TESTJsValue_HPP
#define _TESTJsValue_HPP

#include "testTools.hpp"
#include "momo/logging.hpp"
#include "../src/jsValue.hpp"
#include "../src/json.hpp"

class TestJsValue {

private:
  std::ofstream *logStream;

public:

  TestJsValue();

  ~TestJsValue();

  int testAll();

  int testInt();

  int testString();

  int testDouble();

  int testArray();

  int testObject();

  int testConstructors();


};



#endif // _TESTJsValue_HPP
