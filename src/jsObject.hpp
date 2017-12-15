#ifndef _JSOBJ_HPP
#define _JSOBJ_HPP

#include <map>
#include <vector>
#include <string>

namespace momo {

class jsValue;

/**
* @brief represents an object (list of key-value-pairs). 
* Was heavily used in user-code, but I now try to substitue
* all these uses based on jsValue. If possible I'd like to get
* rid of jsObject completely
*/
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

  bool has(std::string) const;


};

std::ostream& operator<<(std::ostream& os, const jsObject& x);

} // end namespace momo

#endif
