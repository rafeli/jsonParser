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

  // -1- just an int
  try {
    actual_.str("");
    expected_= testString = "12345";
    driver.parse(testString);
    actual_ << driver.result; 
    TestTools::report(actual_.str(), expected_, test_); 
  } catch (std::string s) {
    std::cout << "ERROR in " << test_ <<": " << s;
  }

  // -2- just a string
  try {
    actual_.str("");
    expected_=  testString = "\"12abc34\"";
    driver.parse(testString);
    actual_ << driver.result; 
    TestTools::report(actual_.str(), expected_, test_); 
  } catch (std::string s) {
    std::cout << "ERROR in " << test_ <<": " << s << std::endl;
  }

  // -2a- single quoted string
  try {
    actual_.str("");
    expected_=  testString = "'12abc34'";
    driver.parse(testString);
    actual_ << driver.result; 
    TestTools::report(actual_.str(), expected_, test_); 
  } catch (std::string s) {
    std::cout << "ERROR in " << test_ <<": " << s << std::endl;
  }

  // -2- just a double
  try {
    actual_.str("");
    expected_=  testString = "6.0223E-23";
    driver.parse(testString);
    actual_ << driver.result; 
    TestTools::report(actual_.str(), expected_, test_); 
  } catch (std::string s) {
    std::cout << "ERROR in " << test_ <<": " << s << std::endl;
  }

}
