#include "testJsValue.hpp"


TestJsValue::TestJsValue(){
   
   // -0- init 

   // -1- request logstream
   logStream = TestTools::getLogStream();
   
}

TestJsValue::~TestJsValue(){}

void TestJsValue::testAll(){

  testInt();

  testArray();

  testObject();

  testString();

}


void TestJsValue::testString(){

  std::string plain="abcd",                                //  abcd
          quote="ein \" quote",                            //  ein " quote
          escQuote="escaped \\\" quote",                   //  escaped \" quote
          plainJSON = "\"abcd\"",                          //  "abcd"
          quoteJSON = "\"ein \\\" quote\"",                //  "ein \" quote"
          escQuoteJSON = "\"escaped \\\\\\\" quote\"";     //  "escaped \\\" quote"

  jsValue jsPlain(plain), jsQuote(quote), jsEscQuote(escQuote);

  try{
    std::stringstream actual_;
    std::string test_ = "testString",
                expected_ = "";
    // -0- 
    MYLOG(DEBUG, "ENTER");

    // -1- test constructor, these should take strings (without outer double quotes)
    //     and handle contained double quotes correctly  
    TestTools::report(jsPlain.stringify(), plainJSON, "jsValue(plain string).stringify()");
    TestTools::report(jsQuote.stringify(), quoteJSON, "jsValue(string w. double quote).stringify()");
    TestTools::report(jsEscQuote.stringify(), escQuoteJSON, "jsValue(string w. escape).stringify()");

    // -2- getString should give original string 
    TestTools::report(jsPlain.getString(), plain, "jsValue(plain string).getString()");
    TestTools::report(jsQuote.getString(), quote, "jsValue(string w. double quote).getString()");
    TestTools::report(jsEscQuote.getString(), escQuote, "jsValue(string w. escape).getString()");

    // -3- same should work with getJSONValue, but this takes a JSON-string,
    //     i.e. outer double quotes must be contained
    TestTools::report(getJSONValue(plainJSON).stringify(), plainJSON, "getJSONValue(plain string)");
    TestTools::report(getJSONValue(quoteJSON).stringify(), quoteJSON, "getJSONValue(quote)");
    TestTools::report(getJSONValue(escQuoteJSON).stringify(), escQuoteJSON, "getJSONValue(escaped quote)");

    TestTools::report(getJSONValue(plainJSON).getString(), plain, "getJSONValue(plain string)");
    TestTools::report(getJSONValue(quoteJSON).getString(), quote, "getJSONValue(quote)");
    TestTools::report(getJSONValue(escQuoteJSON).getString(), escQuote, "getJSONValue(escaped quote)");

    // -4- tests on incorrect strings
    try {
      getJSONValue("a\\b").getString();
      TestTools::report("a\\b incorrect", getJSONValue("a\\b").getString(), "getJSONValue(non-json)");
    } catch (std::string s) {
      TestTools::report("a\\b no JSON", "a\\b no JSON", "getJSONValue(non-json)");
    }

    try {
      getJSONValue("a\"b").getString();
      TestTools::report("a\"b incorrect", getJSONValue("a\\b").getString(), "getJSONValue(non-json)");
    } catch (std::string s) {
      TestTools::report("a\"b no JSON", "a\"b no JSON", "getJSONValue(non-json)");
    }

    try {
      getJSONValue("a\\\\\"b").getString();
      TestTools::report("a\\\\\"b incorrect", getJSONValue("a\\\\\"b").getString(), "getJSONValue(non-json)");
    } catch (std::string s) {
      TestTools::report("a\\\\\"b no JSON", "a\\\\\"b no JSON", "getJSONValue(non-json)");
    }

    // ???
    MYLOG(DEBUG, "EXIT");
  } 
  catch(std::string s) {
    std::cout << "Exception thrown in testString: " << s << std::endl;
  }
}

void TestJsValue::testInt(){

  jsValue intVal( 17),
          stringVal("abcd");

  try{
    std::stringstream actual_;
    std::string test_ = "testInt",
                expected_ = "";
    // -0- 
    MYLOG(DEBUG, "ENTER");

    // -1-   test  
    actual_.str("");
    expected_ = "\"abcd\"17";
    actual_ <<  stringVal << intVal ;
    TestTools::report(actual_.str(), expected_, test_);

    // ???
    MYLOG(DEBUG, "EXIT");
  } 
  catch(char const *s) {
    std::cout << "Exception thrown in testInt: " << s << std::endl;
  }
}

void TestJsValue::testArray(){

  jsValue intVal(17),
          stringVal("abcd");
  std::vector<jsValue> myVector;

  try{
    std::stringstream actual_;
    std::string test_ = "testArray",
                expected_ = "";
    // -0- 
    MYLOG(DEBUG, "entering");
    myVector.clear();
    myVector.push_back(intVal);
    myVector.push_back(stringVal);
    jsValue arrayVal(std::move(myVector)); // 2016: was macht move noch mal ???

    // -1-   test read JSON Array 
    actual_.str("");
    expected_ = "[17, \"abcd\"]";
    actual_ <<  arrayVal ;
    TestTools::report(actual_.str(), expected_, test_);

    // -2- test read CPP Array into JSON
    actual_.str("");
    expected_ = "[\"aaa\", \"bbb\"][17, -129][-1.000000e+00, 1.414230e+00]";
    std::vector<std::string> stringVector;
    std::vector<long> intVector;
    std::vector<double> dblVector;
    stringVector.push_back("aaa");
    stringVector.push_back("bbb");
    intVector.push_back(17);
    intVector.push_back(-129);
    dblVector.push_back(-1.0);
    dblVector.push_back(1.41423);
    jsValue x1(stringVector);
    jsValue x2(intVector);
    jsValue x3(dblVector);
    std::string x3s = x3.stringify();
    actual_ <<  x1 << x2 << x3s ;
    TestTools::report(actual_.str(), expected_, test_);
    

    // ???
    MYLOG(DEBUG, "EXIT");
  } catch (std::string s) {
    std::cout << "Exception thrown in testArray: " << s << std::endl;
  }
}

void TestJsValue::testObject(){

  jsValue intVal(17),
          stringVal("abcd"),
          dblVal(1.17E+003);
  std::vector<jsValue> myVector;
  jsObject myObject;

  try{
    std::stringstream actual_;
    std::string test_ = "testObject",
                expected_ = "";
    // -0- 
    MYLOG(DEBUG, "entering");
    myVector.clear();
    myVector.push_back(intVal);
    myVector.push_back(stringVal);
    myVector.push_back(dblVal);
    jsValue arrayVal(std::move(myVector));
    myObject.add("myInt", intVal);
    myObject.add("myString", stringVal);
    myObject.add("myDouble", dblVal);
    myObject.add("myVector", arrayVal);


    // -1-   test  
    actual_.str("");
    expected_ = "{\"myDouble\":1.170000e+03, \"myInt\":17, \"myString\":\"abcd\", \"myVector\":[17, \"abcd\", 1.170000e+03]}";
    actual_ <<  myObject ;
    TestTools::report(actual_.str(), expected_, test_);


    // ???
    MYLOG(DEBUG, "EXIT");
  } catch (std::string s) {
    std::cout << "Exception thrown in testArray: " << s << std::endl;
  }
}

