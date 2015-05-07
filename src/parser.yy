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
# include "jsObject.hpp"
# include "jsValue.hpp"
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
  QUOTE        "Double Quote"
  SQUOTE       "'"
  COLON        ":"
  COMMA        ","
;
%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER_I 
%token <double> NUMBER_F 
%token <std::string> STRING 
%type  <jsValue> jsonexp
%type  <jsValue> jsvalue
%type  <jsValue> jsarray 
%type  <jsValue> jsobject 

%printer { yyoutput << $$; } <*>;

%start jsonexp;

%%

// following works but less preferred because dynamic allocation by-hand:
//  {$$ = *(new jsValue($1)); }


jsonexp : jsvalue {driver.result =std::move($1);};

jsvalue : NUMBER_I    {$$ = *(new jsValue($1));}
        | NUMBER_F    {jsValue v($1); $$ = std::move(v); }
        | STRING      {jsValue v($1.substr(1,$1.size()-2)); $$ = std::move(v);}
        | ARRAYOPEN jsarray ARRAYCLOSE    {$$ = std::move($2);}
        | OBJECTOPEN jsobject OBJECTCLOSE {$$ = std::move($2);}
      ;

jsarray : %empty   { 
                      std::vector<jsValue> stdVector;
                      jsValue jsArray(std::move(stdVector));
                      $$ = std::move(jsArray);
                   }
        | jsvalue  {
                      std::vector<jsValue> stdVector;
                      jsValue oneValue($1); 
                      stdVector.push_back(std::move(oneValue));
                      jsValue jsArray (std::move(stdVector));
                      $$ = std::move(jsArray);
                    }
        | jsarray COMMA jsvalue  {
                      $1.add($3); $$ = std::move($1);}
        ;

jsobject : %empty  {
                      jsObject object_;
                      jsValue jsObject_(std::move(object_));
                      $$ = std::move(jsObject_);
                   }
         | IDENTIFIER COLON jsvalue {
                      jsObject object_;
                      jsValue jsObject_(std::move(object_));
                      jsObject_.add($1,$3);
                      $$ = std::move(jsObject_);
                   }
         | jsobject COMMA IDENTIFIER COLON jsvalue {
                      $1.add($3,$5); $$ = std::move($1);}
         ;


//  jsobject : OBJECTOPEN objectContent OBJECTCLOSE {$$ = $2;};
//  
//  objectContent :
//    %empty     {}
//  | OBJECTOPEN IDENTIFIER ":" NUMBER OBJECTCLOSE    {driver.result = $4;};

%%
void
yy::calcxx_parser::error (const location_type& l,
                          const std::string& m)
{
  driver.error (l, m);
}
