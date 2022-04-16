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

std::vector<FunctionInfo> Functions::_functions
{
        { "abs" , {LanguageType::Double}, "abs(number)" },
        { "sqrt" , {LanguageType::Double}, "sqrt(number)" },
        { "sin", {LanguageType::Double}, "sin(number)" },
        { "tan" , {LanguageType::Double}, "tan(number)" },
        { "cos" , {LanguageType::Double}, "cos(number)" },
        { "arctan", {LanguageType::Double}, "arctan(number)" },
        { "arcsin", {LanguageType::Double}, "arcsin(number)" },
        { "arccos", {LanguageType::Double}, "arccos(number)" },
        { "cosh", {LanguageType::Double}, "cosh(number)" },
        { "sinh", {LanguageType::Double}, "sinh(number)" },
        { "tanh", {LanguageType::Double}, "tanh(number)" },
        { "exp", {LanguageType::Double}, "exp(number)" },
        { "ln", {LanguageType::Double}, "ln(number)" },
        { "log", {LanguageType::Double}, "log(number)" },
        { "log10", {LanguageType::Double}, "log10(number)" },
        { "log2", {LanguageType::Double}, "log2(number)" },
        { "ceil", {LanguageType::Double}, "ceil(number)" },
        { "floor", {LanguageType::Double}, "floor(number)" }
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

FunctionInfo* Functions::Find(const std::string &name)
{
    for (auto& func : _functions)
        if (func.Name() == name)
            return &func;
    return nullptr;
}

const std::vector<FunctionInfo>& Functions::GetAll()
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
