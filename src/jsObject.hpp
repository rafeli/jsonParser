#ifndef _JSOBJ_HPP
#define _JSOBJ_HPP

#include <map>
#include <string>

class jsValue;

class jsObject {

  public:

  std::map<std::string, jsValue> myValues; // dies soll eher private sein ??

  jsObject();

  ~jsObject();

  void add(std::string, jsValue);

  jsValue get(std::string);

  bool has(std::string);

};

std::ostream& operator<<(std::ostream& os, const jsObject& x);

#endif
