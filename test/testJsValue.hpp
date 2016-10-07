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

  void testInt();

  void testString();

  void testDouble();

  void testArray();

  void testObject();

  int testConstructors();


};



#endif // _TESTJsValue_HPP
