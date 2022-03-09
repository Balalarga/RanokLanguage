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
    Lexeme lexeme = LexerCheckedTop();
    while (!_lexer->Empty() && lexeme.Type() != Token::End)
    {
        std::string tokenName;
        tokenName = lexeme.Name();

        if (lexeme.Type() == Token::Id)
        {
            if (CaseCompare(tokenName, "argument") ||
                CaseCompare(tokenName, "arguments") ||
                CaseCompare(tokenName, "args"))
            {
                HandleArgument();
            }
            else if (CaseCompare(tokenName, "constant") ||
                     CaseCompare(tokenName, "const"))
            {
                HandleConstant();
            }
            else if(CaseCompare(tokenName, "return"))
            {
                LexerCheckedPop();
                program.Init(HandleReturn());
            }
            else if (CaseCompare(tokenName, "variable") ||
                     CaseCompare(tokenName, "var"))
            {
                LexerCheckedPop();
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
            break;
        }
        lexeme = LexerCheckedPop();
    }
    _program = nullptr;
    _lexer = nullptr;
    return program;
}

Lexeme Parser::LexerCheckedPop(Token token)
{
    if (_lexer && !_lexer->Empty())
    {
        return _lexer->Pop(token);
    }
    else
    {
        std::cout << "Lexer is empty\n";
        return {Token::None, "NONE"};
    }
}
Lexeme Parser::LexerCheckedTop()
{
    if (_lexer)
    {
        return _lexer->Top();
    }
    else
    {
        std::cout << "Lexer is empty\n";
        return {Token::None, "NONE"};
    }
}

void Parser::HandleArgument()
{
    Lexeme lexeme = LexerCheckedTop();
    spExpression expr;
    while (lexeme.Type() != Token::Endline)
    {
        ArgumentExpression::Range range{-5, 5};

        lexeme = LexerCheckedPop(Token::Id);
        std::string name = lexeme.Name();
        LexerCheckedPop(Token::ParenOpen);
        bool negative = false;
        lexeme = LexerCheckedPop();
        if (lexeme.Type() == Token::Minus)
        {
            negative = true;
            lexeme = LexerCheckedPop(Token::Number);
        }
        range.min = lexeme.Value();
        lexeme = LexerCheckedPop();
        if (lexeme.Type() == Token::Comma)
        {
            range.min = negative ? -range.min : range.min;
            lexeme = LexerCheckedPop();
            negative = false;
            if (lexeme.Type() == Token::Minus)
            {
                negative = true;
                lexeme = LexerCheckedPop(Token::Number);
            }
            range.max = negative ? -lexeme.Value() : lexeme.Value();
            LexerCheckedPop();
        }
        else
        {
            range.max = range.min;
            range.min = -range.min;
        }
        lexeme = LexerCheckedPop();
        _program->Table().CreateArgument(name, range);
    }
}


void Parser::HandleConstant()
{
    Lexeme lexeme = LexerCheckedTop();
    while(lexeme.Type() != Token::Endline)
    {
        lexeme = LexerCheckedPop(Token::Id);
        std::string name = lexeme.Name();
        LexerCheckedPop(Token::Assign);
        lexeme = LexerCheckedPop();
        Expr();
    }
    LexerCheckedPop();
}

void Parser::HandleVariable()
{
    Lexeme lexeme = LexerCheckedTop();
    std::string name = lexeme.Name();
    LexerCheckedPop(Token::Assign);
    LexerCheckedPop();
    _program->Table().CreateVariable(name, Expr());
}

spExpression Parser::HandleReturn()
{
    Lexeme lexeme = LexerCheckedTop();
    spExpression expr = Expr();
    LexerCheckedPop();
    return expr;
}

spExpression Parser::Expr()
{
    spExpression node = Term();
    Lexeme lexeme = LexerCheckedTop();
    while (lexeme.Type() == Token::Minus ||
           lexeme.Type() == Token::Plus  ||
           lexeme.Type() == Token::Cross ||
           lexeme.Type() == Token::Union)
    {
        auto prev = lexeme;
        lexeme = LexerCheckedPop();
        FunctionInfo function(prev.Name(), Operations::FromString(prev.Name()));
        node = std::make_shared<BinaryOperation>(function, node, Term());
    }
    return node;
}

spExpression Parser::Term()
{
    spExpression node = Factor();
    Lexeme lexeme = LexerCheckedTop();
    while (lexeme.Type() == Token::Pow      ||
           lexeme.Type() == Token::Multiply ||
           lexeme.Type() == Token::Divide)
    {
        auto prev = lexeme;
        lexeme = LexerCheckedPop();
        FunctionInfo function(prev.Name(), Operations::FromString(prev.Name()));
        node = std::make_shared<BinaryOperation>(function, node, Term());
    }
    return node;
}

spExpression Parser::Factor()
{
    Lexeme lexeme = LexerCheckedTop();
    if(lexeme.Type() == Token::Number)
    {
        auto constant = _program->Table().CreateConstant(lexeme.Value());
        LexerCheckedPop();
        return constant;
    }
    else if(lexeme.Type() == Token::ParenOpen)
    {
        LexerCheckedPop();
        auto expr = Expr();
        LexerCheckedPop();
        return expr;
    }
    else if(lexeme.Type() == Token::Minus)
    {
        LexerCheckedPop();
        FunctionInfo function("-", Operations::unaryMinus);
        return std::make_shared<UnaryOperation>(function, Factor());
    }
    else if(lexeme.Type() == Token::Id)
    {
        auto prev = lexeme;
        LexerCheckedPop();
        if (auto expr = _program->Table().FindArgument(prev.Name()))
            return expr;

        if (auto expr = _program->Table().FindConstant(prev.Name()))
            return expr;

        // if (auto func = LangFunctions::FindFunction(prev.name))
        //     return std::make_shared<FunctionExpression>(func, Term());

        if (auto expr = _program->Table().FindVariable(prev.Name()))
            return expr;
    }
    return nullptr;
}
