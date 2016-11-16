%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <string>
# include "json.hpp"
# include "parser.tab.hh"

// Work around an incompatibility in flex (at least versions
// 2.5.31 through 2.5.33): it generates code that does
// not conform to C89.  See Debian bug 333231
// <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.
# undef yywrap
# define yywrap() 1

// The location of the current token.
static yy::location loc;
// regexp doubleQuotedString:
// read opening double quotes, 
// read any repetition of: (either \" or something neiter \ nor ")
// read closing double quotes
// do NOT read '-' as part of a number, we parse this later
%}
%option noyywrap nounput batch debug noinput
doubleQuotedString   \"(\\\"|[^"])*\"
singleQuotedString   \'[^\"]*\'
id    [a-zA-Z][a-zA-Z_0-9]*
int   [+]?[0-9]+
dblE   [0-9]\.[0-9]*([eE][-+]?[0-9][0-9])
dblF   [0-9]*\.[0-9]*
dblS   [+]?[0-9]*\.?[0-9]*[eE][-+]?[0-9]+
blank [ \t]

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}

%%

%{
  // Code run each time yylex is called.
  loc.step ();
%}

{blank}+   loc.step ();
[\n]+      loc.lines (yyleng); loc.step ();
"{"        return yy::calcxx_parser::make_OBJECTOPEN(loc);
"}"        return yy::calcxx_parser::make_OBJECTCLOSE(loc);
"["        return yy::calcxx_parser::make_ARRAYOPEN(loc);
"]"        return yy::calcxx_parser::make_ARRAYCLOSE(loc);
":"        return yy::calcxx_parser::make_COLON(loc);
","        return yy::calcxx_parser::make_COMMA(loc);
"-"        return yy::calcxx_parser::make_MINUS(loc);

{int}      {
  errno = 0;
  long n = strtol (yytext, NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    driver.error (loc, "integer is out of range");
  return yy::calcxx_parser::make_NUMBER_I(n, loc);
}

{dblE}      {
  // double scientific format
  return yy::calcxx_parser::make_NUMBER_F(yytext, loc);
}

{dblF}      {
  // double fixed format: convert to scientific with standard precision 
  std::stringstream ss("");
  std::size_t pos = std::string(yytext).find(".");
  ss.precision(strlen(yytext) - pos);
  ss << std::scientific << std::stod (yytext, NULL);
  return yy::calcxx_parser::make_NUMBER_F(ss.str(), loc);
}

{dblS}      {
  // double non-standard "scientific" format, e.g. .2E5 or 12.1E033
  // exact same code as above ...
  std::stringstream ss("");
  ss.precision(LOWPRECISION);
  ss << std::scientific << std::stod (yytext, NULL);
  return yy::calcxx_parser::make_NUMBER_F(ss.str(), loc);
}

{doubleQuotedString} {

  std::size_t pos=0;
  std::string s=yytext; // darf ich neu allokieren ???

  // ??? warum muss ich enclosing doublequotes nicht wegnehmen ???
  // folgendes nimmt 1. und letzte char des enthaltenden strings weg
  //  s = s.substr(1,s.size() - 2);

  // decode string from json (i.e. unescape \\ and \" )
  // following should *not* decode e.g.:  abc\\"def 
  while ((pos=s.find("\\\"",pos)) != std::string::npos) {
    s.replace(pos,2,"\"");
    pos+=1;
  }
  pos=0;
  while ((pos=s.find("\\\\",pos)) != std::string::npos) {
    s.replace(pos,2,"\\");
    pos+=1;
  }

//  std::cout << "yytext: " << yytext << "\ndecoded:" << s <<std::endl;
  
  return yy::calcxx_parser::make_STRING(s, loc);
}
{singleQuotedString}       return yy::calcxx_parser::make_STRING(yytext, loc);
{id}                       return yy::calcxx_parser::make_IDENTIFIER(yytext, loc);
.          driver.error (loc, "invalid character");
<<EOF>>    return yy::calcxx_parser::make_END(loc);
%%

void
calcxx_driver::scan_begin (std::string s, int debug_)
{
  yy_scan_string(s.c_str()); // see FLEX documentation
  yyset_debug(debug_);            // aus scanner.cpp gelesen ..
}

