#ifndef LEXER_H
#define LEXER_H

#include "Ranok/Utility/CheckedResult.h"
#include "Tokens.h"

#include <queue>


struct Lexeme
{
    Lexeme(const Token& token, std::string name, size_t line):
        _type(token),
        _name(name),
        _value(0),
        _line(line),
        _isNumber(false) {}
    Lexeme(const Token& token, char name, size_t line):
        _type(token),
        _name(std::string(1, name)),
        _value(0),
        _line(line),
        _isNumber(false) {}

    Lexeme(const Token& token, double value, size_t line):
        _type(token),
        _name(std::to_string(value)),
        _value(value),
        _line(line),
        _isNumber(true) {}

    std::string ToPrintableString() const;

    inline const Token& Type() { return _type; }
    inline const std::string& Name() { return _name; }
    inline const double& Value() { return _value; }
    inline const bool& IsNumber() { return _isNumber; }
    inline const size_t& Line() { return _line; }


private:
    Token _type;
    std::string _name;
    double _value;
    bool _isNumber;
    size_t _line;
};


class Lexer
{
public:
    Lexer() = default;

    static Lexer CreateFrom(const std::string& code);

    void Process(const std::string& code, bool forceClean = true);

    inline const std::string& Error() const { return _error; }

    Lexeme Top();
    Lexeme Pop(Token token = Token::None);
    bool Empty() const;


protected:
    Lexeme NextLexeme(const std::string& data, unsigned& pivot, size_t line);


private:
    std::queue<Lexeme> _lexemes;
    std::string _error;
};

#endif // LEXER_H
