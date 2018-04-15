#include "jsValue.hpp"

namespace momo {

// casts char* Exception into std::string
void throw_(std::string s) { throw s;}

std::ostream& operator<<(std::ostream& os, const jsValue& x) {

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
     os << "\"" << x.getString() << "\"";
     break;
   case T_ARRAY:
     os << "[";
     if (x.arrayVal.size()>0) os << x.arrayVal[0];
     for (unsigned int i=1; i<x.arrayVal.size(); i++) {
       os << ", " << x.arrayVal[i] ;
     }
     os << "]";
     break;
   case T_OBJECT:
//     os << x.getObject();
     os << "{" ;
     for (auto it=x.objectVal.begin(); it!=x.objectVal.end(); ++it) {
      os << (it==x.objectVal.begin() ? "" : ", ") <<  "\"" << it->first <<  "\":" << it->second;
     }
     os << "}";
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
* *without* enclosing double-quotes, e.g. @code jsValue jsName("myName"); @endcode. Note that the
* equivalent call to JSON.parse would require double-quotes, e.g. @code jsValue jsName = JSON.parse("\"myName\"");
* @endcode
* TODO: check and prevent that a string contains a non-escaped " as in jsValue("abc\"def") which is a 7-character
*       string and invalid, wher the 8-character jsValue("abc\\\"def") would be valid
*/
jsValue::jsValue(const std::string &s) {
   init();
   type = T_STRING;
   stringVal = s;

  std::size_t pos=0;
  while ((pos=s.find("\"",pos)) != std::string::npos) {
    if (pos==0 || s[pos-1] != '\\') {
      throw std::string("from jsValue(string) constructor: string contains unescaped \" at pos:")
      + std::to_string(pos);
    }
    pos++;
  }
   


// 20171016
stringVal = momo::tools::b64toa(stringVal);
// end 20171016

}
 
/**
* @brief constructs jsValue with type ARRAY
*        with values copied from the provided argument
*
* @param v the vector of jsValues that is copied into jsValue
*/
jsValue::jsValue(const std::vector<jsValue> &v) {
  init();
  type = T_ARRAY;
  arrayVal = v;
}


/**
* @brief constructs jsValue of ARRAY type directly from vector<String>
*        with values copied from the provided argument
*
* @param v the vector of strings that is copied into jsValue
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
*        with values copied from the provided argument
*
* @param v the vector of strings that is copied into jsValue
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
  return momo::tools::atob64(stringVal);
}



// commented out april 2018: doesnt seem consistent to provide a copy
// of the implementation. If anything, this should be called jsValue_to_stdVector 
//
//std::vector<jsValue>  jsValue::getArray() const {
//  if (type != T_ARRAY) throw_( "requesting array from non-array jsonValue");
//  return arrayVal;
//}
//
//std::vector<jsValue>&  jsValue::getArrayRef() {
//  if (type != T_ARRAY) throw_( "requesting array from non-array jsonValue");
//  return arrayVal;
//}


/**
* @brief provide a non-const ref to the i-th element of an Array-type jsValue
*
* @param i: the requested index
* @throw if this is not an array or if index too high
*
* @return a reference to the jsValue at index=i 
*/
jsValue&  jsValue::getRef(std::size_t i) {
  if (type != T_ARRAY) throw_( "requesting array element from non-array jsonValue");
  if (i >= arrayVal.size()) throw_( "array index error");
  return arrayVal[i];
}

/**
* @brief provide a non-const ref to an object element
*
* @param key: the key for this element
*
* @return a reference to the jsValue corresponding to this key
*/
jsValue&  jsValue::getRef(std::string key) {
  if (type != T_OBJECT) throw_( "requesting property from non-object jsonValue");
  if (objectVal.count(key) == 0) throw "requesting unknown field from jsObject: " + key ;
  return objectVal[key];
}

/**
* @brief provide a const (TODO: why?)  copy of the i-th element of an Array-type jsValue
*
* @param i: the requested index
* @throw if this is not an array or if index too high
*
* @return a copy of the jsValue at index=i
*/
const jsValue jsValue::get(std::size_t i) const {
  if (type != T_ARRAY) throw_( "requesting array element from non-array jsonValue");
  if (i >= arrayVal.size()) throw_( "array index error");
  return arrayVal[i];
}

/**
* @brief provide a const (TODO: why?)  copy of an object element
*
* @param key: the key for which the element is requested
*
* @return a copy of the jsValue corresonding to this key 
*/
const jsValue jsValue::get(const std::string& key) const {

  // checks
  if (type != T_OBJECT) throw_( "requesting property from non-object jsonValue");
  if (objectVal.count(key) == 0) throw "requesting unknown field from jsObject: " + key ;

  // return:  std::map.at(key) has const and non-const variants and will here choose const
  // the []-operator is always non-const and cannot be used here
  return objectVal.at(key); 

}

/**
* @brief provide a copy of the i-th element of an Array-type jsValue
*        which must be of object-type
*
* @param i
*
* @return 
*/
jsValue jsValue::getObject(std::size_t i) const {
  jsValue v = get(i);
  if (v.type != T_OBJECT) throw_("from getObject(i): jsValue at requested index is not an object");
  return v;
}

/**
* @brief provide a const (TODO: why?)  copy of an object element
*        which must be of object-type itself
*
* @param key
*
* @return 
*/
jsValue jsValue::getObject(std::string key) const {
  jsValue v = get(key);
  if (v.type != T_OBJECT) throw_("from getObject(i): jsValue at requested index is not an object");
  return v;
}

jsValue jsValue::getArray(std::size_t i) const {
  jsValue v = get(i);
  if (v.type != T_ARRAY) throw_("from getObject(i): jsValue at requested index is not an object");
  return v;
}

jsValue jsValue::getArray(std::string key) const {
  if (type != T_OBJECT) throw_( "requesting property from non-object jsonValue");
  if (get(key).type != T_ARRAY) throw_("from getArray(i): jsValue at requested index is not an array");
  return get(key);
}

bool jsValue::has(std::string key) const {
  if (type != T_OBJECT) throw_( "testing property from non-object jsonValue");
  return objectVal.count(key) > 0;
}


std::size_t jsValue::size() const {
  if (type != T_ARRAY) throw_( "requesting size from non-Array jsonValue");
  return arrayVal.size();
}

std::vector<double>  jsValue::getDblArray() const {

  if (type != T_ARRAY) throw_( "requesting double array from non-Array jsonValue");
  unsigned int i;
  try {
    std::vector<double> x;
    for (i=0; i<arrayVal.size(); i++) {
       x.push_back(arrayVal[i].getDbl()) ;
     }
    return x;
  } catch (std::string s) {
    throw "jsValue::getDblArray() : " + s + "\nat index: " + std::to_string(i);
  }
}



//jsObject jsValue::getObject() const {
//  if (type != T_OBJECT) throw_("requesting object from non-object jsonValue");
//  return objectVal;
//}

void jsValue::add(jsValue x) {
  if (type != T_ARRAY) throw_("adding element to non-array jsonValue");
  arrayVal.push_back(x);
}

void jsValue::set(std::size_t i, jsValue value_) {
  if (type != T_ARRAY) throw_( "requesting array element from non-array jsonValue");
  if (i >= arrayVal.size()) throw_( "array index error");
  arrayVal[i] = value_;
}

void jsValue::add(std::string key, jsValue val) {
  if (type != T_OBJECT) throw_("adding keyValuePair to non-object jsonValue");
  if (has(key)) throw std::string("adding key-value for existing key " + key + ", you may want to use set?");
  objectVal.insert(std::pair<std::string, jsValue>(key,val));
//  objectVal.add(key_, value_);
}

void jsValue::set(std::string key, jsValue val) {
  if (type != T_OBJECT) throw_("setting keyValuePair in non-object jsonValue");
  if (has(key)) {
    objectVal[key] = val;
  } else {
    objectVal.insert(std::pair<std::string, jsValue>(key,val));
  }
//  objectVal.set(key_, value_);
}

void jsValue::getKeys(std::vector<std::string>& keys) const {

  if (type != T_OBJECT) throw_("requesting keys for non-object jsonValue");
  for (auto it=objectVal.begin(); it != objectVal.end(); it++) {
      keys.push_back(it->first);
  }
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
    getKeys(keys);
    for (std::size_t i=0; i<keys.size(); i++) {
      s << get(keys[i]).toXML(keys[i], indent + 2);
    }
    s << lCloseT << tagName << ">"; 
  break;
  case T_ARRAY:
    for (std::size_t i=0; i<arrayVal.size(); i++) {
       s << arrayVal[i].toXML(tagName, indent );  
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
