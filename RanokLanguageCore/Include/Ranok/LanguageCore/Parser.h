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
    Program Parse(Lexer lexer);

    inline const std::string& Error() const { return _error; }

private:
    Program* _program = nullptr;
    Lexer* _lexer = nullptr;
    std::string _error;

    Lexeme LexerCheckedPop(Token token = Token::None);
    Lexeme LexerCheckedTop();

    void HandleArgument();
    void HandleVariable();
    void HandleFunctionArgs(std::vector<spExpression>& args);
    void HandleFunctionArgs();
    spExpression HandleReturn();
    spExpression HandleArray();
    
    spExpression Term();
    spExpression Factor();
    spExpression Expr();

};


#endif //RANOKLANGUAGE_PARSER_H
