#ifndef _JSON_HPP
#define _JSON_HPP

#include "jsValue.hpp"

namespace momo {

/**
* @brief parses json (i.e. strings) into momo::jsValue
* Use: @code
*
* #include "momo/json.hpp"
*
* momo::json json;
* momo::jsValue myValue = json.parse("[1,2,3,4]");
* std::cout << myValue.getInt(1); // outputs 2
* @endcode
*/
class json {

  private:

  jsValue parseExpectJSON(const std::string& s, std::size_t& pos);

  jsValue parseExpectJSObject(const std::string& s, std::size_t& pos);

  jsValue parseExpectJSArray(const std::string& s, std::size_t& pos);

  jsValue parseExpectString(const std::string& s, std::size_t& pos);

  jsValue parseExpectNumber(const std::string& s, std::size_t& pos);

  jsValue parseExpectBoolean(const std::string& s, std::size_t& pos);


  public:

  // constructor
  json(){}

  jsValue parse(const std::string& s);

  jsValue parseB64(const std::string& s){
    return parse(momo::tools::atob64(s));
  }

};

}

#endif
