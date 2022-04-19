//
// Created by Balalarga on 06.03.2022.
//

#include "Parser.h"
#include "Operations.h"
#include "Functions.h"

#include <iostream>

#if 0
    #define Debug(msg) cout << msg << endl;
#else
    #define Debug(msg)
#endif

static unsigned CurrentArrayVarSize = 0;

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
    while (!_lexer->Empty() && lexeme.Type() != Token::End && lexeme.Type() != Token::None)
    {
        if (lexeme.Type() == Token::Id)
        {
            if (CaseCompare(lexeme.Name(), "args", true))
            {
                HandleArgument();
            }
            else if(CaseCompare(lexeme.Name(), "return", true))
            {
                LexerCheckedPop();
                program.Init(HandleReturn());
            }
            else if (CaseCompare(lexeme.Name(), "var", true))
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
            _error = "Unknown token " + lexeme.Name() + " at " + std::to_string(lexeme.Line()) + " line";
            break;
        }
        if (!_lexer->Error().empty())
        {
            _error = _lexer->Error() + " at " + std::to_string(lexeme.Line()) + " line";
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
        _error = "Lexer is empty";
        return {Token::None, "NONE", 0};
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
        _error = "Lexer is empty";
        return {Token::None, "NONE", 0};
    }
}

void Parser::HandleArgument()
{
    Lexeme lexeme = LexerCheckedTop();
    spExpression expr;
    while (lexeme.Type() != Token::Endline)
    {
        std::vector<Range> ranges(1);

        lexeme = LexerCheckedPop(Token::Id);
        std::string name = lexeme.Name();
        lexeme = LexerCheckedPop();
        bool isArray = false;
        if (lexeme.Type() == Token::SquareBracketOpen)
        {
            isArray = true;
            lexeme = LexerCheckedPop();
            if (lexeme.IsNumber())
            {
                ranges.resize(static_cast<unsigned>(lexeme.Value()));
                lexeme = LexerCheckedPop();
            }
            lexeme = LexerCheckedPop();
        }

        if (lexeme.Type() == Token::ParenOpen)
        {
            lexeme = LexerCheckedPop();
            if (isArray)
            {
                size_t counter = 0;
                while (lexeme.Type() != Token::ParenClose && counter < ranges.size())
                {
                    bool negative = false;
                    if (lexeme.Type() == Token::Minus)
                    {
                        negative = true;
                        lexeme = LexerCheckedPop(Token::Number);
                    }
                    ranges[counter].min = lexeme.Value();
                    lexeme = LexerCheckedPop();
                    if (lexeme.Type() == Token::Comma)
                    {
                        ++counter;   
                        lexeme = LexerCheckedPop();
                    }
                }
            }
            else
            {
                bool negative = false;
                if (lexeme.Type() == Token::Minus)
                {
                    negative = true;
                    lexeme = LexerCheckedPop(Token::Number);
                }
                ranges[0].min = lexeme.Value();
                lexeme = LexerCheckedPop();
                if (lexeme.Type() == Token::Comma)
                {
                    ranges[0].min = negative ? -ranges[0].min : ranges[0].min;
                    lexeme = LexerCheckedPop();
                    negative = false;
                    if (lexeme.Type() == Token::Minus)
                    {
                        negative = true;
                        lexeme = LexerCheckedPop(Token::Number);
                    }
                    ranges[0].max = negative ? -lexeme.Value() : lexeme.Value();
                    LexerCheckedPop();
                }
                else
                {
                    ranges[0].max = ranges[0].min;
                    ranges[0].min = -ranges[0].min;
                }
            }
            lexeme = LexerCheckedPop();
        }

        if (isArray)
            _program->Table().CreateArgument(name, ranges);
        else
            _program->Table().CreateArgument(name, ranges[0]);
    }
}

void Parser::HandleVariable()
{
    Lexeme lexeme = LexerCheckedTop();
    std::string name = lexeme.Name();
    lexeme = LexerCheckedPop();
    if (lexeme.Type() == Token::SquareBracketOpen)
    {
        lexeme = LexerCheckedPop(Token::Number);
        CurrentArrayVarSize = static_cast<unsigned>(lexeme.Value());
        lexeme = LexerCheckedPop(Token::SquareBracketClose);
        lexeme = LexerCheckedPop();
    }
    else
    {
        CurrentArrayVarSize = 0;
    }
    lexeme = LexerCheckedPop();

    _program->Table().CreateVariable(name, Expr());
}

spExpression Parser::HandleReturn()
{
    Lexeme lexeme = LexerCheckedTop();
    spExpression expr = Expr();
    return expr;
}

