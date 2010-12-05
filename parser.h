#ifndef MUD_PARSER_H
#define MUD_PARSER_H

#include <string>
#include <stdexcept>
#include <map>
#include "lexer.h"

namespace mud
{

using std::string;
using std::runtime_error;
using std::map;
using std::cout;
using std::endl;

extern map<string, double> env;

typedef map<string, double>::const_iterator ENV_CITOR;
typedef map<string, double>::iterator ENV_ITOR;

class fragment
{
public:
    virtual fragment* parse(Lexer& l) = 0;
    virtual double excute() = 0;
    virtual void print() = 0;
    virtual ~fragment() {} ;
};

class assgin_expr;

class primary:public fragment
{
public:
    enum type{PRI_INVALID, PRI_NUMBER, PRI_IDENTITY, PRI_EXPR};

    primary();

    virtual fragment* parse(Lexer& l);
    virtual double excute();
    virtual void print();
    virtual ~primary();

private:
    type _type;
    double number;
    string identity;
    assgin_expr* pExpr;
    bool negative;
};

class multi_expr:public fragment
{
public:
    multi_expr()
        :op(NONE), pLeft(NULL), pRight(NULL)
    {}

    virtual fragment* parse(Lexer& l);
    virtual double excute();
    virtual void print();
    virtual ~multi_expr();

    void set_left(multi_expr* left);
    void set_op(Token_type v);

private:
    Token_type op;
    multi_expr* pLeft;
    primary* pRight;
};

class add_expr:public fragment
{
public:
    add_expr()
        :op(NONE), pLeft(NULL), pRight(NULL)
    {}

    virtual fragment* parse(Lexer& l);
    virtual double excute();
    virtual void print();
    virtual ~add_expr();

    void set_left(add_expr* left);
    void set_op(Token_type v); 

private:
    Token_type op;
    add_expr* pLeft;
    multi_expr*  pRight; 
};

class assgin_expr:public fragment
{
public:
    assgin_expr()
        :identity(), pAdd(NULL), pAssgin(NULL)
    {}

    virtual fragment* parse(Lexer& l);
    virtual double excute();
    virtual void print();
    virtual ~assgin_expr();

private:
    string identity;
    add_expr* pAdd;
    assgin_expr* pAssgin;
};

class var_def:public fragment
{
public:
    var_def()
        :identity(), pAssgin(NULL)
    {}

    virtual fragment* parse(Lexer& l);
    virtual double excute();
    virtual void print();
    virtual ~var_def();

private:
    string identity;
    assgin_expr* pAssgin;
};

class print_fun:public fragment
{
public:
    print_fun()
        :pAssgin(NULL)
    {}

    virtual fragment* parse(Lexer& l);
    virtual double excute();
    virtual void print();
    virtual ~print_fun();

private:
    assgin_expr* pAssgin;
};

class program:public fragment
{
public:
    program()
        :fragments()
    {}

    virtual fragment* parse(Lexer& l);
    virtual double excute();
    virtual void print();
    virtual ~program();

private:
    list<fragment*> fragments;
};

};//end of mud

#endif //MUD_PARSER_H
