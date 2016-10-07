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
  COLON        ":"
  COMMA        ","
;
%token <std::string> IDENTIFIER "identifier"
%token <long> NUMBER_I 
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
        | STRING      {
            std::string s =$1.substr(1,$1.size()-2);
            jsValue v(s, true);  // true = decode from JSON (e.g. \\n to \n)
            $$ = std::move(v);}
        | ARRAYOPEN jsarray ARRAYCLOSE    {$$ = std::move($2);}
        | OBJECTOPEN jsobject OBJECTCLOSE {$$ = std::move($2);}
      ;

jsarray : %empty   { 
                      std::vector<jsValue> stdVector;
//                      jsValue jsArray(std::move(stdVector));
                      jsValue jsArray((stdVector));
                      $$ = std::move(jsArray);
                   }
        | jsvalue  {
                      std::vector<jsValue> stdVector;
                      jsValue oneValue($1); 
                      stdVector.push_back(std::move(oneValue));
//                      jsValue jsArray (std::move(stdVector));
                      jsValue jsArray ((stdVector));
                      $$ = std::move(jsArray);
                    }
        | jsarray COMMA jsvalue  {
                      $1.add($3); $$ = std::move($1);}
        ;

jsobject : %empty  {
                      jsObject object_;
//                      jsValue jsObject_(std::move(object_));
                      jsValue jsObject_((object_));
                      $$ = std::move(jsObject_);
                   }
         | STRING COLON jsvalue {
                      jsObject object_;
                      std::string key = $1.substr(1,$1.size()-2);
//                      jsValue jsObject_(std::move(object_));
                      jsValue jsObject_((object_));
                      jsObject_.add(key,$3);
                      $$ = std::move(jsObject_);
                   }
         | jsobject COMMA STRING COLON jsvalue {
                      std::string key = $3.substr(1,$3.size()-2);
                      $1.add(key,$5); $$ = std::move($1);}
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
  throw std::string ("parsing JSON: ")  + m;
//  driver.error (l, m);
}
