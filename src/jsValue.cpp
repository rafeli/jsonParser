#include "jsValue.hpp"

namespace momo {

// casts char* Exception into std::string
void throw_(std::string s) { throw s;}

std::ostream& operator<<(std::ostream& os, const jsValue& x) {
   std::vector<jsValue> v;

   // -1- stream depd. on type (recursive for array and object)
   switch (x.getType()) {
   case T_INT:
     os << x.getInt();
     break;
   case T_DOUBLE:
     os.precision(x.getPrecision());
     os << std::scientific << x.getDbl();
     break;
   case T_STRING:
     os << "\"" << x.getEncodedString() << "\"";
     break;
   case T_ARRAY:
     v = x.getArray();
     os << "[";
     if (v.size()>0) os << v[0];
     for (unsigned int i=1; i<v.size(); i++) {
       os << ", " << v[i] ;
     }
     os << "]";
     break;
   case T_OBJECT:
     os << x.getObject();
     break;
   default:
     throw_("streaming unimplemented value type");
   }

   // -2- return
   return os;
 }

/**
* @brief produce the JSON-string representing this jsValue.
* the exact same result is achieved with the <<-operator
* @code
*   jsValue x1 = getJSONValue("\"abc\\\"def\"");
*           x2 = jsValue("abc\"def");  // same thing: x1==x2
*   std::cout << x1;                   // produces: "abc\"def"
*   std::cout << x1.stringify();       // produces: "abc\"def"
*   std::cout << x2;                   // produces: "abc\"def"
*   std::cout << x2.getString();       // produces: abc"def
* @endcode
*
* @return std::string
*/
std::string jsValue::stringify() const{
  std::stringstream ss;
  ss.str("");
  ss << (*this); 
  return ss.str();
}

void jsValue::init() {

  type = T_UNDEFINED;
  intVal = 0;
  arrayVal.clear();;
  stringVal = "";
}

/**
* @brief default constructor: constructs jsValue with type 'OBJECT'
*/
jsValue::jsValue() {
  init();
  type = T_OBJECT;
}

/**
* @brief constructs jsValue with type 'INT'
*
* @param v: the integer value that is to be represented
*/
jsValue::jsValue(const long &v) {
  init();
  type = T_INT;
  intVal = v;
}

/**
* @brief constructs jsValue with type 'INT'
*
* @param v: the integer value that is to be represented
*/
jsValue::jsValue(const int &v) {
  init();
  type = T_INT;
  intVal = (long) v;
}

/**
* @brief constructs jsValue with type 'DOUBLE'
*
* @param v: the value that is to be represented
* @param precision_: number of decimal places, default LOWPRECISION=2
*                    (FULLPRECISION = 16)
*/
jsValue::jsValue(const double &v, int precision_) {
  init();
  type = T_DOUBLE;
  precision = precision_;
  dblVal = v;
}

/**
* @brief constructs jsValue with type 'STRING'
*
* @param s: the string that is to be represented
* @param encoded: for internal use in parser, should be left to default=false (even
* though each string is stored encdoded)
*/
jsValue::jsValue(const std::string &s, bool encoded) {
   init();
   type = T_STRING;
   stringVal = s;

  // decode value if it was encoded, this is currently (20171016)
  // only if jsValue is called from json-parser. 
  if (encoded) {
    std::size_t pos;

    pos =0;
    while ((pos=stringVal.find("\\\"",pos)) != std::string::npos) {
      stringVal = stringVal.replace(pos,2,"\"");
      pos += 1;
    }
// 20171016
//    pos =0;
//    while ((pos=stringVal.find("\\n",pos)) != std::string::npos) {
//      stringVal = stringVal.replace(pos,2,"\n");
//      pos += 1;
//    }
//    pos =0;
//    while ((pos=stringVal.find("\\t",pos)) != std::string::npos) {
//      stringVal = stringVal.replace(pos,2,"\t");
//      pos += 1;
//    }
  }

// 20171016
stringVal = b64toa(stringVal);
// end 20171016

}
 
/**
* @brief constructs jsValue with type ARRAY
*
* @param v the vector of jsValues that is represented
*/
jsValue::jsValue(const std::vector<jsValue> &v) {
  init();
  type = T_ARRAY;
  arrayVal = v;
}


/**
* @brief constructs jsValue of ARRAY type directly from vector<String>
*
* @param v
*/
jsValue::jsValue(const std::vector<std::string> &v) {
  init();
  type = T_ARRAY;
  arrayVal.clear();
  for (unsigned int i=0; i<v.size(); i++) {
//    jsValue e(v[i]);
    arrayVal.push_back(jsValue(v[i]));
  }
}

/**
* @brief constructs jsValue of ARRAY type directly from vector<double>
*
* @param v: vector<double> that is represented
* @param precision_: number of decimal places on stringifying the doubles
*/
jsValue::jsValue(const std::vector<double> &v, int precision_) {
  init();
  type = T_ARRAY;
  arrayVal.clear();
  for (unsigned int i=0; i<v.size(); i++) {
//    jsValue e(v[i]);
    arrayVal.push_back(jsValue(v[i], precision_));
  }
}

/**
* @brief constructs jsValue of ARRAY type from vector<long double>
*
* @param v
* @param precision_
*/
jsValue::jsValue(const std::vector<long double> &v, int precision_) {
  init();
  type = T_ARRAY;
  arrayVal.clear();
  for (unsigned int i=0; i<v.size(); i++) {
//    jsValue e(v[i]);
    arrayVal.push_back(jsValue(v[i], precision_));
  }
}

 
/**
* @brief constructs jsValue of ARRAY type from vector<String>
*
* @param v
*/
jsValue::jsValue(const std::vector<long> &v) {
  init();
  type = T_ARRAY;
  arrayVal.clear();
  for (unsigned int i=0; i<v.size(); i++) {
    arrayVal.push_back(jsValue(v[i]));
  }
}



/**
* @brief return an int representing the type of this jsValue, currently:
*  #define T_UNDEFINED -1
*  #define T_INT 0
*  #define T_DOUBLE 1
*  #define T_STRING 2
*  #define T_ARRAY 3
*  #define T_OBJECT 4
*  TODO: define these as enum
* @return 
*/
int jsValue::getType() const {
  return type;
}

/**
* @brief check this is an INTEGER-type jsValue and return its value
*
* recommended alternatives: object.getInt(key) and/or array.getInt(index)
*
* @return represented value as int
*/
int jsValue::getInt() const {
  if (type != T_INT) throw_( "requesting int from non-int jsonValue");
  return (intVal);
}

/**
* @brief check this is a DOUBLE-type jsValue and return its value
*
* recommended alternatives: object.getDbl(key) and/or array.getDbl(index)
*
* @return represented value as double 
*/
double jsValue::getDbl() const {
  if (type != T_DOUBLE && type != T_INT) throw_("requesting Double from non-dbl jsonValue: " + stringVal);
  if (type == T_DOUBLE) {
    return (dblVal);
  } else {
    return (1.0*intVal);
  }
}

int jsValue::getPrecision() const {
  if (type != T_DOUBLE) throw_("requesting precision from non-dbl jsonValue: " + stringVal);
  return precision;
}

std::string  jsValue::getString() const {
  if (type != T_STRING) throw_("requesting string from non-string jsonValue");
  return atob64(stringVal);
}

std::string jsValue::getEncodedString() const {

  // the counterpart to this encoding is in scanning {doubleQuotedString}
  // as implemented in scanner.l 

  std::string s = atob64(stringVal);
  std::size_t pos;

  if (type != T_STRING) throw_("requesting string from non-string jsonValue");
 
  pos =0;
  while ((pos=s.find("\\",pos+1)) != std::string::npos) {
    s = s.replace(pos,1,"\\\\");
    pos += 2;
  }
  pos =0;
  while ((pos=s.find("\"",pos+1)) != std::string::npos) {
    s = s.replace(pos,1,"\\\"");
    pos += 2;
  }
//  pos =0;
//  while ((pos=s.find("\n",pos+1)) != std::string::npos) {
//    s = s.replace(pos,1,"\\n");
//    pos += 2;
//  }
  
  return  s ;

}

std::vector<jsValue>  jsValue::getArray() const {
  if (type != T_ARRAY) throw_( "requesting array from non-array jsonValue");
  return arrayVal;
}

std::vector<jsValue>&  jsValue::getArrayRef() {
  if (type != T_ARRAY) throw_( "requesting array from non-array jsonValue");
  return arrayVal;
}

jsValue&  jsValue::getRef(std::size_t i) {
  if (type != T_ARRAY) throw_( "requesting array element from non-array jsonValue");
  if (i >= arrayVal.size()) throw_( "array index error");
  return arrayVal[i];
}

jsValue&  jsValue::getRef(std::string key) {
  if (type != T_OBJECT) throw_( "requesting property from non-object jsonValue");
  return objectVal.getRef(key);
}

jsValue jsValue::getObject(std::size_t i) const {
  if (type != T_ARRAY) throw_( "requesting array element from non-array jsonValue");
  if (i >= arrayVal.size()) throw_( "array index error");
  if (arrayVal[i].type != T_OBJECT) throw_("from getObject(i): jsValue at requested index is not an object");
  return arrayVal[i];
}

jsValue jsValue::getObject(std::string key) const {
  if (type != T_OBJECT) throw_( "requesting property from non-object jsonValue");
  if (objectVal.get(key).type != T_OBJECT) throw_("from getObject(i): jsValue at requested index is not an object");
  return objectVal.get(key);
}

jsValue jsValue::getArray(std::size_t i) const {
  if (type != T_ARRAY) throw_( "requesting array element from non-array jsonValue");
  if (i >= arrayVal.size()) throw_( "array index error");
  if (arrayVal[i].type != T_ARRAY) throw_("from getObject(i): jsValue at requested index is not an object");
  return arrayVal[i];
}

jsValue jsValue::getArray(std::string key) const {
  if (type != T_OBJECT) throw_( "requesting property from non-object jsonValue");
  if (objectVal.get(key).type != T_ARRAY) throw_("from getObject(i): jsValue at requested index is not an object");
  return objectVal.get(key);
}

bool jsValue::has(std::string key) const {
  if (type != T_OBJECT) throw_( "testing property from non-object jsonValue");
  return objectVal.has(key);
}

std::string jsValue::getString(std::string key) const {
  if (type != T_OBJECT) throw_( "getting string-property from non-object jsonValue");
  return objectVal.get(key).getString();
}

int jsValue::getInt(std::string key) const {
  if (type != T_OBJECT) throw_( "getting int-property from non-object jsonValue");
  return objectVal.get(key).getInt();
}

double jsValue::getDbl(std::string key) const {
  if (type != T_OBJECT) throw_( "getting double-property from non-object jsonValue");
  return objectVal.get(key).getDbl();
}

std::string jsValue::getString(std::size_t index) const {
  if (type != T_ARRAY) throw_( "indexing string-property from non-array jsonValue");
  return arrayVal[index].getString();
}

int jsValue::getInt(std::size_t index) const {
  if (type != T_ARRAY) throw_( "indexing int-property from non-array jsonValue");
  return arrayVal[index].getInt();
}

double jsValue::getDbl(std::size_t index) const {
  if (type != T_ARRAY) throw_( "indexing double-property from non-array jsonValue");
  return arrayVal[index].getDbl();
}

std::size_t jsValue::size() const {
  if (type != T_ARRAY) throw_( "requesting size from non-Array jsonValue");
  return arrayVal.size();
}

std::vector<double>  jsValue::getDblArray(std::size_t index) const {
  if (type != T_ARRAY) throw_( "indexing doubleArray-property from non-array jsonValue");
  return arrayVal[index].getDblArray();
}

std::vector<double>  jsValue::getDblArray(std::string key) const {
  if (type != T_OBJECT) throw_( "requesting doubleArray-property from non-object jsonValue");
  return objectVal.get(key).getDblArray();
}

std::vector<double>  jsValue::getDblArray() const {

  try {
    std::vector<jsValue> v = getArray();
    std::vector<double> x;
    for (unsigned int i=0; i<v.size(); i++) {
       x.push_back(v[i].getDbl()) ;
     }
    return x;
  } catch (std::string s) {
    throw "Constructing double array: " + s;
  }
}



jsObject jsValue::getObject() const {
  if (type != T_OBJECT) throw_("requesting object from non-object jsonValue");
  return objectVal;
}

void jsValue::add(jsValue x) {
  if (type != T_ARRAY) throw_("adding element to non-array jsonValue");
  arrayVal.push_back(x);
}

void jsValue::set(std::size_t i, jsValue value_) {
  if (type != T_ARRAY) throw_( "requesting array element from non-array jsonValue");
  if (i >= arrayVal.size()) throw_( "array index error");
  arrayVal[i] = value_;
}

void jsValue::add(std::string key_, jsValue value_) {
  if (type != T_OBJECT) throw_("adding keyValuePair to non-object jsonValue");
  objectVal.add(key_, value_);
}

void jsValue::set(std::string key_, jsValue value_) {
  if (type != T_OBJECT) throw_("setting keyValuePair in non-object jsonValue");
  objectVal.set(key_, value_);
}

std::string jsValue::toXML(const std::string& tagName, int indent) const {

  std::stringstream s;
  std::vector<jsValue> myArray;
  std::map<std::string, jsValue> myValues;
  std::vector<std::string> keys;


  // -0- calculate indent
  std::string lStartT = "\n" + std::string(indent,' ') + "<",
              lCloseT = "\n" + std::string(indent,' ') + "</";

  // -1- 
  switch (getType()) {
  case T_OBJECT:
    s << lStartT << tagName << ">" ;
    getObject().getKeys(keys);
    for (std::size_t i=0; i<keys.size(); i++) {
      s << getObject().getRef(keys[i]).toXML(keys[i], indent + 2);
    }
    s << lCloseT << tagName << ">"; 
  break;
  case T_ARRAY:
    myArray = getArray();
    for (std::size_t i=0; i<myArray.size(); i++) {
       s << myArray[i].toXML(tagName, indent );  
    }
  break;
  case T_STRING:
    s << lStartT << tagName << ">" ;
    s << getString();
    if (getString().find("\n") == std::string::npos) {
      s << "</" << tagName << ">"; 
    } else {
      s << lCloseT << tagName << ">"; 
    } 
  break;
  default:
    s << lStartT << tagName << ">" ;
    s << (*this);
    s << "</" << tagName << ">"; 
  }


  // -4- return
  return s.str();

}

} // end namespace momo
