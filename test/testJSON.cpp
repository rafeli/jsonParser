#include "testJSON.hpp"

TestJSON::TestJSON(){
   
   // -0- init 

   // -1- request logstream
   logStream = momo::TestTools::getLogStream();
   
}


int TestJSON::testAll(){

  int numTests = 0;

  numTests += testParse();

  return numTests;

}


int TestJSON::testParse() {

  std::stringstream actual_;
  int numTests = 0;
  momo::JSON json;
  momo::jsValue jsValue;
  std::string test_ = "parseString",
          expected_ = "",
           testString;

  // -0- Entering
  MYLOG(DEBUG,"ENTER");

  // -1- integer
  try {

    test_ = "integer";
    testString = "123456789";
    jsValue = json.parse(testString);
    momo::TestTools::report(jsValue.stringify(), testString, test_); 
    numTests++;

    testString = "-11";
    jsValue = json.parse(testString);
    momo::TestTools::report(jsValue.stringify(), testString, test_); 
    numTests++;

    testString = " -1 ";
    jsValue = json.parse(testString);
    momo::TestTools::report(jsValue.getInt(), -1, test_); 
    numTests++;

  } catch (std::string s) {
    std::cout << "ERROR in " << test_ <<": " << s;
  }

  // -1a- incorrect integers
  try {
    test_ = "incorrect integer";
    testString = "1 1";
    jsValue = json.parse(testString);
    momo::TestTools::report(false, "didn't recognize as error: 1 1"); 
  } catch (std::string e) {
    momo::TestTools::report(e, "JSON::parse unexpected: 1", test_); 
    numTests++;
  }

  // -2- doubles 
  try {

    test_ = "double";
    testString = "12.3456789";
    jsValue = json.parse(testString);
    momo::TestTools::report(jsValue.stringify(), "1.23456789e+01", test_); 
    numTests++;

    testString = "-11E05";
    jsValue = json.parse(testString);
    momo::TestTools::report(jsValue.stringify(), "-1.1e+06", test_); 
    numTests++;

    testString = " -0.0012E-002 ";
    jsValue = json.parse(testString);
    momo::TestTools::report(jsValue.stringify(), "-1.2e-05", test_); 
    numTests++;

    testString = "-6.022300e+23";
    jsValue = json.parse(testString);
    momo::TestTools::report(jsValue.stringify(), "-6.022300e+23", test_); 
    numTests++;

  } catch (std::string s) {
    std::cout << "ERROR in " << test_ <<": " << s;
  }

  // -2a- incorrect doubles
  try {
    test_ = "incorrect double";
    testString = "A1";
    jsValue = json.parse(testString);
    momo::TestTools::report(false, "didn't recognize as error: A1"); 
  } catch (std::string e) {
    momo::TestTools::report(e, "Error parsing number from: \"A1\" expecting digit, not: A1", test_); 
    numTests++;
  }

  try {
    testString = "1.A1";
    jsValue = json.parse(testString);
    momo::TestTools::report(false, "didn't recognize as error: A1"); 
  } catch (std::string e) {
    momo::TestTools::report(e, "Error parsing number from: \"1.A1\" expecting digit, not: A1", test_); 
    numTests++;
  }

  try {
    testString = "1.2E";
    jsValue = json.parse(testString);
    momo::TestTools::report(false, "didn't recognize as error: A1"); 
  } catch (std::string e) {
    momo::TestTools::report(e, "Error parsing number from: \"1.2E\" expecting digit, not: ", test_); 
    numTests++;
  }

  // -3- string
  try {

    test_ = "string";
    testString = "\"12abc34\"";
    jsValue = json.parse(testString);
    momo::TestTools::report(jsValue.getString(), testString, test_); 
    numTests++;

    testString =     "\"12\tabc\\\"def\n34\"";
    jsValue = json.parse(testString);
    momo::TestTools::report(jsValue.getString(), testString, test_); 
    numTests++;

  } catch (std::string s) {
    std::cout << "ERROR in " << test_ <<": " << s;
  }

//  // -10 a simple array
//  test_ = "parseArray";
//  try {
//    actual_.str("");
//    expected_=  testString = "[12, \"abcdef\", 1.078000e+00]";
//    driver.parse(testString);
//    actual_ << driver.result; 
//    momo::TestTools::report(actual_.str(), expected_, test_); 
//  } catch (std::string s) {
//    std::cout << "ERROR in " << test_ <<": " << s << std::endl;
//  }
//
//  // -11- a simple object
//  test_ = "parseObject";
//  try {
//    actual_.str("");
//    expected_= "12";
//    testString = "{\"myInt\" : 12}";
//    driver.parse(testString);
//    momo::jsObject result = driver.result.getObject();
//    actual_ << result.get("myInt")  ; 
////    actual_ << result.get("myInt") << ", " << result.get("myArray") ; 
//    momo::TestTools::report(actual_.str(), expected_, test_); 
//  } catch (std::string s) {
//    std::cout << "ERROR in " << test_ <<": " << s << std::endl;
//  }
//
//  // -12- a simple object
//  try {
//    actual_.str("");
//    expected_= testString = "{\"myArray\":[12, \"abc\"], \"myInt\":12}";
//    driver.parse(testString);
//    actual_ << driver.result.getObject();
//    momo::TestTools::report(actual_.str(), expected_, test_); 
//  } catch (std::string s) {
//    std::cout << "ERROR in " << test_ <<": " << s << std::endl;
//  }
//
//  // -13- non-JSON string
//  //     (havent invested much time in error messages from bison)
//  try {
//    actual_.str("");
//    expected_= testString = "abc";
//    driver.parse(testString);
//    actual_ << driver.result; 
//    momo::TestTools::report("not OK", "", "syntax error not recognized"); 
//  } catch (std::string s) {
//    momo::TestTools::report(s, "parsing JSON: syntax error, unexpected identifier", "no JSON: parse(\"abc\")"); 
//  }
//
//  try {
//    actual_.str("");
//    expected_= testString = "[-xyz, nan]";
//    driver.parse(testString);
//    actual_ << driver.result; 
//    momo::TestTools::report("not OK", "", "syntax error not recognized"); 
//  } catch (std::string s) {
//    momo::TestTools::report(s, "parsing JSON: syntax error, unexpected identifier, expecting NUMBER_I or NUMBER_F", "no JSON: parse([-xyz])"); 
//  }
//
//  // ?? exit
//  MYLOG(DEBUG,"EXIT");
  return numTests;
}
