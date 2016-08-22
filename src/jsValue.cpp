#include "jsObject.hpp"
#include "jsValue.hpp"

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
     os << std::scientific << x.getDbl();
     break;
   case T_STRING:
     os << "\"" << x.getString() << "\"";
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

std::string jsValue::stringify() {
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

jsValue::jsValue() {
  init();
}

jsValue::jsValue(const int &v) {
  init();
  type = T_INT;
  intVal = v;
}

jsValue::jsValue(const double &v) {
  init();
  type = T_DOUBLE;
  dblVal = v;
}

 jsValue::jsValue(const std::string &s) {
   init();
   type = T_STRING;
   stringVal = s;
 }
 
jsValue::jsValue(std::vector<jsValue> &&v) {
  init();
  type = T_ARRAY;
  arrayVal = v;
}

jsValue::jsValue(jsObject &&x) {
  init();
  type = T_OBJECT;
  objectVal = x;
}

// new Constructors, not needed for parsing but for stringify
// havent managed this with templates (linker doesnt find specific functions)
jsValue::jsValue(std::vector<std::string> &v) {
  init();
  type = T_ARRAY;
  arrayVal.clear();
  for (unsigned int i=0; i<v.size(); i++) {
    jsValue e(v[i]);
    arrayVal.push_back(e);
  }
}

jsValue::jsValue(std::vector<double> &v) {
  init();
  type = T_ARRAY;
  arrayVal.clear();
  for (unsigned int i=0; i<v.size(); i++) {
    jsValue e(v[i]);
    arrayVal.push_back(e);
  }
}
 
jsValue::jsValue(std::vector<int> &v) {
  init();
  type = T_ARRAY;
  arrayVal.clear();
  for (unsigned int i=0; i<v.size(); i++) {
    jsValue e(v[i]);
    arrayVal.push_back(e);
  }
}



int jsValue::getType() const {
  return type;
}

int jsValue::getInt() const {

  if (type != T_INT) throw_( "requesting int from non-int jsonValue");
  return (intVal);
}

double jsValue::getDbl() const {
  if (type != T_DOUBLE && type != T_INT) throw_("requesting Double from non-dbl jsonValue: " + stringVal);
  if (type == T_DOUBLE) {
    return (dblVal);
  } else {
    return (1.0*intVal);
  }
}

std::string  jsValue::getString() const {

  if (type != T_STRING) throw_("requesting string from non-string jsonValue");
  return stringVal;

}

std::vector<jsValue>  jsValue::getArray() const {
  
  if (type != T_ARRAY) throw_( "requesting array from non-array jsonValue");
  return arrayVal;
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

void jsValue::add(std::string key_, jsValue value_) {
  if (type != T_OBJECT) throw_("adding keyValuePair to non-object jsonValue");
  objectVal.add(key_, value_);
}
