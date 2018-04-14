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

  // -1- int positive and negative
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

  } catch (std::string s) {
    std::cout << "ERROR in " << test_ <<": " << s;
  }

  // -2a- incorrect doubles
  try {
    testString = "A1";
    jsValue = json.parse(testString);
    momo::TestTools::report(false, "didn't recognize as error: A1"); 
  } catch (std::string e) {
    momo::TestTools::report(e, "??", test_); 
    numTests++;
  }

  try {
    testString = "1.A1";
    jsValue = json.parse(testString);
    momo::TestTools::report(false, "didn't recognize as error: A1"); 
  } catch (std::string e) {
    momo::TestTools::report(e, "??", test_); 
    numTests++;
  }

  try {
    testString = "1.2E";
    jsValue = json.parse(testString);
    momo::TestTools::report(false, "didn't recognize as error: A1"); 
  } catch (std::string e) {
    momo::TestTools::report(e, "??", test_); 
    numTests++;
  }

//
//  // -3- just a string
//  try {
//    actual_.str("");
//    expected_ = "\"12abc34\"";
//    testString = "\"12abc34\"";
//    driver.parse(testString);
//    actual_ << driver.result; 
//    momo::TestTools::report(actual_.str(), expected_, "just a string"); 
//    MYLOG(DEBUG,"Test String complete");
//  } catch (std::string s) {
//    std::cout << "ERROR in " << test_ <<": " << s << std::endl;
//  }
//
//  // -4- single quoted string
//  try {
//    actual_.str("");
//    expected_= testString =  "\"12abc34\"";
//    driver.parse(testString);
//    actual_ << driver.result; 
//    momo::TestTools::report(actual_.str(), expected_, "a single quoted string"); 
//    MYLOG(DEBUG,"Test Single Quoted String complete");
//  } catch (std::string s) {
//    std::cout << "ERROR in " << test_ <<": " << s << std::endl;
//  }
//
//  // -5- string with \t and \n and \"
//  //     changed to \\t and \\n, as these chars are encoded in JSON
//  try {
//    testString =     "\"12\tabc\\\"def\n34\"";
//    expected_=    "12\tabc\"def\n34";
//    driver.parse(testString);
//    momo::TestTools::report(driver.result.getString(), expected_, "single quoted string containing tab and cr"); 
//    MYLOG(DEBUG,"Test Single Quoted String complete");
//  } catch (std::string s) {
//    std::cout << "ERROR in " << test_ <<": " << s << std::endl;
//  }
//
//
//
//  // -6- double in scientific format
//  test_ = "parseDouble scientific format";
//  try {
//    actual_.str("");
//    testString = "6.022300e-23";
//    driver.parse(testString);
//    momo::TestTools::report(driver.result.stringify(), testString, test_); 
//  } catch (std::string s) {
//    std::cout << "ERROR in " << test_ <<": " << s << std::endl;
//  }
//
//  // -7- double in scientific format
//  test_ = "parseDouble negative ";
//  try {
//    actual_.str("");
//    testString = "-6.022300e+23";
//    driver.parse(testString);
//    momo::TestTools::report(driver.result.stringify(), testString, test_); 
//  } catch (std::string s) {
//    std::cout << "ERROR in " << test_ <<": " << s << std::endl;
//  }
//
//  // -8- double in fixed format: precision set to LOWPRECISION = 2
//  test_ = "parseDouble fixed format";
//  try {
//    testString = "17.0500";
//    driver.parse(testString);
//    momo::TestTools::report(driver.result.stringify(), "1.70500e+01", test_); 
//    MYLOG(DEBUG,"Test double complete");
//  } catch (std::string s) {
//    std::cout << "ERROR in " << test_ <<": " << s << std::endl;
//  }
//
//  // -9- double in non-standard format: precision set to LOWPRECISION = 2
//  test_ = "parseDouble non-standard format";
//  try {
//    testString = "16.000e-3";
//    driver.parse(testString);
//    momo::TestTools::report(driver.result.stringify(), "1.60e-02", test_); 
//    MYLOG(DEBUG,"Test double complete");
//  } catch (std::string s) {
//    std::cout << "ERROR in " << test_ <<": " << s << std::endl;
//  }
//
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
