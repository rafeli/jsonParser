#ifndef _JSVALUE_HPP
#define _JSVALUE_HPP

#include <string>
#include <vector>
#include <ostream>
#include <sstream>

#include "jsObject.hpp"
#include "momo/binaryCoding.hpp"

#define T_UNDEFINED -1
#define T_INT 0
#define T_DOUBLE 1
#define T_STRING 2
#define T_ARRAY 3
#define T_OBJECT 4

#define FULLPRECISION 16
#define LOWPRECISION 2


class jsValue {

  private:

  int type, precision;

  long intVal; 

  double dblVal; 

  std::string stringVal; // TODO: use string && stringVal and std::move()

  std::vector<jsValue> arrayVal;

  jsObject objectVal;

  void init();

  public:

  jsValue(); // default constructor gives an object-type value

  jsValue(const long &);
  jsValue(const int &);
  jsValue(const double &, int precision = LOWPRECISION);
  jsValue(const std::string &, bool encoded = false); // TODO: encoded=true no longer required ?!!
  jsValue(const std::vector<jsValue> &);
// the following allowed e.g. jsValue x = y; with y a std::vector<jsValue>
// without we now need: jsValule x = jsValue(y); which is clearer to me
//  jsValue(std::vector<jsValue> &&);


  jsValue(const jsObject &);

  // TODO: implement Compare (?), < , > 
  bool operator == (const jsValue& d) const {
    return (stringify() == d.stringify());
  }

  // following  not needed for parsing, added 2016 to allow stringify
  jsValue(const std::vector<std::string> &);
  jsValue(const std::vector<double> &, int precision = LOWPRECISION);
  jsValue(const std::vector<long double> &, int precision = LOWPRECISION);
  jsValue(const std::vector<long> &);
  std::string stringify() const;

  void add(jsValue); // add to array

  void add(std::string, jsValue); // add key/value to object
  void set(std::string, jsValue); // set key/value in object, add if non-existent
  void set(std::size_t, jsValue); // set one value of a jsValue representing an array

  int getType() const;

  int getInt() const;

  double getDbl() const;
  int getPrecision() const;

  std::vector<jsValue> getArray() const;

  std::vector<jsValue>& getArrayRef() ;

  jsObject getObject() const;

  std::string getString() const;

  std::string getEncodedString() const;

  std::string toXML(const std::string& tagName, int indent=0 ) const;

  // NEU 2017: soll getArray, getObject, getArrayRef, .. ersetzen ?
  // methoden die nur angewendet werden koennen/duerfen, wenn jsValue ein Objekt
  jsValue& getRef(std::size_t index); 
  jsValue& getRef(std::string key);
  bool has(std::string);
  std::size_t size() const;
  std::string getString(std::string key) const;
  int getInt(std::string key) const;
  double getDbl(std::string key) const;
  std::string getString(std::size_t index) const;
  int getInt(std::size_t index) const;
  double getDbl(std::size_t index) const;



  // convenience:
  std::vector<double> getDblArray() const;


};

std::ostream& operator<<(std::ostream& os, const jsValue& x);

#endif
