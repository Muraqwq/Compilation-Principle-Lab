#include "Parser.h"

int main()
{
    /*Translate translate{};
    TransMethod methods{ translate };
    Grammer grammer{ methods };

    Constructor constructor{ grammer.grammer_list, 'V' };
    auto dfa = constructor.constructDFA();
    constructor.generateTables(dfa);*/

    Parser parser{};
    parser.prework();
    parser.run();
    parser.generate_intermediate_code();
}