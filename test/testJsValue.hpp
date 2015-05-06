#ifndef _TESTJsValue_HPP
#define _TESTJsValue_HPP

#include "testTools.hpp"
#include "momo/logging.hpp"
#include "../src/jsValue.hpp"

class TestJsValue {

private:
  std::ofstream *logStream;

public:

  TestJsValue();

  ~TestJsValue();

  void testAll();

  void testInt();

  void testString();

  void testDouble();

  void testArray();


};



#endif // _TESTJsValue_HPP
