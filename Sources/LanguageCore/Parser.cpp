//
// Created by Balalarga on 06.03.2022.
//

#include "Parser.h"

Program Parser::Parse(Lexer &lexer)
{
    Program program;
    while (!lexer.Empty())
    {
        const Lexeme& lexeme = lexer.Top();
        std::string tokenName;
        if (std::holds_alternative<std::string>(lexeme.value))
            tokenName = get<std::string>(lexeme.value);

        if(lexeme.token == Token::Id)
        {
            if(token.name == "argument" ||
                    token.name == "arg")
            {
                HandleArgument(table);
            }
            else if(token.name == "constant" ||
                    token.name == "const")
            {
                HandleConstant(table);
            }
            else if(token.name == "return")
            {
                auto expr = HandleReturn(table);
                program->Init(expr);
            }
            else if (tokenName == "variable" ||
                 tokenName == "var")
            {
                auto expr = HandleVariable(table);
                if(expr)
                    table.Add(expr);
            }
        }
        else
        {
            error = "Unknown token " + token.name;
        }
    }

    return program;
}
