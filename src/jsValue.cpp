#include "jsObject.hpp"
#include "jsValue.hpp"

 std::ostream& operator<<(std::ostream& os, const jsValue& x) {
   std::vector<jsValue> v;

   // -1- stream depd. on type (recursive for array and object)
   switch (x.getType()) {
   case T_INT:
     os << x.getInt();
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
   default:
     throw std::string("streaming unimplemented value type");
   }

   // -2- return
   return os;
 }


void jsValue::init() {

  type = T_UNDEFINED;
  intVal = NULL;
  arrayVal.clear();;
  stringVal = "";
}

jsValue::jsValue(const int &v) {
  init();
  type = T_INT;
  intVal = new int;
  (*intVal) = v;
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

int jsValue::getType() const {
  return type;
}

int jsValue::getInt() const {
  if (type == T_INT) {
    return (*intVal);
  } else {
    throw "requesting int from non-int jsonValue";
  }
}

std::string  jsValue::getString() const {
  if (type == T_STRING) {
    return stringVal;
  } else {
    throw "requesting string from non-string jsonValue";
  }
}

std::vector<jsValue>  jsValue::getArray() const {
  if (type == T_ARRAY) {
    return arrayVal;
  } else {
    throw "requesting array from non-array jsonValue";
  }
}
