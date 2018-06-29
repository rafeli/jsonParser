#ifndef _JSVALUE_HPP
#define _JSVALUE_HPP

#include <string>
#include <vector>
#include <ostream>
#include <sstream>

#include "momo/binaryCoding.hpp"

#define T_UNDEFINED -1
#define T_INT 0
#define T_DOUBLE 1
#define T_STRING 2
#define T_BOOL 3
#define T_ARRAY 4
#define T_OBJECT 5
#define T_NULL 6

#define FULLPRECISION 16
#define LOWPRECISION 2

namespace momo {

/**
* @brief Represents a javascript value, which can be an
* object, array, number (double/int) or text (string). Typical use e.g
* in an http-service that receives and responds JSON-strings. The jsValue class has no
* functionality to parse JSON: this is implemented in json.parse() for this.
* 
* @code
* std::string service(std::string requestBody) {
*   try {
*
*     jsValue jsRequest(atob64(requestBody))
*           , response();
*
*     std::string senderName = jsRequest.getString("senderName");
*     response.set("message", jsValue("hello, " + senderName));
*     return response.stringify();
*
*   } catch (std::string errorMsg) {
*     ...
*   }
* }
* @endcode
* Working with json involves three representations of information:
* the C++ variable,  the corresponding jsValue and the C++-string that represents a jsValue. 
* The corresponsing transformations between these representation include:
* <h3> encoding and parsing </h3>
* @code
*     std::string responseString = response.stringify(),
*        xmlString = response.toXML(); // does that work ???
* @endcode
*
* <h3> extracting C++-variables from jsValue </h3>
* This typically means extracting information from a jsValue that represents either
* an object or an array. The information is retrieved with a key (for objects)
* or an index (for arrays). There are two different approaches, either the information
* is retrieved directly as C++-variable:
* @code
*     jsValue myObject("{\"keyA\":17, \"keyB\":\"aText\",\"keyC\":{}}"),
*             myArray("[17.3,\"anotherText\"]"); 
*     int ia = myObject.getInt("keyA")
*         db = myArray.getDouble(0); 
*     String sa = myObject.getString("keyB"),
*            sb = myArray.getString(1); 
*     jsValue jsOa = myObject.getObject("keyC");
* @endcode
* or the information is first extracted as jsValue, from which the C++-variable is read:
* @code
*     ...
*     jsValue& jsa = myObject.getRef("keyA"),
*              jsb = myArray.getRef(0),
*             jssa = myObject.get("keyB")
*     std::string sa = jssa.getString();
*     int a = jsa.getInteger();
*     double b = jsb.getDbl();
* @endcode
* The above statements throw in case a key or index is not availabe or
* the type of the corresponding jsValue doesnt fit, e.g. when a getDouble()
* is applied to a text. Other getters are: getArray, getObject, getBool
*
*
* <h3> constructing jsValue </h3>
* @code
*     // The jsValue()-constructor infers the value-type from 
*     // its argument. The default constructor creates on object-type
*     jsValue response();
*     response.set("someNumber", jsValue(17.1));
*     response.set("someObject", jsValue());
*     std::vector<jsValue> myVector;
*     response.set("someArray", jsValue(myVector);
*
*     // the objects/arrays in the response could have been filled
*     // with data *before* inclusion. Alternatively, we work with a reference
*     jsValue& refSomeObject = response.getRef("someObject");
*     jsValue& refSomeArray = response.getRef("someArray");
*     refSomeObject.set("x", jsValue("y"));
*     refSomeArray.set(2, jsValue("y"));  // ?? 
* @endcode
*
*
*
*   
* }
* @endcode
*/
class jsValue {

  protected:

  int type, precision;

  long intVal; 
  double dblVal; 
  bool boolVal; 
  std::string stringVal; // TODO: use string && stringVal and std::move()
  std::vector<jsValue> arrayVal;
  std::map<std::string, jsValue> objectVal;

  void init();


  public:


  jsValue(); // default constructor gives an object-type value

  jsValue(const bool &);
  jsValue(const long &);
  jsValue(const int &);
  jsValue(const double &, int precision = LOWPRECISION);
  jsValue(const char* );
  jsValue(const std::string &);
  jsValue(const std::vector<jsValue> &);

// the following allowed e.g. jsValue x = y; with y a std::vector<jsValue>
// without we now need: jsValue x = jsValue(y); which is clearer to me
//  jsValue(std::vector<jsValue> &&);



  // TODO: implement Compare (?), < , > 
  bool operator == (const jsValue& d) const {
    return (stringify() == d.stringify());
  }

  // following  not needed for parsing, added 2016 to allow stringify
  jsValue(const std::vector<std::string> &);
  jsValue(const std::vector<double> &, int precision = LOWPRECISION);
  jsValue(const std::vector<long double> &, int precision = LOWPRECISION);
  jsValue(const std::vector<long> &);

  // SONSTIGE
  int getType() const;
  int getPrecision() const;

  // ACCESSING ELEMENTS OF OBJECT/ARRAY-TYPE JSVALUE
  bool has(std::string) const;
  void getKeys(std::vector<std::string>& keys) const;
  std::size_t size() const;
  jsValue& getRef(std::size_t index); 
  jsValue& getRef(std::string key);
  const jsValue get(const std::string& key) const; 
  const jsValue get(std::size_t index) const;
  jsValue getObject(std::size_t index) const;
  jsValue getObject(std::string key) const;
  jsValue getArray(std::size_t index) const;
  jsValue getArray(std::string key) const;

  // READING ELEMENTS OF OBJECT/ARRAY-TYPE JSVALUE INTO C++ primitive types
  std::string getString(std::size_t index) const { return get(index).getString();}
  std::string getString(std::string key) const { return get(key).getString();}
  int getInt(std::size_t index) const { return get(index).getInt();}
  int getInt(std::string key) const { return get(key).getInt();}
  double getDbl(std::size_t index) const { return get(index).getDbl();}
  double getDbl(std::string key) const {return get(key).getDbl();}
  bool getBool(std::size_t index) const { return get(index).getBool();}
  bool getBool(std::string key) const {return get(key).getBool();}
  std::vector<double> getDblArray(std::string key) const {return get(key).getDblArray();};
  std::vector<double> getDblArray(std::size_t index) const {return get(index).getDblArray();};

  // CONVERTING JSVALUE primitive types INTO C++ primitive types
  int getInt() const;
  double getDbl() const;
  bool getBool() const;
  std::string getString() const;
  std::vector<double> getDblArray() const;

  // WRITING C++ INTO JSVALUE
  void add(jsValue);              // add to array
  void add(std::string, jsValue); // add key/value to object
  void set(std::string, jsValue); // set key/value in object, add if non-existent
  void set(std::size_t, jsValue); // set one value of a jsValue representing an array

  // STRINGIFY
  std::string toXML(const std::string& tagName, int indent=0 ) const;
  std::string stringify() const;
  friend std::ostream& operator<<(std::ostream& os, const jsValue& x);

  // TODO: delete / mov? 
  // I try to avoid these, but still use them e.g. in testV8js
  std::vector<jsValue> getArray() const;
//  jsObject getObject() const;
  std::string getEncodedString() const;
  std::vector<jsValue>& getArrayRef() ;

};

std::ostream& operator<<(std::ostream& os, const jsValue& x);

} // end namespace momo

#endif
