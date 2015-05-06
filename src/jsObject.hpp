#ifndef _JSOBJ_HPP
#define _JSOBJ_HPP

#include <map>
#include <string>

class jsValue;

class jsObject {

  public:

  std::map<std::string, jsValue> myValues;

  jsObject();

  ~jsObject();

  void add(std::string, jsValue);
};

std::ostream& operator<<(std::ostream& os, const jsObject& x);

#endif
