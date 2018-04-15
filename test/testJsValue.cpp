#include "testJsValue.hpp"


TestJsValue::TestJsValue(){
   
   // -0- init 

   // -1- request logstream
   logStream = momo::TestTools::getLogStream();
   
}

TestJsValue::~TestJsValue(){}

int TestJsValue::testAll(){

  int numTests = 0;

  numTests +=   testArray();

  numTests +=   testObject();

  numTests += testConstructors();

  return numTests;
}


int TestJsValue::testArray(){

  momo::jsValue intVal(17),
          stringVal("abcd");
  std::vector<momo::jsValue> myVector;
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
    momo::jsValue arrayVal((myVector)); 

    // -1-   test read JSON Array 
    actual_.str("");
    expected_ = "[17, \"abcd\"]";
    actual_ <<  arrayVal ;
    momo::TestTools::report(actual_.str(), expected_, test_);
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
    momo::jsValue x1(stringVector);
    momo::jsValue x2(intVector);
    momo::jsValue x3(dblVector, 5); // 5 = precision
    std::string x3s = x3.stringify();
    actual_ <<  x1 << x2 << x3s ;
    momo::TestTools::report(actual_.str(), expected_, test_);
    numTests++;

    // -3- (2017) accessing array of objects
    momo::jsValue myObjects(std::vector<momo::jsValue>{});
    myObjects.add(momo::jsValue());
    myObjects.add(momo::jsValue());
    myObjects.add(momo::jsValue());
    myObjects.getRef(1).add("name",momo::jsValue("objB"));
    myObjects.getRef(2).add("name",momo::jsValue("objC"));
    myObjects.getRef(0).add("name",momo::jsValue("objA, not:" + myObjects.getRef(1).getRef("name").getString()));
    momo::TestTools::report(myObjects.getRef(0).getRef("name").getString(), "objA, not:objB", test_);
    numTests++;
    

    // ???
    MYLOG(DEBUG, "EXIT");
  } catch (std::string s) {
    std::cout << "Exception thrown in testArray: " << s << std::endl;
  }

  return numTests;
}

int TestJsValue::testObject(){

  momo::jsValue intVal(17),
          stringVal("abcd"),
          dblVal(1.17E+003, FULLPRECISION),
          myVector( std::vector<momo::jsValue>({intVal,stringVal})),
          myObject;

  try{
    std::stringstream actual_;
    std::string test_ = "testObject",
                expected_ = "";
    // -0- 
    MYLOG(DEBUG, "entering");
    myVector.add(dblVal);
    momo::jsValue arrayVal((myVector));
    myObject.add("myInt", intVal);
    myObject.add("myString", stringVal);
    myObject.add("myDouble", dblVal);
    myObject.add("myVector", arrayVal);


    // -1-   test  
    actual_.str("");
    expected_ = "{\"myDouble\":1.1700000000000000e+03, \"myInt\":17, \"myString\":\"abcd\", \"myVector\":[17, \"abcd\", 1.1700000000000000e+03]}";
    actual_ <<  myObject ;
    momo::TestTools::report(actual_.str(), expected_, test_);


    // ???
    MYLOG(DEBUG, "EXIT");
  } catch (std::string s) {
    std::cout << "Exception thrown in testArray: " << s << std::endl;
  }

  return 2;
}

