#ifndef _JSVALUE_HPP
#define _JSVALUE_HPP

#include <string>
#include <vector>
#include <ostream>
#include <sstream>

#include "jsObject.hpp"

#define T_UNDEFINED -1
#define T_INT 0
#define T_DOUBLE 1
#define T_STRING 2
#define T_ARRAY 3
#define T_OBJECT 4


class jsValue {

  private:

  int type;

  long intVal; 

  double dblVal; 

  std::string stringVal; // TODO: use string && stringVal and std::move()

  std::vector<jsValue> arrayVal;

  jsObject objectVal;

  void init();

  public:

  jsValue(); // TODO: is this needed ???

  jsValue(const long &);
  jsValue(const int &);

  jsValue(const double &);

  jsValue(const std::string &, bool encoded = false);
 
  jsValue(std::vector<jsValue> &&);

  jsValue(jsObject &&);

  // following  not needed for parsing, added 2016 to allow stringify
  jsValue(std::vector<std::string> &);
  jsValue(std::vector<double> &);
  jsValue(std::vector<long> &);
  std::string stringify();

  void add(jsValue); // add to array

  void add(std::string, jsValue); // add to object

  int getType() const;

  int getInt() const;

  double getDbl() const;

  std::vector<jsValue> getArray() const;

  jsObject getObject() const;

  std::string getString() const;

  std::string getEncodedString() const;

  // convenience:
  std::vector<double> getDblArray() const;


};

std::ostream& operator<<(std::ostream& os, const jsValue& x);

#endif
