#include <iostream>
#include <fstream>
#include <memory>
#include "parser.h"

using namespace mud;
using namespace std;

int main(int argc, char** argv)
{
    if (argc != 2) {
        cout << "usage: mud file\n";
        return 1;
    }

    ifstream ifs(argv[1]);
    if (!ifs) {
        cout << "can't open file \n"; 
        return 1;
    }

    mud::Lexer lexer(ifs);
    fragment* p = new program();
    auto_ptr<fragment> auto_p;

    try {
        p = p->parse(lexer);
        auto_p.reset(p);
        cout << "PARSE END" << endl;

        cout << "\nPROGROM:\n\n";
        cout << "--------------------------------\n";
        p->print();
        cout << "\n--------------------------------\n";
        cout << "\n";

        double r = 0;
        r = p->excute();
        cout<<"RESULT: " << r <<endl;
    } 
    catch (runtime_error err) {
        cout << "ERROR: "  << err.what() << endl;
    } 
    catch(...) {
        cout << "ERROR: unkonwn error"  << endl;
    }

    return 0;
}
