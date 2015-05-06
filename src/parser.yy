%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.4"
%defines
%define parser_class_name {calcxx_parser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%code requires
{
# include <string>
class calcxx_driver;
}
// The parsing context.
%param { calcxx_driver& driver }
%locations
// %initial-action
// {
//   // Initialize the initial location.
//   @$.begin.filename = @$.end.filename = &driver.file;
// };
%define parse.trace
%define parse.error verbose
%code
{
# include "json.hpp"
}
%define api.token.prefix {TOK_}
%token
  END  0       "end of file"
  OBJECTOPEN   "{"
  OBJECTCLOSE  "}"
  ARRAYOPEN    "["
  ARRAYCLOSE   "]"
  QUOTE        "DQ"
  SQUOTE       "SQ"
  COLON        ":"
;
%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
%type  <int> jsonexp
%printer { yyoutput << $$; } <*>;
%%
%start jsonexp;

jsonexp:
  %empty     {}
| OBJECTOPEN IDENTIFIER ":" NUMBER OBJECTCLOSE    {driver.result = $4;};

%%
void
yy::calcxx_parser::error (const location_type& l,
                          const std::string& m)
{
  driver.error (l, m);
}
