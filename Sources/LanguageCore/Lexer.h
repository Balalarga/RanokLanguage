#ifndef LEXER_H
#define LEXER_H

#include "Utils.h"
#include "Tokens.h"

#include <queue>


struct Lexeme: public Variant<std::string, double>
{
    Lexeme(const Token& token, std::string name):
        Variant<std::string, double>(name),
        type(token) {}
    Lexeme(const Token& token, char name):
        Variant<std::string, double>(std::string(1, name)),
        type(token) {}
    Lexeme(const Token& token, double value):
        Variant<std::string, double>(value),
        type(token) {}


    std::string ToPrintableString() const;


    Token type;
};


class Lexer
{
public:
    Lexer() = default;

    static Lexer CreateFrom(const std::string& code);

    void Process(const std::string& code, bool forceClean = true);

    std::string GetError() const;

    Lexeme& Top();
    Lexeme& Pop(Token token = Token::None);
    bool Empty() const;


protected:
    Lexeme NextLexeme(const std::string& data, unsigned& pivot);


private:
    std::queue<Lexeme> _lexemes;
    std::string _error;
};

#endif // LEXER_H
