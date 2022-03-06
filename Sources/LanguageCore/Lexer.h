#ifndef LEXER_H
#define LEXER_H

#include "Tokens.h"
#include <queue>
#include <variant>

struct Lexeme
{
    Lexeme(const Token& token, const std::string& name):
        token(token),
        value(name){}
    Lexeme(const Token& token, const char& name):
            token(token),
            value(std::string(1, name)){}
    Lexeme(const Token& token, const double& value):
            token(token),
            value(value){}

    Token token;
    std::variant<std::string, double> value;
};

class Lexer
{
public:
    Lexer() = default;

    void Process(const std::string& code, bool forceClean = true);
    std::string GetError() const;

    const Lexeme& Top() const;
    Lexeme Pop();
    bool Empty() const;

protected:
    virtual Lexeme NextLexeme(const std::string& data, unsigned& pivot);


private:
    std::queue<Lexeme> _lexemes;
    std::string _error;
};

#endif // LEXER_H
