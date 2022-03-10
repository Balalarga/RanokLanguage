//
// Created by Balalarga on 07.03.2022.
//

#ifndef RANOKLANGUAGE_FUNCTIONS_H
#define RANOKLANGUAGE_FUNCTIONS_H

#include <vector>

#include "FunctionInfo.h"
#include "Expression.h"


class Functions
{
public:
    Functions() = delete;

    static FunctionInfo<FunctionExpression::FuncType>* Find(const std::string& name);
    static const std::vector<FunctionInfo<FunctionExpression::FuncType>>& GetAll();

private:
    static std::vector<FunctionInfo<FunctionExpression::FuncType>> _functions;
};

#endif //RANOKLANGUAGE_FUNCTIONS_H
