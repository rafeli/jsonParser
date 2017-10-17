#ifndef _JSOBJ_HPP
#define _JSOBJ_HPP

#include <map>
#include <vector>
#include <string>

class jsValue;

class jsObject {

  public:

  std::map<std::string, jsValue> myValues; // dies soll eher private sein ??

  jsObject();

  ~jsObject();

  void add(std::string, jsValue);

  void set(std::string, jsValue);

  const jsValue get(const std::string&) const;

  jsValue& getRef(const std::string&);

  void getKeys(std::vector<std::string>& keys) const;

  bool has(std::string);


};

std::ostream& operator<<(std::ostream& os, const jsObject& x);

#endif
