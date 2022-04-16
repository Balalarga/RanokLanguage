//
// Created by Balalarga on 07.03.2022.
//

#include "Functions.h"
#include "Operations.h"
#include "Ranok/Utility/StringUtility.h"
#include "Ranok/Utility/CheckedResult.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

#define CheckParams(params, count) \
    if (params.size() != count) \
    { \
        cout << __FUNCTION__ << ": parameters error(must be " << count << ")\n"; \
        return false; \
    }

std::vector<FunctionInfo<FunctionExpression::FuncType>> Functions::_functions
{
        { "abs" , "abs(number)" },
        { "sqrt" , "sqrt(number)" },
        { "sin", "sin(number)" },
        { "cos" , "cos(number)" },
        { "tan" , "tan(number)" },
        { "arctan", "arctan(number)" },
        { "arcsin", "arcsin(number)" },
        { "arccos", "arccos(number)" },
        { "cosh", "cosh(number)" },
        { "sinh", "sinh(number)" },
        { "tanh", "tanh(number)" },
        { "exp", "exp(number)" },
        { "ln", "ln(number)" },
        { "log", "log(number)" },
        { "log10", "log10(number)" },
        { "log2", "log2(number)" },
        { "ceil", "ceil(number)" },
        { "floor", "floor(number)" }
};

std::vector<CustomFunction> Functions::_customFunctions
{

};

size_t InitialCustomFunctionsSize = 0;


void Functions::InitCustomsFrom(const std::string &filepath)
{
    std::ifstream funcsFile(filepath);
    if (funcsFile)
    {
        int startId = 0;
        std::stringstream dataStream;
        dataStream << funcsFile.rdbuf();
        std::string data = StringUtility::Trim(dataStream.str());
        while (startId < data.size())
        {
            std::string cutted = data.substr(startId);
            Functions::AddCustom(CustomFunction::FromString(cutted, startId));
        }

        funcsFile.close();
    }
    InitialCustomFunctionsSize = _customFunctions.size();
}

void Functions::DumpCustomsOnDemandTo(const std::string &filepath)
{
    if (InitialCustomFunctionsSize == _customFunctions.size())
        return;

    std::ofstream file(filepath);
    if (file)
    {
        for (auto& func: _customFunctions)
            file << CustomFunction::ToString(func) << "\n";

        file.close();
    }
}

FunctionInfo<FunctionExpression::FuncType>* Functions::Find(const std::string &name)
{
    for (auto& func : _functions)
        if (func.Name() == name)
            return &func;
    return nullptr;
}

const std::vector<FunctionInfo<FunctionExpression::FuncType>>& Functions::GetAll()
{
    return _functions;
}

CustomFunction* Functions::FindCustom(const std::string& name)
{
    for (auto& func : _customFunctions)
        if (func.Info().Name() == name)
            return &func;
    return nullptr;
}

const std::vector<CustomFunction>& Functions::GetAllCustoms()
{
    return _customFunctions;
}

void Functions::AddCustom(const CustomFunction& function)
{
    if (!FindCustom(function.Name()))
        _customFunctions.push_back(function);
}
