#include "lexer.h"
#include <ctype.h>

namespace mud
{

Token Lexer::get()
{
    if (!buf.empty()) {
        Token r = buf.front();
        buf.pop_front();
        return r;
    }

    char c = is.get();
    while ( isspace(c))
        c = is.get();

    if (is.eof())
        return Token(TOK_EOF);

    switch(c) {
        case PLUS:    
        case MINUS:
        case MUL:
        case DIV:
        case LBRACKET:
        case RBRACKET:
        case TOK_EOL:
        case ASSGIN:
            return Token(c);
        case ':':
            char c1 = is.get();
            if (c1 == '=') {
                return Token(AUTO_ASSGIN,string(":="));
            }
    }

    if ( is_number_head(c) ) {
        is.unget();
        return parse_double();
    }

    if ( is_head_char(c)) {
        is.unget();
       return parse_name(); 
    }

    throw UnexpectedChar(
            string("Lexer::get : unexpected char '") + c + '\'');
}

void Lexer::unget(Token tok)
{
    if ( buf.size() >= BUFFER_SIZE )
        throw LexerNotNull("Lexer::unget : buf is full");
    buf.push_back(tok);
}

Token Lexer::parse_name()
{
    string str;
    char c;

    c = is.get();
    str+=c;

    while( is.get(c)) {
        if ( !is_name_char(c) ) { 
            is.unget();
            break;
        }
        str+=c;
    }

    // deal with keywords
    if ( str == "var") 
        return Token(VAR, str);

    if ( str == "print" )
        return Token(PRINT, str);

    if ( str == "double" )
        return Token(DOUBLE, str);

    return Token(NAME, str);
}

Token Lexer::parse_double()
{
    string str;
    char c;

    c = is.get();
    str+=c;

    // integer part
    while ( is.get(c))  {
       if ( !isdigit(c) ) {
           is.unget();
           break;
       }
       str += c;
    }

    // decimal part
    c =is.get();
    if ( c != '.' && c != 'E' && c!= 'e') {
        is.unget();
        return Token(NUMBER, str);
    }

    if (c == '.') {
        str += c;
        while ( is.get(c))  {
            if ( !isdigit(c) ) {
                is.unget();
                break;
            }
            str += c;
        }
        c = is.get();
        if ( c != 'E' && c != 'e') {
            is.unget();
            return Token(NUMBER, str);
        }
    }

    // the part afer 'E' or 'e'
    str += c;
    while ( is.get(c))  {
        if ( !isdigit(c) ) {
            is.unget();
            break;
        } 
        str += c;
    }

    return Token(NUMBER, str);
}

} // end of mud