int TestJsValue::testConstructors(){

  std::string test, testString, testJSON;

  // -1- construct from int
  try {
    int a=17, b=-3, c=0;
    momo::jsValue x(a), *y = new momo::jsValue(b), z = momo::jsValue(c);

    if (x.getInt() == a && y->getInt()==b && z.getInt() == c) {
       momo::TestTools::report("ok", "ok" , "jsValue(int)");
    } else {
       momo::TestTools::report("Error:", "" , "jsValue(int)");
    }
  } catch (std::string e) {
    momo::TestTools::report(false , "Exception in jsValue(int): " + e);
  }

  // -2- construct from string
  try {

    //  constructor should strings (without outer double quotes)
    //  getString should give original string, stringify() the corresponsing JSON
    test = "jsValue::jsValue(string) ";
    testString = "ab@cd";
    testJSON = "\"" + testString + "\"";
    momo::TestTools::report(momo::jsValue(testString).getString(), testString, test);
    momo::TestTools::report(momo::jsValue(testString).stringify(), testJSON, test + "+ stringify ");

    testString = "ab\\ncd";
    testJSON = "\"" + testString + "\"";
    momo::TestTools::report(momo::jsValue(testString).getString(), testString, test);
    momo::TestTools::report(momo::jsValue(testString).stringify(), testJSON, test + "+ stringify() ");

    testString = "ab\\\"cd";
    testJSON = "\"" + testString + "\"";
    momo::TestTools::report(momo::jsValue(testString).getString(), testString, test);
    momo::TestTools::report(momo::jsValue(testString).stringify(), testJSON, test + "+ stringify() ");

    testString = "ab\ncd";
    testJSON = "\"" + testString + "\"";
    momo::TestTools::report(momo::jsValue(testString).getString(), testString, test);
    momo::TestTools::report(momo::jsValue(testString).stringify(), testJSON, test + "+ stringify() ");

    // string with unescaped " should throw
    try {
      test = "string with unescaped double quote";
      testString = "ab\"cd";
      momo::TestTools::report(momo::jsValue(testString).getString(), "not recognized", test);
    } catch (std::string e) {
      momo::TestTools::report(e, "from jsValue(string) constructor: string contains unescaped \" at pos:2", test); 
    }

    // three different ways to use constructor:
    std::string a="aa", b="C\\\"", c="";
    momo::jsValue x(a), *y = new momo::jsValue(b), z = momo::jsValue(c);
    if (x.getString() == a && y->getString()==b && z.getString() == c) {
       momo::TestTools::report("ok", "ok" , "jsValue(string)");
    } else {
       momo::TestTools::report("Error:", "" , "jsValue(string)");
    }

  } catch (std::string e) {
    momo::TestTools::report(false , "Exception in jsValue(string): " + e);
  }


  // -3- construct from double
  try {
    double a=0.4, b=-1.789E-99, c=0;
    momo::jsValue x(a), *y = new momo::jsValue(b), z = momo::jsValue(c);

    if (x.getDbl() == a && y->getDbl()==b && z.getDbl() == c) {
       momo::TestTools::report("ok", "ok" , "jsValue(double)");
    } else {
       momo::TestTools::report("Error:", "" , "jsValue(double)");
    }
  } catch (std::string e) {
    momo::TestTools::report(false , "Exception in jsValue(double): " + e);
  }

  // -4- construct from std::vector<double>
  try {
    double a=0.4, b=-1.789E-19, c=0;
    std::vector<double> myVector = {a,b,c};
    momo::jsValue x(myVector), *y = new momo::jsValue(myVector), z = momo::jsValue(myVector);

    if (x.getDbl(0)==a && y->getDbl(1)==b && z.getDbl(2)==c) {
       momo::TestTools::report("ok", "ok" , "jsValue(std::vector<double>)");
    } else {
       momo::TestTools::report("Error:", "" , "jsValue(std::vector<double>)");
    }
  } catch (std::string e) {
    momo::TestTools::report(false , "Exception in jsValue(std::vector<double>): " + e);
  }

  // -5- construct from std::vector<string>
  try {
    std::string a="aa", b="C\\\"", c="";
    std::vector<std::string> myVector = {a,b,c};
    momo::jsValue x(myVector), *y = new momo::jsValue(myVector), z = momo::jsValue(myVector);

    if (x.getString(0)==a && y->getString(1)==b && z.getString(2)==c) {
       momo::TestTools::report("ok", "ok" , "jsValue(std::vector<double>)");
    } else {
       momo::TestTools::report("Error:", "" , "jsValue(std::vector<double>)");
    }
  } catch (std::string e) {
    momo::TestTools::report(false , "Exception in jsValue(std::vector<double>): " + e);
  }

  // -6- construct from std::vector<jsValue>
  try {
    double a=0.4, b=-1.789E-19, c=0;
    std::vector<momo::jsValue> myVector = {momo::jsValue(a),momo::jsValue(b),momo::jsValue(c)};
    momo::jsValue x(myVector), *y = new momo::jsValue(myVector), z = momo::jsValue(myVector);

    if (x.getDbl(0)==a && y->getDbl(1)==b && z.getDbl(2)==c) {
       momo::TestTools::report("ok", "ok" , "jsValue(std::vector<double>)");
    } else {
       momo::TestTools::report("Error:", "" , "jsValue(std::vector<double>)");
    }
  } catch (std::string e) {
    momo::TestTools::report(false, "Exception in jsValue(std::vector<double>): " + e);
  }

  // -7- default constructon as OBJECT
  try {
    momo::jsValue x, *y = new momo::jsValue(), z;

    double a= -1.789E-19;
    x.add("x",momo::jsValue(a));
    y->add("x",momo::jsValue(a));
    z = x;

    // TODO: implement == for jsValue and jsObject
    if (x.getDbl("x") == a) {
       momo::TestTools::report("ok", "ok" , "jsValue default constructor");
    } else {
       momo::TestTools::report("Error:", "" , "jsValue default constructor");
    }

    if (y->getDbl("x")==a) {
       momo::TestTools::report("ok", "ok" , "jsValue default constructor");
    } else {
       momo::TestTools::report("Error:", "" , "jsValue default constructor");
    }

    if (z.getDbl("x")==a) {
       momo::TestTools::report("ok", "ok" , "jsValue default constructor");
    } else {
       momo::TestTools::report("Error:", "" , "jsValue default constructor");
    }
  } catch (std::string e) {
    momo::TestTools::report(false, "Exception in jsValue default constructor: " + e);
  }

  return 15; 
}
