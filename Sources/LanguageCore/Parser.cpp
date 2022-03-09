//
// Created by Balalarga on 06.03.2022.
//

#include "Parser.h"
#include "Operations.h"

#include <iostream>


Program Parser::Parse(Lexer lexer)
{
    auto CaseCompare = [](std::string str1, const std::string& str2, bool sensitive = false)
    {
        if (!sensitive)
        {
            std::transform(str1.begin(),
                           str1.end(),
                           str1.begin(),
                           [](unsigned char c){ return std::tolower(c); });
        }
        return str1 == str2;
    };

    Program program;
    _program = &program;
    _lexer = &lexer;
    while (!_lexer->Empty())
    {
        Lexeme& lexeme = _lexer->Top();
        std::string tokenName;
        if (lexeme.Is<std::string>())
            tokenName = lexeme.Get<std::string>();

        if (lexeme.type == Token::Id)
        {
            if (CaseCompare(tokenName, "argument") ||
                CaseCompare(tokenName, "arguments") ||
                CaseCompare(tokenName, "args"))
            {
                std::cout<<"Arguments handling started\n";
                HandleArgument();
                std::cout<<"Arguments handled finished\n";
            }
            else if (CaseCompare(tokenName, "constant") ||
                     CaseCompare(tokenName, "const"))
            {
                HandleConstant();
            }
            else if(CaseCompare(tokenName, "return"))
            {
                program.Init(HandleReturn());
            }
            else if (CaseCompare(tokenName, "variable") ||
                     CaseCompare(tokenName, "var"))
            {
                _lexer->Pop();
                HandleVariable();
            }
            else
            {
                HandleVariable();
            }
        }
        else
        {
            std::cout << "Unknown token " << tokenName << "\n";
        }
        lexeme = _lexer->Pop();
    }

    return program;
}

void Parser::HandleArgument()
{
    Lexeme& lexeme = _lexer->Top();
    spExpression expr;
    while (lexeme.type != Token::Endline)
    {
        ArgumentExpression::Range range{-5, 5};

        lexeme = _lexer->Pop(Token::Id);
        std::string name = lexeme.Get<std::string>();
        lexeme = _lexer->Pop(Token::ParenOpen);
        bool negative = false;
        lexeme = _lexer->Pop();
        if (lexeme.type == Token::Minus)
        {
            negative = true;
            lexeme = _lexer->Pop(Token::Number);
        }
        range.min = lexeme.Get<double>();
        lexeme = _lexer->Pop();
        if (lexeme.type == Token::Comma)
        {
            range.min = negative ? -range.min : range.min;
            lexeme = _lexer->Pop();
            negative = false;
            if (lexeme.type == Token::Minus)
            {
                negative = true;
                lexeme = _lexer->Pop(Token::Number);
            }
            range.max = negative ? -lexeme.Get<double>() : lexeme.Get<double>();
            lexeme = _lexer->Pop();
        }
        else
        {
            range.max = range.min;
            range.min = -range.min;
        }
        lexeme = _lexer->Pop();
        _program->Table().CreateArgument(name, range);
    }
    lexeme = _lexer->Pop();
}


void Parser::HandleConstant()
{
    Lexeme& lexeme = _lexer->Top();
    Expression* expr = nullptr;
    while(lexeme.type != Token::Endline)
    {
        lexeme = _lexer->Pop();
        // CheckToken(Token::Id);
        std::string name = lexeme.Get<std::string>();
        lexeme = _lexer->Pop();
        // CheckToken(Token::Assign);
        lexeme = _lexer->Pop();
        Expr();
    }
    lexeme = _lexer->Pop();
}

void Parser::HandleVariable()
{
    Lexeme& lexeme = _lexer->Top();
    std::string name = lexeme.Get<std::string>();
    lexeme = _lexer->Pop();
    lexeme = _lexer->Pop();
    _program->Table().CreateVariable(name, Expr());
    lexeme = _lexer->Pop();
}

spExpression Parser::HandleReturn()
{
    Lexeme& lexeme = _lexer->Top();
    spExpression expr = Expr();
    lexeme = _lexer->Pop();
    return expr;
}

spExpression Parser::Expr()
{
    std::cout<<"Expr\n";
    spExpression node = Term();
    Lexeme& lexeme = _lexer->Top();
    while (lexeme.type == Token::Minus ||
           lexeme.type == Token::Plus  ||
           lexeme.type == Token::Cross ||
           lexeme.type == Token::Union)
    {
        auto prev = lexeme;
        lexeme = _lexer->Pop();
        FunctionInfo function(prev.Get<std::string>(), Operations::FromString(prev.Get<std::string>()));
        node = std::make_shared<BinaryOperation>(function, node, Term());
    }
    return node;
}

spExpression Parser::Term()
{
    spExpression node = Factor();
    Lexeme& lexeme = _lexer->Top();
    while (lexeme.type == Token::Pow      ||
           lexeme.type == Token::Multiply ||
           lexeme.type == Token::Divide)
    {
        auto prev = lexeme;
        lexeme = _lexer->Pop();
        FunctionInfo function(prev.Get<std::string>(), Operations::FromString(prev.Get<std::string>()));
        node = std::make_shared<BinaryOperation>(function, node, Term());
    }
    return node;
}

spExpression Parser::Factor()
{
    Lexeme& lexeme = _lexer->Top();
    if(lexeme.type == Token::Number)
    {
        auto constant = _program->Table().CreateConstant(lexeme.Get<double>());
        lexeme = _lexer->Pop();
        return constant;
    }
    else if(lexeme.type == Token::ParenOpen)
    {
        lexeme = _lexer->Pop();
        auto expr = Expr();
        lexeme = _lexer->Pop();
        return expr;
    }
    else if(lexeme.type == Token::Minus)
    {
        lexeme = _lexer->Pop();
        FunctionInfo function("-", Operations::unaryMinus);
        return std::make_shared<UnaryOperation>(function, Factor());
    }
    else if(lexeme.type == Token::Id)
    {
        auto prev = lexeme;
        lexeme = _lexer->Pop();
        if (auto expr = _program->Table().FindArgument(prev.Get<std::string>()))
            return expr;

        if (auto expr = _program->Table().FindConstant(prev.Get<std::string>()))
            return expr;

        // if (auto func = LangFunctions::FindFunction(prev.Get<std::string>()))
        //     return std::make_shared<FunctionExpression>(func, Term());

        if (auto expr = _program->Table().FindVariable(prev.Get<std::string>()))
            return expr;
    }
    return nullptr;
}
