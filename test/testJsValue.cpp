#include "testJsValue.hpp"


TestJsValue::TestJsValue(){
   
   // -0- init 

   // -1- request logstream
   logStream = TestTools::getLogStream();
   
}

TestJsValue::~TestJsValue(){}

int TestJsValue::testAll(){
  int numTests = 0;

  numTests +=   testInt();

  numTests +=   testArray();

  numTests +=   testObject();

  numTests +=   testString();

  numTests += testConstructors();

  return numTests;
}


int TestJsValue::testString(){

  std::string plain="ab@cd",                               //  abcd
          quote="ein \" quote",                            //  ein " quote
          escQuote="escaped \\\" quote",                   //  escaped \" quote
          linebreak = "ab\ncd",                            //  escaped linebreak
          plainJSON = "\"ab@cd\"",                          //  "abcd"
          linebreakJSON = "\"ab\\ncd\"",                   //  "ab\ncd"
          quoteJSON = "\"ein \\\" quote\"",                //  "ein \" quote"
          escQuoteJSON = "\"escaped \\\\\\\" quote\"";     //  "escaped \\\" quote"

  jsValue jsPlain(plain), jsQuote(quote), jsEscQuote(escQuote), jsLineBreak(linebreak);

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
    TestTools::report(jsLineBreak.stringify(), linebreakJSON, "jsValue(string w. \\n).stringify()");

    // -2- getString should give original string 
    TestTools::report(jsPlain.getString(), plain, "jsValue(plain string).getString()");
    TestTools::report(jsQuote.getString(), quote, "jsValue(string w. double quote).getString()");
    TestTools::report(jsEscQuote.getString(), escQuote, "jsValue(string w. escape).getString()");
    TestTools::report(jsLineBreak.getString(), linebreak, "jsValue(string w \\n).getString()");

    // -3- same should work with getJSONValue
    TestTools::report(getJSONValue(plainJSON).stringify(), plainJSON, "getJSONValue(plain string)");
    TestTools::report(getJSONValue(quoteJSON).stringify(), quoteJSON, "getJSONValue(quote)");
    TestTools::report(getJSONValue(escQuoteJSON).stringify(), escQuoteJSON, "getJSONValue(escaped quote)");
    TestTools::report(getJSONValue(linebreakJSON).stringify(), linebreakJSON, "getJSONValue(string w. \\n)");

    TestTools::report(getJSONValue(plainJSON).getString(), plain, "getJSONValue(plain string)");
    TestTools::report(getJSONValue(quoteJSON).getString(), quote, "getJSONValue(quote)");
    TestTools::report(getJSONValue(escQuoteJSON).getString(), escQuote, "getJSONValue(escaped quote)");
    TestTools::report(getJSONValue(linebreakJSON).getString(), linebreak, "getJSONValue(string w \\n)");

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

  return 20;
}

int TestJsValue::testInt(){

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

  return 1;
}

int TestJsValue::testArray(){

  jsValue intVal(17),
          stringVal("abcd");
  std::vector<jsValue> myVector;
  int numTests = 0;

  try{
    std::stringstream actual_;
    std::string test_ = "testArray",
                expected_ = "";
    // -0- 
    MYLOG(DEBUG, "entering");
    myVector.clear();
    myVector.push_back(intVal);
    myVector.push_back(stringVal);
    jsValue arrayVal((myVector)); 

    // -1-   test read JSON Array 
    actual_.str("");
    expected_ = "[17, \"abcd\"]";
    actual_ <<  arrayVal ;
    TestTools::report(actual_.str(), expected_, test_);
    numTests++;

    // -2- test read CPP Array into JSON
    actual_.str("");
    expected_ = "[\"aaa\", \"bbb\"][17, -129][-1.00000e+00, 1.41423e+00]";
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
    jsValue x3(dblVector, 5); // 5 = precision
    std::string x3s = x3.stringify();
    actual_ <<  x1 << x2 << x3s ;
    TestTools::report(actual_.str(), expected_, test_);
    numTests++;

    // -3- (2017) accessing array of objects
    jsValue myObjects(std::vector<jsValue>{});
    myObjects.add(jsValue(jsObject()));
    myObjects.add(jsValue(jsObject()));
    myObjects.add(jsValue(jsObject()));
    myObjects.getRef(1).add("name",jsValue("objB"));
    myObjects.getRef(2).add("name",jsValue("objC"));
    myObjects.getRef(0).add("name",jsValue("objA, not:" + myObjects.getRef(1).getRef("name").getString()));
    TestTools::report(myObjects.getRef(0).getRef("name").getString(), "objA, not:objB", test_);
    numTests++;
    

    // ???
    MYLOG(DEBUG, "EXIT");
  } catch (std::string s) {
    std::cout << "Exception thrown in testArray: " << s << std::endl;
  }

  return numTests;
}

