#include "Lexer.h"
#include <sstream>
#include <iostream>

using namespace std;


Lexer Lexer::CreateFrom(const std::string &code)
{
    Lexer lexer;
    lexer.Process(code);
    return lexer;
}

void Lexer::Process(const std::string &code, bool forceClean)
{
    if (code.empty())
        return;

    if (forceClean)
        while (!_lexemes.empty())
            _lexemes.pop();

    unsigned pivot = 0;
    size_t line = 1;
    while(_lexemes.empty() || _lexemes.back().Type() != Token::End)
    {
        _lexemes.push(NextLexeme(code, pivot, line));
        if (_lexemes.back().Type() == Token::Endline)
            ++line;
    }
}

Lexeme Lexer::NextLexeme(const std::string &data, unsigned int &pivot, size_t line)
{
    while (pivot < data.size() && isspace(data[pivot]))
        pivot++;

    if(data[pivot] == '/' && data[pivot+1] == '/')
    {
        while(pivot < data.size() && data[pivot] != '\n')
            pivot++;

        pivot++;
        return NextLexeme(data, pivot, line);
    }

    if(pivot >= data.size() || data[pivot] == '\0')
        return {Token::End, "End", line};

    if(data[pivot] == ';')
    {
        pivot++;
        return {Token::Endline, ';', line};
    }

    unsigned begin = pivot;
    if ((data[pivot+1] == '.')
        || isdigit (data[pivot])
        // allow decimal numbers without a leading 0. e.g. ".5"
        || (data[pivot] == '.' && isdigit (data[pivot+1])))
    {
        // skip sign for now
        if ((data[pivot] == '+' || data[pivot] == '-'))
            pivot++;

        while (isdigit (data[pivot]) || data[pivot] == '.')
            pivot++;

        // allow for 1.53158e+15
        if (data[pivot] == 'e' || data[pivot] == 'E')
        {
            pivot++; // skip 'e'
            if ((data[pivot]  == '+' || data[pivot]  == '-'))
                pivot++; // skip sign after e

            while (isdigit (data[pivot]))  // now digits after e
                pivot++;
        }

        double val;
        istringstream is (data.substr(begin, pivot - begin));
        is >> val;

        if (is.fail () && !is.eof ())
        {
            _error = "Unexpected eof";
            return {Token::End, "eof", line};
        }

        return {Token::Number, val, line};
    }

    char symbol = data[pivot++];
    switch (symbol)
    {
        case '=':
            return {Token::Assign, symbol, line};
        case '(':
            return {Token::ParenOpen, symbol, line};
        case ')':
            return {Token::ParenClose, symbol, line};
        case '[':
            return {Token::SquareBracketOpen, symbol, line};
        case ']':
            return {Token::SquareBracketClose, symbol, line};
        case '+':
            return {Token::Plus, symbol, line};
        case '-':
            return {Token::Minus, symbol, line};
        case '*':
            return {Token::Multiply, symbol, line};
        case '/':
        case '\\':
            return {Token::Divide, symbol, line};
        case '&':
            return {Token::Union, symbol, line};
        case '|':
            return {Token::Cross, symbol, line};
        case ',':
            return {Token::Comma, symbol, line};
        case '^':
            return {Token::Pow, symbol, line};
        case ':':
            return {Token::Colon, symbol, line};
    }
    pivot--;

    if (!isalpha (data[pivot]))
    {
        _error = "Unexpected symbol "+string(1, data[pivot]);
        return {Token::End, "eof", line};
    }

    // we have a word (starting with A-Z) - pull it out
    while (isalnum (data[pivot]) || data[pivot] == '_')
        pivot++;

    auto word = data.substr(begin, pivot-begin);
    return {Token::Id, word, line};
}

Lexeme Lexer::Pop(Token token)
{
    if (_lexemes.empty())
        return {Token::None, "None", 0};

    _lexemes.pop();

    if (_lexemes.empty())
        return {Token::None, "None", 0};

    if (token != Token::None && _lexemes.front().Type() != token)
        _error = "Error: Token is " + TokenToString(_lexemes.front().Type()) + "Expected " + TokenToString(token);

    return _lexemes.front();
}

bool Lexer::Empty() const
{
    return _lexemes.empty();
}

Lexeme Lexer::Top()
{
    if (!_lexemes.empty())
        return _lexemes.front();
    return {Token::None, "None", 0};
}

string Lexeme::ToPrintableString() const
{
    stringstream res;

    res << TokenToString(_type) << ": ";
    if (_isNumber)
        res << "(num)";
    res << _name;

    return res.str();
}
