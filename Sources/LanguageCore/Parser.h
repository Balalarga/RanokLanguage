//
// Created by Balalarga on 06.03.2022.
//

#ifndef RANOKLANGUAGE_PARSER_H
#define RANOKLANGUAGE_PARSER_H

#include "Lexer.h"
#include "Program.h"


class Parser
{
    Parser() = default;
    Program Parse(Lexer& lexer);


private:
};


#endif //RANOKLANGUAGE_PARSER_H
