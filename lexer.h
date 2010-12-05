#ifndef MUD_LEXER_H
#define MUD_LEXER_H

#include <string>
#include <list>
#include <iostream>
#include <stdexcept>

namespace mud
{

using std::string;
using std::istream;
using std::cin;
using std::runtime_error;
using std::list;

class LexerNotNull:public runtime_error
{
public:
    explicit LexerNotNull(const char* msg)
        :runtime_error(msg) 
    {}
};

class UnexpectedChar:public runtime_error
{
public:
    explicit UnexpectedChar(const char* msg)
        :runtime_error(msg)
    {}
    
    explicit UnexpectedChar(string msg)
        :runtime_error(msg)
    {}
};

enum Token_type{
    PLUS = '+', MINUS = '-', MUL = '*', DIV = '/', 
    LBRACKET = '(', RBRACKET = ')', ASSGIN = '=', TOK_EOL = ';',
    NONE = 128, NUMBER, NAME, VAR, PRINT, AUTO_ASSGIN, TOK_EOF, DOUBLE
};

class Token
{
public:
    Token() : type(NONE), str() {};
    explicit Token(char op) :type(Token_type(op)), str() { str += op; };
    explicit Token(Token_type t):type(t) {};
    Token(Token_type t, string s) : type(t), str(s) {};

    Token_type      get_type()   const { return type; };
    const string&   get_string() const { return str; } 

private:
    Token_type type;
    string str;
};


class Lexer
{
public:
    Lexer() : is(cin), buf() {};
    explicit Lexer(istream& ss) : is(ss), buf() {};

    Token get();
    void  unget(Token tok);

    const static int BUFFER_SIZE = 3;

private:
    bool  is_head_char(char c) { return isalpha(c) || (c == '_'); };
    bool  is_name_char(char c) { return isalnum(c) || (c == '_');  };
    bool  is_number_head(char c) 
    { return isdigit(c) || c == '+' || c == '-'; };

    Token parse_double();
    Token parse_name();

    istream& is;
    list<Token>  buf;
};

}; // end of mud

#endif // MUD_LEXER_H
