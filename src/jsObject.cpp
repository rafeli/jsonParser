#include "jsObject.hpp"
#include "jsValue.hpp"

namespace momo {

std::ostream& operator<<(std::ostream& os, const jsObject& x) {
   std::vector<jsValue> v;

   // -1- stream depd. on type (recursive for array and object)
   os << "{" ;
   for (auto it=x.myValues.begin(); it!=x.myValues.end(); ++it) {
    os << (it==x.myValues.begin() ? "" : ", ") <<  "\"" << it->first <<  "\":" << it->second;
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
  if (has(key)) throw std::string("adding key-value for existing key " + key + ", you may want to use set?");
  myValues.insert(std::pair<std::string, jsValue>(key,val));
}

void jsObject::set(std::string key, jsValue val) {
  if (has(key)) {
    myValues[key] = val;
  } else {
    myValues.insert(std::pair<std::string, jsValue>(key,val));
  }
}

const jsValue jsObject::get(const std::string& key) const {
  if (myValues.count(key) == 0) throw "requesting unknown field from jsObject: " + key ;

  // at has const and non-const variants and will here choose const
  // the []-operator is always non-const and cannot be used here
  return myValues.at(key); 
}

jsValue& jsObject::getRef(const std::string& key) {
  if (myValues.count(key) == 0) throw "requesting unknown field from jsObject: " + key ;
  return myValues[key];
}

bool jsObject::has(std::string key) const {
  return (myValues.count(key) > 0) ;
}

void jsObject::getKeys(std::vector<std::string>& keys) const {

  for (auto it=myValues.begin(); it != myValues.end(); it++) {
      keys.push_back(it->first);
  }
}


} // end namespace momo


