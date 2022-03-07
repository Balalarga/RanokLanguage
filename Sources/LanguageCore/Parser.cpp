//
// Created by Balalarga on 06.03.2022.
//

#include "Parser.h"
#include "Operations.h"

#include <iostream>


Program Parser::Parse(Lexer &lexer)
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
    SymbolsTable& symbolsTable = program.Table();
    while (!lexer.Empty())
    {
        Lexeme& lexeme = lexer.Top();
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
                HandleArgument(lexer, symbolsTable);
                std::cout<<"Arguments handled finished\n";
            }
            else if (CaseCompare(tokenName, "constant") ||
                     CaseCompare(tokenName, "const"))
            {
                HandleConstant(lexer, symbolsTable);
            }
            else if(CaseCompare(tokenName, "return"))
            {
                program.Init(HandleReturn(lexer, symbolsTable));
            }
            else if (CaseCompare(tokenName, "variable") ||
                     CaseCompare(tokenName, "var"))
            {
                lexer.Pop();
                HandleVariable(lexer, symbolsTable);
            }
            else
            {
                HandleVariable(lexer, symbolsTable);
            }
        }
        else
        {
            std::cout << "Unknown token " << tokenName << "\n";
        }
        lexeme = lexer.Pop();
    }

    return program;
}

void Parser::HandleArgument(Lexer& lexer, SymbolsTable& symbolsTable)
{
    Lexeme& lexeme = lexer.Top();
    spExpression expr;
    while (lexeme.type != Token::Endline)
    {
        ArgumentExpression::Range range{-5, 5};

        lexeme = lexer.Pop(Token::Id);
        std::string name = lexeme.Get<std::string>();
        lexeme = lexer.Pop(Token::ParenOpen);
        bool negative = false;
        lexeme = lexer.Pop();
        if (lexeme.type == Token::Minus)
        {
            negative = true;
            lexeme = lexer.Pop(Token::Number);
        }
        range.min = lexeme.Get<double>();
        lexeme = lexer.Pop();
        if (lexeme.type == Token::Comma)
        {
            range.min = negative ? -range.min : range.min;
            lexeme = lexer.Pop();
            negative = false;
            if (lexeme.type == Token::Minus)
            {
                negative = true;
                lexeme = lexer.Pop(Token::Number);
            }
            range.max = negative ? -lexeme.Get<double>() : lexeme.Get<double>();
            lexeme = lexer.Pop();
        }
        else
        {
            range.max = range.min;
            range.min = -range.min;
        }
        lexeme = lexer.Pop();
        symbolsTable.CreateArgument(name, range);
    }
    lexeme = lexer.Pop();
}


void Parser::HandleConstant(Lexer& lexer, SymbolsTable& symbolsTable)
{
    Lexeme& lexeme = lexer.Top();
    Expression* expr = nullptr;
    while(lexeme.type != Token::Endline)
    {
        lexeme = lexer.Pop();
        // CheckToken(Token::Id);
        std::string name = lexeme.Get<std::string>();
        lexeme = lexer.Pop();
        // CheckToken(Token::Assign);
        lexeme = lexer.Pop();
        Expr(lexer, symbolsTable);
    }
    lexeme = lexer.Pop();
}

void Parser::HandleVariable(Lexer& lexer, SymbolsTable& symbolsTable)
{
    Lexeme& lexeme = lexer.Top();
    std::string name = lexeme.Get<std::string>();
    lexeme = lexer.Pop();
    lexeme = lexer.Pop();
    symbolsTable.CreateVariable(name, Expr(lexer, symbolsTable));
    lexeme = lexer.Pop();
}

spExpression Parser::HandleReturn(Lexer& lexer, SymbolsTable& symbolsTable)
{
    Lexeme& lexeme = lexer.Top();
    spExpression expr = Expr(lexer, symbolsTable);
    lexeme = lexer.Pop();
    return expr;
}

spExpression Parser::Expr(Lexer& lexer, SymbolsTable& symbolsTable)
{
    std::cout<<"Expr\n";
    spExpression node = Term(lexer, symbolsTable);
    Lexeme& lexeme = lexer.Top();
    while (lexeme.type == Token::Minus ||
           lexeme.type == Token::Plus  ||
           lexeme.type == Token::Cross ||
           lexeme.type == Token::Union)
    {
        auto prev = lexeme;
        lexeme = lexer.Pop();
        FunctionInfo function(prev.Get<std::string>(), Operations::FromString(prev.Get<std::string>()));
        node = std::make_shared<BinaryOperation>(function, node, Term(lexer, symbolsTable));
    }
    return node;
}

spExpression Parser::Term(Lexer& lexer, SymbolsTable& symbolsTable)
{
    spExpression node = Factor(lexer, symbolsTable);
    Lexeme& lexeme = lexer.Top();
    while (lexeme.type == Token::Pow      ||
           lexeme.type == Token::Multiply ||
           lexeme.type == Token::Divide)
    {
        auto prev = lexeme;
        lexeme = lexer.Pop();
        FunctionInfo function(prev.Get<std::string>(), Operations::FromString(prev.Get<std::string>()));
        node = std::make_shared<BinaryOperation>(function, node, Term(lexer, symbolsTable));
    }
    return node;
}

spExpression Parser::Factor(Lexer& lexer, SymbolsTable& symbolsTable)
{
    Lexeme& lexeme = lexer.Top();
    if(lexeme.type == Token::Number)
    {
        auto constant = symbolsTable.CreateConstant(lexeme.Get<double>());
        lexeme = lexer.Pop();
        return constant;
    }
    else if(lexeme.type == Token::ParenOpen)
    {
        lexeme = lexer.Pop();
        auto expr = Expr(lexer, symbolsTable);
        lexeme = lexer.Pop();
        return expr;
    }
    else if(lexeme.type == Token::Minus)
    {
        lexeme = lexer.Pop();
        FunctionInfo function("-", Operations::unaryMinus);
        return std::make_shared<UnaryOperation>(function, Factor(lexer, symbolsTable));
    }
    else if(lexeme.type == Token::Id)
    {
        auto prev = lexeme;
        lexeme = lexer.Pop();
        if (auto expr = symbolsTable.FindArgument(prev.Get<std::string>()))
            return expr;

        if (auto expr = symbolsTable.FindConstant(prev.Get<std::string>()))
            return expr;

        if (auto expr = symbolsTable.FindVariable(prev.Get<std::string>()))
            return expr;
        
        // if (auto func = LangFunctions::FindFunction(prev.Get<std::string>()))
        //     return std::make_shared<FunctionExpression>(func, Term(lexer, symbolsTable));
    }
    return nullptr;
}