//
// Created by Balalarga on 06.03.2022.
//

#include "Parser.h"
#include "Operations.h"
#include "Functions.h"

#include <iostream>
#include <fmt/format.h>

#if 0
    #define Debug(msg) std::cout << "[Debug] " << msg << std::endl;
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
    Debug("------------------Begin program--------------------");
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
                Debug(fmt::format("Process args"));
                HandleArgument();
            }
            else if(CaseCompare(lexeme.Name(), "return", true))
            {
                Debug(fmt::format("Process return"));
                LexerCheckedPop();
                program.Init(HandleReturn());
            }
            else if (CaseCompare(lexeme.Name(), "var", true))
            {
                Debug(fmt::format("Process variable"));
                LexerCheckedPop();
                HandleVariable();
            }
            else
            {
                Debug(fmt::format("Process variable"));
                HandleVariable();
            }
        }
        else
        {
            _error = "Unknown token " + lexeme.Name() + " at " + std::to_string(lexeme.Line()) + " line";
            Debug(fmt::format("Lexer error: {}", _error));
            break;
        }
        if (!_lexer->Error().empty())
        {
            _error = _lexer->Error() + " at " + std::to_string(lexeme.Line()) + " line";
            Debug(fmt::format("Lexer error: {}", _error));
            break;
        }

        lexeme = LexerCheckedPop();
    }
    _program = nullptr;
    _lexer = nullptr;
    Debug("-------------------End program---------------------");
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
        {
            Debug(fmt::format("Create argument array: {}", name));
            _program->Table().CreateArgument(name, ranges);
        }
        else
        {
            Debug(fmt::format("Create argument: {}", name));
            _program->Table().CreateArgument(name, ranges[0]);
        }
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

    Debug(fmt::format("Create variable: {}", name));
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
        Debug(fmt::format("Expr: {}", TokenToString(lexeme.Type())));
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
        Debug(fmt::format("Term: {}", TokenToString(lexeme.Type())));
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
        Debug(fmt::format("Constant: {}", constant->name));
        return constant;
    }
    else if(lexeme.Type() == Token::ParenOpen)
    {
        LexerCheckedPop();
        Debug("Begin Paren");
        auto expr = Expr();
        LexerCheckedPop();
        Debug(fmt::format("End Paren: {}", TokenToString(LexerCheckedTop().Type())));
        return expr;
    }
    else if (lexeme.Type() == Token::BraceOpen)
    {
        LexerCheckedPop();
        Debug("Begin Brace");
        auto expr = HandleArray();
        LexerCheckedPop();
        Debug(fmt::format("End Brace: {}", TokenToString(LexerCheckedTop().Type())));
        return expr;
    }
    else if(lexeme.Type() == Token::Minus)
    {
        LexerCheckedPop();
        auto unaryInfo = Operations::UnaryFromString(lexeme.Name());
        Debug(fmt::format("Unary minus: {}", unaryInfo->Name()));
        return std::make_shared<UnaryOperation>(*unaryInfo, Factor());
    }
    else if(lexeme.Type() == Token::Id)
    {
        Lexeme prev = lexeme;
        std::string name = lexeme.Name();
        LexerCheckedPop();
        if (auto expr = _program->Table().FindArgument(name))
        {
            Debug(fmt::format("Argument: {}", expr->name));
            return expr;
        }

        if (auto expr = _program->Table().FindConstant(name))
        {
            Debug(fmt::format("Constant: {}", expr->name));
            return expr;
        }

        if (auto func = Functions::Find(name))
        {
            if (func->ReturnType().Type == LanguageType::DoubleArray && func->ReturnType().Count != CurrentArrayVarSize)
            {
                _error = "Array size error near function " + name;
                return nullptr;
            }

            std::vector<spExpression> args;
            HandleFunctionArgs(args);
            Debug(fmt::format("Function: {}", func->Name()));
            return std::make_shared<FunctionExpression>(*func, args);
        }

        if (auto func = Functions::FindCustom(name))
        {
            if (func->Info().ReturnType().Type == LanguageType::DoubleArray && func->Info().ReturnType().Count != CurrentArrayVarSize)
            {
                _error = "Array size error near function " + name;
                return nullptr;
            }

            std::vector<spExpression> args;
            HandleFunctionArgs(args);
            Debug(fmt::format("CustomFunction: {}", func->Info().Name()));
            return std::make_shared<CustomFunctionExpression>(func->Info(), func->Root(), args);
        }

        if (auto expr = _program->Table().FindVariable(name))
        {
            Debug(fmt::format("Variable: {}", expr->name));
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
