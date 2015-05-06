#ifndef _JSVALUE_HPP
#define _JSVALUE_HPP

#include <string>
#include <vector>
#include <ostream>

#define T_UNDEFINED -1
#define T_INT 0
#define T_DOUBLE 1
#define T_STRING 2
#define T_ARRAY 3


class jsValue {

  private:

  int type;

  int *intVal;

  std::string stringVal;

  std::vector<jsValue> arrayVal;

  void init();

  public:

  jsValue(const int &);

  jsValue(const std::string &);
 
  jsValue(std::vector<jsValue> &&);

  int getType() const;

  int getInt() const;

  std::vector<jsValue> getArray() const;

  std::string  getString() const;


};

std::ostream& operator<<(std::ostream& os, const jsValue& x);

#endif
