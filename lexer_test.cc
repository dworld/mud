#include <iostream>
#include <fstream>
#include <iomanip>
#include "lexer.h"

using namespace std;
using namespace mud;

int main(int argc, char ** argv)
{
    if (argc != 2) {
        cout << "usage: lexer_test file\n";
        return 1;
    }

    ifstream ifs(argv[1]);
    if (!ifs) {
        cout << "can't open file \n"; 
        return 1;
    }

    mud::Lexer lexer(ifs);
    mud::Token tok;
    tok = lexer.get();
    while( tok.get_type() != mud::TOK_EOF ) {
        cout << "Token :   ";

        switch (tok.get_type()) {
            case NONE:
               cout << left << setw(16) << "NONE"; 
               break;
            case NUMBER:
               cout << left << setw(16) << "NUMBER"; 
               break;
            case NAME:
               cout << left << setw(16) << "NAME"; 
               break;
            case VAR:
               cout << left << setw(16) << "VAR"; 
               break;
            case PRINT:
               cout << left << setw(16) << "PRINT"; 
               break;
            case AUTO_ASSGIN:
               cout << left << setw(16) << "AUTO_ASSGIN"; 
               break;
            case PLUS:
               cout << left << setw(16) << "PLUS"; 
               break;
            case MINUS:
               cout << left << setw(16) << "MINUS"; 
               break;
            case MUL:
               cout << left << setw(16) << "MUL"; 
               break;
            case DIV:
               cout << left << setw(16) << "DIV"; 
               break;
            case LBRACKET:
               cout << left << setw(16) << "LBRACKET"; 
               break;
            case RBRACKET:
               cout << left << setw(16) << "RBRACKET"; 
               break;
            case ASSGIN:
               cout << left << setw(16) << "ASSGIN"; 
               break;
            case DOUBLE:
               cout << left << setw(16) << "DOUBLE";
               break;
            case TOK_EOL:
               cout << left << setw(16) << "TOK_EOL";
               break;
            case TOK_EOF:
               cout << left << setw(16) << "TOK_EOF"; 
               break;
            default:
               cout << "Unknown token type. error!" << endl;
               return 0;
        }

        cout << tok.get_string() <<endl;  
        tok = lexer.get();
    }

    return 0;
}
