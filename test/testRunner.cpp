#include <iostream>

#include "testJSON.hpp"
#include "testJsValue.hpp"
#include "momo/testTools.hpp"
#include "momo/logging.hpp"

int main(int argc, char ** argv) {

  std::string logFileName="test.log";
  int numTests = 0 ;

  Logging::prepare();

  // -1- open logfile

  if (argc==2) {
    logFileName = argv[1];
  }
  momo::TestTools::prepare(logFileName);

  // -2- test each unit
  TestJSON testJSON;
  numTests += testJSON.testAll();
  TestJsValue testJsValue;
  numTests += testJsValue.testAll();

  // -3- finalize
  momo::TestTools::testingComplete();
  std::cout << numTests << " tests finished \n";

}
