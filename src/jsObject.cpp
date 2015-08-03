#include "jsObject.hpp"
#include "jsValue.hpp"

std::ostream& operator<<(std::ostream& os, const jsObject& x) {
   std::vector<jsValue> v;

   // -1- stream depd. on type (recursive for array and object)
   os << "{" ;
   for (auto it=x.myValues.begin(); it!=x.myValues.end(); ++it) {
    os << (it==x.myValues.begin() ? "" : ", ") << it->first << ":" << it->second;
   }
   os << "}";

   // -2- return
   return os;
}


jsObject::jsObject() {
}

jsObject::~jsObject() {
  
}

void jsObject::add(std::string key, jsValue val) {
  myValues.insert(std::pair<std::string, jsValue>(key,val));
}

jsValue jsObject::get(std::string key) {
  if (myValues.count(key) == 0) throw "requesting unknown field from jsObject: " + key ;
  return myValues[key];
}

bool jsObject::has(std::string key) {
  return (myValues.count(key) > 0) ;
}
