#include <iostream>

#include "testJsonParser.hpp"
#include "testJsValue.hpp"
#include "testTools.hpp"
#include "momo/logging.hpp"

int main(int argc, char ** argv) {

  std::string logFileName="test.log";
  int numTests = 0 ;

  Logging::prepare();

  // -1- open logfile

  if (argc==2) {
    logFileName = argv[1];
  }
  TestTools::prepare(logFileName);

  // -2- test each unit
  TestJsonParser testJsonParser;
  numTests += testJsonParser.testAll();
  TestJsValue testJsValue;
  numTests += testJsValue.testAll();

  // -3- finalize
  TestTools::testingComplete();
  std::cout << numTests << " tests finished \n";

}