int TestJsValue::testObject(){

  jsValue intVal(17),
          stringVal("abcd"),
          dblVal(1.17E+003, FULLPRECISION);  // 
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
//    jsValue arrayVal(std::move(myVector));
    jsValue arrayVal((myVector));
    myObject.add("myInt", intVal);
    myObject.add("myString", stringVal);
    myObject.add("myDouble", dblVal);
    myObject.add("myVector", arrayVal);


    // -1-   test  
    actual_.str("");
    expected_ = "{\"myDouble\":1.1700000000000000e+03, \"myInt\":17, \"myString\":\"abcd\", \"myVector\":[17, \"abcd\", 1.1700000000000000e+03]}";
    actual_ <<  myObject ;
    TestTools::report(actual_.str(), expected_, test_);


    // ???
    MYLOG(DEBUG, "EXIT");
  } catch (std::string s) {
    std::cout << "Exception thrown in testArray: " << s << std::endl;
  }

  return 2;
}

int TestJsValue::testConstructors(){

  // -1- construct from int
  try {
    int a=17, b=-3, c=0;
    jsValue x(a), *y = new jsValue(b), z = jsValue(c);

    if (x.getInt() == a && y->getInt()==b && z.getInt() == c) {
       TestTools::report("ok", "ok" , "jsValue(int)");
    } else {
       TestTools::report("Error:", "" , "jsValue(int)");
    }
  } catch (std::string e) {
    TestTools::report("Error:", e , "jsValue(int)");
  }

  // -2- construct from string
  try {
    std::string a="aa", b="C\"", c="";
    jsValue x(a), *y = new jsValue(b), z = jsValue(c);

    if (x.getString() == a && y->getString()==b && z.getString() == c) {
       TestTools::report("ok", "ok" , "jsValue(string)");
    } else {
       TestTools::report("Error:", "" , "jsValue(string)");
    }
  } catch (std::string e) {
    TestTools::report("Error:", e , "jsValue(string)");
  }

  // -3- construct from double
  try {
    double a=0.4, b=-1.789E-99, c=0;
    jsValue x(a), *y = new jsValue(b), z = jsValue(c);

    if (x.getDbl() == a && y->getDbl()==b && z.getDbl() == c) {
       TestTools::report("ok", "ok" , "jsValue(double)");
    } else {
       TestTools::report("Error:", "" , "jsValue(double)");
    }
  } catch (std::string e) {
    TestTools::report("Error:", e , "jsValue(double)");
  }

  // -4- construct from std::vector<double>
  try {
    double a=0.4, b=-1.789E-19, c=0;
    std::vector<double> myVector = {a,b,c};
    jsValue x(myVector), *y = new jsValue(myVector), z = jsValue(myVector);

    if (x.getArray()[0].getDbl()==a && y->getArray()[1].getDbl()==b && z.getArray()[2].getDbl()==c) {
       TestTools::report("ok", "ok" , "jsValue(std::vector<double>)");
    } else {
       TestTools::report("Error:", "" , "jsValue(std::vector<double>)");
    }
  } catch (std::string e) {
    TestTools::report("Error:", e , "jsValue(std::vector<double>)");
  }

  // -5- construct from std::vector<string>
  try {
    std::string a="aa", b="C\"", c="";
    std::vector<std::string> myVector = {a,b,c};
    jsValue x(myVector), *y = new jsValue(myVector), z = jsValue(myVector);

    if (x.getArray()[0].getString()==a && y->getArray()[1].getString()==b && z.getArray()[2].getString()==c) {
       TestTools::report("ok", "ok" , "jsValue(std::vector<double>)");
    } else {
       TestTools::report("Error:", "" , "jsValue(std::vector<double>)");
    }
  } catch (std::string e) {
    TestTools::report("Error:", e , "jsValue(std::vector<double>)");
  }

  // -6- construct from std::vector<jsValue>
  try {
    double a=0.4, b=-1.789E-19, c=0;
    std::vector<jsValue> myVector = {jsValue(a),jsValue(b),jsValue(c)};
    jsValue x(myVector), *y = new jsValue(myVector), z = jsValue(myVector);

    if (x.getArray()[0].getDbl()==a && y->getArray()[1].getDbl()==b && z.getArray()[2].getDbl()==c) {
       TestTools::report("ok", "ok" , "jsValue(std::vector<double>)");
    } else {
       TestTools::report("Error:", "" , "jsValue(std::vector<double>)");
    }
  } catch (std::string e) {
    TestTools::report("Error:", e , "jsValue(std::vector<double>)");
  }

  // -7- construct from jsObject
  try {
    jsObject o;
    double a= -1.789E-19;
    o.add("x",jsValue(a));
    jsValue x(o), *y = new jsValue(o), z = jsValue(o);

    // TODO: implement == for jsValue and jsObject
    if (x.getObject().get("x").getDbl()==a) {
       TestTools::report("ok", "ok" , "jsValue(std::vector<double>)");
    } else {
       TestTools::report("Error:", "" , "jsValue(std::vector<double>)");
    }

    if (y->getObject().get("x").getDbl()==a) {
       TestTools::report("ok", "ok" , "jsValue(std::vector<double>)");
    } else {
       TestTools::report("Error:", "" , "jsValue(std::vector<double>)");
    }

    if (z.getObject().get("x").getDbl()==a) {
       TestTools::report("ok", "ok" , "jsValue(std::vector<double>)");
    } else {
       TestTools::report("Error:", "" , "jsValue(std::vector<double>)");
    }
  } catch (std::string e) {
    TestTools::report("Error:", e , "jsValue(std::vector<double>)");
  }

  return 15; 
}
