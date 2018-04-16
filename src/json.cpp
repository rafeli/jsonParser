#include "json.hpp"

#include <ctype.h>

namespace momo {

jsValue json::parseExpectJSON(const std::string& s, std::size_t& pos) {

  // accept any number of spaces before valid json
  while (s[pos]==' ') pos++;
  
  // now parse either an object, array or primitive value
  switch (s[pos]) {

  case '[': return parseExpectJSArray(s,pos); break;

  case '{': return parseExpectJSObject(s,pos); break;

  case '\"': // fallthrough intended
  case '\'': return parseExpectString(s,pos); break;

  case 'T':
  case 't':
  case 'F':
  case 'f': return parseExpectBoolean(s,pos); break;

  default: return parseExpectNumber(s,pos); break;
  }
}

jsValue json::parseExpectJSArray(const std::string& s, std::size_t& pos) {

  std::vector<jsValue> myVector;
  std::size_t posL = pos;

  try {

    // parse opening [, no test needed
    pos++;

    // parse first element if available
    if (s[pos] != ']') myVector.push_back(parseExpectJSON(s,pos));

    // parse sequence of JSON content separated by ',' up to ']'
    while (s[pos] != ']') {
      if (s[pos] != ',') throw std::string("expect separating ',' not: ") + s.substr(pos,3);
      pos++;
      myVector.push_back(parseExpectJSON(s,pos));
      if (pos >= s.size()) throw std::string("closing ] fails"); 
    }

    // parse closing "
    pos++;

    // return
    return jsValue(myVector);

  } catch (std::string e) {
    throw "json::parseExpectJSArray: \"" + s.substr(posL,10) + "\" " + e;
  }
}

jsValue json::parseExpectJSObject(const std::string& s, std::size_t& pos) {

  jsValue myObject;
  std::size_t posL = pos;

  try {

    // parse opening {, no test needed
    pos++;

    // parse first key-value-pair KEY:VALUE if available
    if (s[pos] != '}') {
      while (s[pos]==' ') pos++;
      jsValue key = parseExpectString(s,pos);
      while (s[pos]==' ') pos++;
      if (s[pos] != ':') throw std::string("expect separating ':' not: ") + s.substr(pos,3);
      pos++;
      jsValue value = parseExpectJSON(s,pos);
      myObject.add(key.getString(), value);
    }

    // parse sequence of key-value-pairs separated by ',' up to ']'
    while (s[pos] != '}') {

      if (s[pos] != ',') throw std::string("expect separating ',' not: ") + s.substr(pos,3);
      pos++;

      while (s[pos]==' ') pos++;
      jsValue key = parseExpectString(s,pos);
      while (s[pos]==' ') pos++;
      if (s[pos] != ':') throw std::string("expect separating ':' not: ") + s.substr(pos,3);
      pos++;
      jsValue value = parseExpectJSON(s,pos);
      myObject.add(key.getString(), value);

      if (pos >= s.size()) throw std::string("closing } fails"); 

    }

    // parse closing "
    pos++;

    // return
    return myObject;

  } catch (std::string e) {
    throw "json::parseExpectJSObject: \"" + s.substr(posL,10) + "\" " + e;
  }

}

/**
* @brief parse the JSON of the form "..." and produce a string-type
*        jsValue with string=... 
* <br/> The momo::JSON module has no functionality to add or remove escaping
* backslashes, but it checks and forbids unescaped doublequotes in string-values.
* <br/> Note that the remark "a valid JSON-String" can have two completely different
* meanings: (I) the string "{false}" is not valid JSON and (II) the string 'ab"cd'
* may be a valid JS-String but the unescaped double-quote is problematic when
* this string is the string-value in a JSON construct, e.g. as in ["ab"cd"]
* <br/>In case a string contains double-quotes, these double-quotes should be escaped.
* In both C++ and JS code the following "ab\"cd" is an unacceptable UN-escaped quote,
* because the backslash is part of the C++ or JS-Code but not part of the string. Acceptable would 
* be "ab\\\"cd"
*
* @param s
* @param pos
*
* @return 
*/
jsValue json::parseExpectString(const std::string& s, std::size_t& pos) {

  bool escapeNext = false;
  std::size_t posL = pos;

  try {

    // parse opening "
    if (s[pos] != '"') throw std::string("expect opening \", not: ") + s.substr(pos,3);
    pos++;

    // parse sequence of characters up to "
    while (s[pos] != '"' || escapeNext) {
      escapeNext = (s[pos] == '\\');
      pos++;
      if (pos >= s.size()) throw std::string("closing \" fails"); 
    }

    // parse closing "
    pos++;

    // return
    return jsValue(s.substr(posL+1, pos-posL-2));

  } catch (std::string e) {
    throw "Error parsing string from: \"" + s.substr(posL,10) + "\" " + e;
  }

}

jsValue json::parseExpectNumber(const std::string& s, std::size_t& pos) {

  bool isDouble = false, countNumDigits = false;
  std::size_t posL = pos, numDigits=0;

  try {

    // parse sign and space if given
    while (s[pos]==' ') pos++;
    if (s[pos]=='+' || s[pos]=='-') pos++;
  
    // parse digits with optional fraction 
    if (s[pos]<'0' || s[pos] > '9') throw std::string("expecting digit, not: ") + s.substr(pos,3);
    while (s[pos]>='0' && s[pos] <= '9' ) {
      if (s[pos]>'0') countNumDigits = true;
      if (countNumDigits) numDigits++;
      pos++;
    }
    if (s[pos] == '.') {
      isDouble = true;
      pos++;
      if (s[pos]<'0' || s[pos] > '9') throw std::string("expecting digit, not: ") + s.substr(pos,3);
      while (s[pos]>='0' && s[pos] <= '9' ) {
        if (s[pos]>'0') countNumDigits = true;
        if (countNumDigits) numDigits++;
        pos++;
      }
    }
  
    // parse exponent with max 3 digits
    if (s[pos] == 'E' || s[pos] == 'e') {
      isDouble = true;
      pos++;
      if (s[pos]=='+' || s[pos]=='-') pos++;
      if (s[pos]<'0' || s[pos] > '9') throw std::string("expecting digit, not: ") + s.substr(pos,3);
      if (s[pos]>='0' && s[pos] <= '9' ) pos++;
      if (s[pos]>='0' && s[pos] <= '9' ) pos++;
      if (s[pos]>='0' && s[pos] <= '9' ) pos++;
    }

    // accept spaces after number
    while (s[pos]==' ') pos++;
  
    // return double- or int-type jsValue
    // precision is used when stringifying jsValue
    if (isDouble) return jsValue(std::stod(s.substr(posL, pos-posL)), numDigits-1);
    return jsValue(std::stol(s.substr(posL, pos-posL)));
  
  } catch (std::string e) {
    throw "Error parsing number from: \"" + s.substr(posL,10) + "\" " + e;
  }
}

jsValue json::parseExpectBoolean(const std::string& s, std::size_t& pos) {
  throw std::string("not implemented yet");
}

jsValue json::parse(const std::string& s) {

  std::size_t pos=0;
  jsValue v = parseExpectJSON(s,pos);
  if (pos < s.size()) {
    throw std::string("json::parse unexpected: ") + s.substr(pos,10);
  }
  return v;
  
}








}