spExpression Parser::HandleArray()
{
    std::vector<spExpression> values;

    while (LexerCheckedTop().Type() != Token::BraceClose)
    {
        values.push_back(Expr());
        if (LexerCheckedTop().Type() == Token::Comma)
            LexerCheckedPop();
    }

    return spArrayExpression(new ArrayExpression(values));
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
        LexerCheckedPop();
        auto binaryInfo = Operations::BinaryFromString(lexeme.Name());
        node = std::make_shared<BinaryOperation>(*binaryInfo, node, Term());
        lexeme = LexerCheckedTop();
    }
    return node;
}

bool Parser::TryParseArrayGetter(spExpression& node)
{
    auto var = std::dynamic_pointer_cast<VariableExpression>(node);
    if (!var)
        return false;

    Lexeme lexeme = LexerCheckedPop();
    if (lexeme.IsNumber())
    {
        LexerCheckedPop();
        unsigned id = static_cast<unsigned>(lexeme.Value());
        if (auto expr = std::dynamic_pointer_cast<ArrayExpression>(var->child))
        {
            node = std::make_shared<ArrayGetterExpression>(var, id);
        }
        else if(auto expr = std::dynamic_pointer_cast<FunctionExpression>(var->child))
        {
            if (expr->function.ReturnType().Type == LanguageType::DoubleArray)
                node = std::make_shared<ArrayGetterExpression>(var, id);
        }
    }
    LexerCheckedPop();
    return true;
}

spExpression Parser::Term()
{
    spExpression node = Factor();
    if (LexerCheckedTop().Type() == Token::SquareBracketOpen)
        TryParseArrayGetter(node);
    
    Lexeme lexeme = LexerCheckedTop();
    while (lexeme.Type() == Token::Pow      ||
           lexeme.Type() == Token::Multiply ||
           lexeme.Type() == Token::Divide)
    {
        LexerCheckedPop();
        auto binaryInfo = Operations::BinaryFromString(lexeme.Name());
        node = std::make_shared<BinaryOperation>(*binaryInfo, node, Term());
        lexeme = LexerCheckedTop();
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
    else if (lexeme.Type() == Token::BraceOpen)
    {
        LexerCheckedPop();
        auto expr = HandleArray();
        LexerCheckedPop();
        return expr;
    }
    else if(lexeme.Type() == Token::Minus)
    {
        LexerCheckedPop();
        auto unaryInfo = Operations::UnaryFromString(lexeme.Name());
        return std::make_shared<UnaryOperation>(*unaryInfo, Factor());
    }
    else if(lexeme.Type() == Token::Id)
    {
        Lexeme prev = lexeme;
        std::string name = lexeme.Name();
        LexerCheckedPop();
        if (auto expr = _program->Table().FindArgument(name))
            return expr;

        if (auto expr = _program->Table().FindConstant(name))
            return expr;

        if (auto func = Functions::Find(name))
        {
            if (func->ReturnType().Count != CurrentArrayVarSize)
            {
                _error = "Array size error near function " + name;
                return nullptr;
            }

            std::vector<spExpression> args;
            HandleFunctionArgs(args);
            return std::make_shared<FunctionExpression>(*func, args);
        }

        if (auto func = Functions::FindCustom(name))
        {
            if (func->Info().ReturnType().Count != CurrentArrayVarSize)
            {
                _error = "Array size error near function " + name;
                return nullptr;
            }

            std::vector<spExpression> args;
            HandleFunctionArgs(args);
            return std::make_shared<CustomFunctionExpression>(func->Info(), func->Root(), args);
        }

        if (auto expr = _program->Table().FindVariable(name))
        {
            return expr;
        }
    }
    return nullptr;
}

void Parser::HandleFunctionArgs(std::vector<spExpression>& args)
{
    Lexeme lexeme = LexerCheckedPop();
    while (lexeme.Type() != Token::ParenClose)
    {
        args.push_back(Expr());
        lexeme = LexerCheckedTop();
        if (lexeme.Type() == Token::Comma)
            lexeme = LexerCheckedPop();
    }
    LexerCheckedPop();
}

void Parser::HandleFunctionArgs()
{
    Lexeme lexeme = LexerCheckedPop(Token::ParenOpen);
    while (lexeme.Type() != Token::ParenClose)
    {
        Expr();
        lexeme = LexerCheckedTop();
        if (lexeme.Type() == Token::Comma)
            lexeme = LexerCheckedPop();
    }
    LexerCheckedPop(Token::ParenClose);
}
