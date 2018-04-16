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
  momo::json json;
  momo::jsValue myJSValue;
  std::string test_ = "parseString",
          expected_ = "",
           testString;

  // -0- Entering
  MYLOG(DEBUG,"ENTER");

  // -1- integer
  try {

    test_ = "JSON::parse(integer) ";
    testString = "123456789";
    myJSValue = json.parse(testString);
    momo::TestTools::report(myJSValue.stringify(), testString, test_); 
    numTests++;

    testString = "-11";
    myJSValue = json.parse(testString);
    momo::TestTools::report(myJSValue.stringify(), testString, test_); 
    numTests++;

    testString = " -1 ";
    myJSValue = json.parse(testString);
    momo::TestTools::report(myJSValue.getInt(), -1, test_); 
    numTests++;

  } catch (std::string s) {
    std::cout << "ERROR in " << test_ <<": " << s;
  }

  // -1a- incorrect integers
  try {
    test_ = "json::parse(incorrect integer) ";
    testString = "1 1";
    myJSValue = json.parse(testString);
    momo::TestTools::report(false, "didn't recognize as error: 1 1"); 
  } catch (std::string e) {
    momo::TestTools::report(e, "json::parse unexpected: 1", test_); 
    numTests++;
  }

  // -2- doubles 
  try {

    test_ = "json::parse(double) ";
    testString = "12.3456789";
    myJSValue = json.parse(testString);
    momo::TestTools::report(myJSValue.stringify(), "1.23456789e+01", test_); 
    numTests++;

    testString = "-11E05";
    myJSValue = json.parse(testString);
    momo::TestTools::report(myJSValue.stringify(), "-1.1e+06", test_); 
    numTests++;

    testString = " -0.0012E-002 ";
    myJSValue = json.parse(testString);
    momo::TestTools::report(myJSValue.stringify(), "-1.2e-05", test_); 
    numTests++;

    testString = "-6.022300e+23";
    myJSValue = json.parse(testString);
    momo::TestTools::report(myJSValue.stringify(), "-6.022300e+23", test_); 
    numTests++;

  } catch (std::string s) {
    std::cout << "ERROR in " << test_ <<": " << s;
  }

  // -2a- incorrect doubles
  try {
    test_ = "json::parse(incorrect double) ";
    testString = "A1";
    myJSValue = json.parse(testString);
    momo::TestTools::report(false, "didn't recognize as error: A1"); 
  } catch (std::string e) {
    momo::TestTools::report(e, "Error parsing number from: \"A1\" expecting digit, not: A1", test_); 
    numTests++;
  }

  try {
    testString = "1.A1";
    myJSValue = json.parse(testString);
    momo::TestTools::report(false, "didn't recognize as error: A1"); 
  } catch (std::string e) {
    momo::TestTools::report(e, "Error parsing number from: \"1.A1\" expecting digit, not: A1", test_); 
    numTests++;
  }

  try {
    testString = "1.2E";
    myJSValue = json.parse(testString);
    momo::TestTools::report(false, "didn't recognize as error: A1"); 
  } catch (std::string e) {
    momo::TestTools::report(e, "Error parsing number from: \"1.2E\" expecting digit, not: ", test_); 
    numTests++;
  }

  // -3- string
  try {

    test_ = "json::parse(string) ";
    testString = "12abc34";
    momo::TestTools::report(json.parse("\""+testString+"\"").getString(), testString, test_); 

    testString =  "ab@cd";
    momo::TestTools::report(json.parse("\""+testString+"\"").getString(), testString, test_); 

    testString =  "ab\\ncd";
    momo::TestTools::report(json.parse("\""+testString+"\"").getString(), testString, test_); 

    testString =  "ab\\\"cd";
    momo::TestTools::report(json.parse("\""+testString+"\"").getString(), testString, test_); 

    testString =  "ab\ncd";
    momo::TestTools::report(json.parse("\""+testString+"\"").getString(), testString, test_); 

    testString =     "12\tabc\\\"def\n34";
    momo::TestTools::report(json.parse("\""+testString+"\"").getString(), testString, test_); 

    numTests += 3;

  } catch (std::string s) {
    std::cout << "ERROR in " << test_ <<": " << s;
  }

  // -3A- non-JSON string
  try {
    test_ = "string without double quotes";
    testString = "abc";
    myJSValue = json.parse(testString);
    momo::TestTools::report("not OK", "", "syntax error not recognized"); 
  } catch (std::string e) {
    momo::TestTools::report(e, "Error parsing number from: \"abc\" expecting digit, not: abc", test_); 
  }

  try {
    test_ = "string with internal unescaped double quote";
    testString = "ab\"cd";
    myJSValue = json.parse("\"" + testString + "\"");
    momo::TestTools::report("not OK", "", "syntax error not recognized"); 
  } catch (std::string e) {
    momo::TestTools::report(e, "json::parse unexpected: cd\"", test_); 
  }

  // -4- array
  try {

    test_ = "json::parse(array) ";
    testString = "[12, \"abcdef\", 1.078000e+00, true, null]";
    myJSValue = json.parse(testString);
    momo::TestTools::report(myJSValue.stringify(), testString, test_); 
    numTests++;

  } catch (std::string s) {
    std::cout << "ERROR in " << test_ <<": " << s;
  }

  // -4a- an incorrect array
  try {
    test_ = "json::parse(incorrect array) ";
    testString = "[-xyz, nan]";
    myJSValue = json.parse(testString);
    momo::TestTools::report(false, "didn't recognize as error: [-xyz, nan]"); 
  } catch (std::string e) {
    momo::TestTools::report(e, "json::parseExpectJSArray: \"[-xyz, nan\" "
      "Error parsing number from: \"-xyz, nan]\" expecting digit, not: xyz", test_); 
    numTests++;
  }

  // -5- a simple object
  try {

    test_ = "json::parse(Object) ";

    // one key-value-pair
    testString = "{\"myInt\":12}";
    momo::TestTools::report(json.parse(testString).stringify(), testString, test_); 

    // one key-value-pair with non-standard spaces (cannot test by stringify)
    testString = "{ \"myInt\" : 12}";
    myJSValue = json.parse(testString);
    momo::TestTools::report(myJSValue.getInt("myInt"), 12, test_); 

    // two key-value-pairs
    testString = "{\"myArray\":[12, \"abc\"], \"myBool\":false, \"myInt\":12}";
    momo::TestTools::report(json.parse(testString).stringify(), testString, test_); 

    // two key-value-pairs with non-standard spaces (cannot test by stringify)
    testString = "{ \"myArray\": [12, \"abc\"],\"myInt\" :17, \"myBool\" : false}";
    momo::jsValue myJSArray = json.parse(testString).getArray("myArray");
    momo::TestTools::report(myJSArray.size(), 2, test_); 
    momo::TestTools::report(myJSArray.getInt(0), 12, test_); 
    momo::TestTools::report(myJSArray.getString(1), "abc", test_); 
    momo::TestTools::report(json.parse(testString).getInt("myInt"), 17, test_); 
    momo::TestTools::report(json.parse(testString).getBool("myBool"), false, test_); 

    numTests += 4;

  } catch (std::string s) {
    std::cout << "ERROR in " << test_ <<": " << s << std::endl;
  }

  return numTests;
}
