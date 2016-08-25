#ifndef CALCXX_DRIVER_HH
# define CALCXX_DRIVER_HH
# include <string>
# include <sstream>
# include <map>
# include "parser.tab.hh"
// Tell Flex the lexer's prototype ...
# define YY_DECL \
  yy::calcxx_parser::symbol_type yylex (calcxx_driver& driver)
// ... and declare it for the parser's sake.
YY_DECL;
// Conducting the whole scanning and parsing of Calc++.
class calcxx_driver
{
public:
  calcxx_driver ();
  virtual ~calcxx_driver ();

  std::map<std::string, int> variables;

  jsValue result;
  // Handling the scanner.
  void scan_begin (std::string, int);
  bool trace_scanning;
  // Run the parser on file F.
  // Return 0 on success.
  int parse (const std::string& f);

  // Whether parser traces should be generated.
  bool trace_parsing;
  // Error handling.
  void error (const yy::location& l, const std::string& m);
  void error (const std::string& m);

};

// added 2016-04-15: easier to use
jsObject getJSONObject(const std::string&);
jsValue getJSONValue(const std::string&);

#endif // ! CALCXX_DRIVER_HH
