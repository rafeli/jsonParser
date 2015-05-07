#include "testJsonParser.hpp"
#include <stddef.h>

calcxx_driver driver;


TestJsonParser::TestJsonParser(){
   
   // -0- init 

   // -1- request logstream
   logStream = TestTools::getLogStream();
   
}

TestJsonParser::~TestJsonParser(){}

void TestJsonParser::testAll(){

  testParseString();

}


void TestJsonParser::testParseString() {
  std::stringstream actual_;
  std::string test_ = "parseString",
          expected_ = "",
           testString;

  try {
    actual_.str("");
    expected_="??";
    testString = "12345";
    driver.parse(testString);
    actual_ << driver.result; 
    TestTools::report(actual_.str(), expected_, test_); 
  } catch (std::string s) {
  }
}
