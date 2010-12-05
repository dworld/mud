#include "parser.h"
#include "stdlib.h"
#include <assert.h>
#include <iostream>

namespace mud
{

map<string, double> env;

primary::primary()
    :_type(PRI_INVALID), number(0), identity(), pExpr(NULL),negative(false)
{

}

fragment* primary::parse(Lexer& l)
{
    Token t = l.get();
    while ( t.get_type() == MINUS ) 
    {
        negative = !negative;
        t = l.get();
    }

    switch (t.get_type()) {
        case NUMBER:
            _type = PRI_NUMBER;
            number = atof(t.get_string().c_str()); 
            break; 
        case NAME:
            _type = PRI_IDENTITY;
            identity = t.get_string();
            break;
        case LBRACKET:
            _type = PRI_EXPR;
            pExpr = new assgin_expr();
            pExpr->parse(l); 
            t = l.get();
            if (t.get_type() != RBRACKET ) {
                throw runtime_error("primary::parse : invalid token type"
                                    ", \')\' expected");
            }
            break;
        default:
            throw runtime_error("primary::parse : invalid token type");
    }
    return this;
}

primary::~primary()
{
    if (_type == PRI_EXPR) {
        delete pExpr;
    }

}

double primary::excute()
{
    if ( _type == PRI_NUMBER) {
        return negative? -number:number;
    }

    if ( _type == PRI_IDENTITY ) {
        ENV_CITOR p = env.find(identity);
        if ( p != env.end()) {
            return negative?-(p->second):p->second;
        }
        string msg;
        msg = "primary::excute, can't find variable ";
        msg += identity;
        throw runtime_error(msg);
    }

    if ( _type == PRI_EXPR) {
        return negative?-(pExpr->excute()):pExpr->excute();
    }

    throw runtime_error("primary::excute : invalid status");

    return 0;
}

void primary::print()
{
    if (negative) {
        cout << "-";
    }
    switch( _type )  {
        case PRI_NUMBER:
            cout << number;
            break;
        case PRI_IDENTITY:
            cout << identity;
            break;
        case PRI_EXPR:
            cout << "(";
            pExpr->print();
            cout << ")";
            break;
        default:
            throw runtime_error("primary::print : invalid status");
    }
}

void multi_expr::set_left(multi_expr* left)
{
    assert( pLeft == NULL );
    pLeft = left;
}

void multi_expr::set_op(Token_type v)
{
    op = v;
}

fragment* multi_expr::parse(Lexer& l)
{
    pRight = new primary();
    pRight->parse(l);

    Token t = l.get();
    Token_type tok_type = t.get_type();

    if ( tok_type == MUL || tok_type == DIV) {
        multi_expr* pParent = new multi_expr();
        pParent->set_left(this);
        pParent->set_op(tok_type);
        return pParent->parse(l);
    }

    l.unget(t);
    return this;
}

double multi_expr::excute()
{
    if ( op == NONE && pRight != NULL) {
        return pRight->excute();
    }

    if ( op == MUL || op == DIV ) {
        double lv = pLeft->excute();
        double rv = pRight->excute();
        switch( op ) {
            case MUL:
                return lv  * rv;
            case DIV:
                if ( rv == 0 ) {
                    throw runtime_error("multi_expr:excute : divd by 0");
                }
                return lv / rv;
        }
    }
    throw runtime_error("multi_expr::excute : invalid multi-expr status");
}

void multi_expr::print()
{
    if ( op == NONE && pRight != NULL) {
        pRight->print();
        return;
    }

    if ( op == MUL || op == DIV ) {
        pLeft->print();
        switch( op ) {
            case MUL:
                cout << '*';
                break;
            case DIV:
                cout << '/';
                break;
        }
        pRight->print();
        return;
    }
    throw runtime_error("multi_expr::print : invalid multi-expr status");
}

multi_expr::~multi_expr()
{
    if( pRight != NULL )
        delete pRight;

    if ( pLeft != NULL )
        delete pLeft;
}

void add_expr::set_left(add_expr* left)
{
    assert( pLeft == NULL );
    pLeft = left;
}

void add_expr::set_op(Token_type v)
{
    op = v;
}

fragment* add_expr::parse(Lexer& l)
{
    pRight = new multi_expr();
    pRight =dynamic_cast<multi_expr*>(pRight->parse(l));

    Token t = l.get();
    Token_type tok_type = t.get_type();

    if ( tok_type == PLUS || tok_type == MINUS) {
        add_expr* pParent = new add_expr();
        pParent->set_left(this);
        pParent->set_op(tok_type);
        return pParent->parse(l);
    }

    l.unget(t);
    return this;
}

double add_expr::excute()
{
    if ( op == NONE && pRight != NULL) {
        return pRight->excute();
    }

    if ( op == PLUS || op == MINUS ) {
        double lv = pLeft->excute();
        double rv = pRight->excute();
        switch( op ) {
            case PLUS:
                return lv + rv;
            case MINUS:
                return lv - rv;
        }
    }
    throw runtime_error("add_expr::excute : invalid multi-expr status");
}

void add_expr::print()
{
    if ( op == NONE && pRight != NULL) {
        pRight->print();
        return;
    }

    if ( op == PLUS || op == MINUS ) {
        pLeft->print();
        switch( op ) {
            case PLUS:
                cout << '+';
                break;
            case MINUS:
                cout << '-';
                break;
        }
        pRight->print();
        return;
    }
    throw runtime_error("add_expr::print : invalid multi-expr status");
}

add_expr::~add_expr()
{
    if( pRight != NULL )
        delete pRight;

    if ( pLeft != NULL )
        delete pLeft;
}

fragment* assgin_expr::parse(Lexer& l)
{
    Token tok1;
    Token tok2;
    tok1 = l.get();
    tok2 = l.get();

    if ( tok1.get_type() == NAME && tok2.get_type() == ASSGIN ) {
        identity = tok1.get_string();
        pAssgin = new assgin_expr();
        pAssgin->parse(l);
    } else {
        l.unget(tok1);
        l.unget(tok2);
        pAdd = new add_expr(); 
        pAdd = dynamic_cast<add_expr*>(pAdd->parse(l));
    }
    return this;
}

double assgin_expr::excute()
{
    assert( pAdd == NULL || pAssgin == NULL);

    if ( pAdd != NULL)
        return pAdd->excute();    

    if ( pAssgin != NULL) {
        ENV_ITOR p = env.find(identity);
        if ( p == env.end() ) {
            string msg;
            msg = "assgin_expr::excute : identity \" ";
            msg = msg + identity + " \" not exist";
            throw runtime_error(msg);
        }
        double v = pAssgin->excute();
        p->second = v;
        return v;
    }
}

void assgin_expr::print()
{
    assert( pAdd == NULL || pAssgin == NULL);

    if ( pAdd != NULL) {
        pAdd->print();
        return;    
    }

    if ( pAssgin != NULL) {
        cout << identity << " = ";
        pAssgin->print();
        return;
    }

}

assgin_expr::~assgin_expr()
{
    if ( pAdd != NULL )
        delete pAdd;

    if ( pAssgin != NULL )
        delete pAssgin;
}

fragment* var_def::parse(Lexer& l)
{
    Token tok;
    tok = l.get();

    if ( tok.get_type() != VAR) {
        throw runtime_error("var_def::parse : \'var\' expected");
    }

    tok = l.get();
    if ( tok.get_type() == DOUBLE ) {
        tok = l.get();
        if ( tok.get_type() != NAME ) {
            throw runtime_error("var_def::parse : normal assgin, identity"
                                " expected");
        }
        identity = tok.get_string();
        tok = l.get();
        if ( tok.get_type() == ASSGIN ) {
            pAssgin = new assgin_expr();
            pAssgin->parse(l);            
        } else {
            l.unget(tok);
        }
        return this;
    } 

    if (tok.get_type() == NAME )
    {
        Token tok2;
        tok2 = l.get();
        if ( tok2.get_type() != AUTO_ASSGIN )  {
            throw runtime_error("var_def::parse : \':=\' expected");
        }
        identity = tok.get_string();
        pAssgin = new assgin_expr();
        pAssgin->parse(l);
        return this;
    }

    throw runtime_error("var_def::parse : invalid token type");
}

double var_def::excute()
{
    ENV_ITOR p = env.find(identity);
    if ( p != env.end() ) {
        throw runtime_error("var_def::excute : var \'" + identity
                             + "\'redefine");
    }
    env[identity] = 0;
    if  ( pAssgin != NULL ) {
        env[identity] = pAssgin->excute(); 
    }
    return env[identity];
}

void var_def::print()
{
    cout << "var double " << identity;
    if ( pAssgin != NULL ) {
        cout << " = ";
        pAssgin->print();
    }
}

var_def::~var_def()
{
    if ( pAssgin != NULL ) {
        delete pAssgin;
    }
}

fragment* print_fun::parse(Lexer& l)
{
    Token tok;
    tok = l.get();

    if ( tok.get_type() != PRINT ) {
        throw runtime_error("print_fun::parse : invalid token type"
                            " \'print\' expected");
    }
    
    tok = l.get();
    if ( tok.get_type() != LBRACKET ) {
        throw runtime_error("print_fun::parse : invalid token type"
                            " \'(\' expected");
    }

    pAssgin = new assgin_expr();
    pAssgin->parse(l);
    
    tok = l.get();
    if ( tok.get_type() != RBRACKET ) {
        throw runtime_error("print_fun::parse : invalid token type"
                            " \')\' expected");
    }
   
    return this; 
}

double print_fun::excute()
{
    cout << "print: " << pAssgin->excute() << endl;
    return 0;    
}

void print_fun::print()
{
    cout << "print(";
    pAssgin->print();
    cout << ")";
}

print_fun::~print_fun()
{
    if  ( pAssgin != NULL )
        delete pAssgin;
}

fragment* program::parse(Lexer& l)
{
    Token tok;
    tok = l.get();
    while ( tok.get_type() != TOK_EOF ) {
        l.unget(tok);
        switch ( tok.get_type() )  {
            case VAR:
                {
                    var_def* pVar = new var_def();
                    pVar->parse(l);
                    fragments.push_back(pVar);
                }
                break;
            case PRINT:
                {
                    print_fun* pPrint = new print_fun();
                    pPrint->parse(l);
                    fragments.push_back(pPrint);
                    break;
                }
            default:
                {
                    assgin_expr* pAssgin = new assgin_expr();
                    pAssgin->parse(l);
                    fragments.push_back(pAssgin);
                }
        }
        tok = l.get();
    }
    return this;
}

double program::excute()
{
    list<fragment*>::iterator p;
    p = fragments.begin();
    for (; p != fragments.end(); p++)
    {
        (*p)->excute();
    }
    return 0;
}

void program::print()
{
    list<fragment*>::iterator p;
    p = fragments.begin();
    for (; p != fragments.end(); p++)
    {
        (*p)->print();
        cout << endl;
    }
}

program::~program()
{
    list<fragment*>::iterator p;
    p = fragments.begin();
    for (; p != fragments.end(); p++)
    {
        if ( *p != NULL ) {
            delete *p;
        }
    }
}

}// end of mud
