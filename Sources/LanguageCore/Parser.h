//
// Created by Balalarga on 06.03.2022.
//

#ifndef RANOKLANGUAGE_PARSER_H
#define RANOKLANGUAGE_PARSER_H

#include "Lexer.h"
#include "Program.h"
#include "SymbolsTable.h"


class Parser
{
public:
    Parser() = default;
    Program Parse(Lexer& lexer);
    

private:
    void HandleArgument(Lexer& lexer, SymbolsTable& symbolsTable);
    void HandleConstant(Lexer& lexer, SymbolsTable& symbolsTable);
    void HandleVariable(Lexer& lexer, SymbolsTable& symbolsTable);
    spExpression HandleReturn(Lexer& lexer, SymbolsTable& symbolsTable);
    
    spExpression Term(Lexer& lexer, SymbolsTable& symbolsTable);
    spExpression Factor(Lexer& lexer, SymbolsTable& symbolsTable);
    spExpression Expr(Lexer& lexer, SymbolsTable& symbolsTable);

};


#endif //RANOKLANGUAGE_PARSER_H
