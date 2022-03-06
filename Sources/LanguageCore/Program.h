//
// Created by Balalarga on 06.03.2022.
//

#ifndef RANOKLANGUAGE_PROGRAM_H
#define RANOKLANGUAGE_PROGRAM_H

#include "Expression.h"


class Program
{
public:
    Program() = default;

    void Init(spExpression begin);

    double Process();

    inline const std::vector<spArgumentExpression>& Arguments() { return _arguments; }
    inline const std::vector<spVariableExpression>& Variables() { return _variables; }


private:
    spExpression _begin;
    std::vector<spArgumentExpression> _arguments;
    std::vector<spVariableExpression> _variables;
};


#endif //RANOKLANGUAGE_PROGRAM_H
