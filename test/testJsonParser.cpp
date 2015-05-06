#include "testJsonParser.hpp"
#include <stddef.h>


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
}
