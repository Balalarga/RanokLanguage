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
        { "abs" , {LanguageType::Double}, {LanguageType::Double}, "abs(number) -> number" },
        { "sqrt" , {LanguageType::Double}, {LanguageType::Double}, "sqrt(number) -> number" },
        { "sin", {LanguageType::Double}, {LanguageType::Double}, "sin(number) -> number" },
        { "tan" , {LanguageType::Double}, {LanguageType::Double}, "tan(number) -> number" },
        { "cos" , {LanguageType::Double}, {LanguageType::Double}, "cos(number) -> number" },
        { "arctan", {LanguageType::Double}, {LanguageType::Double}, "arctan(number) -> number" },
        { "arcsin", {LanguageType::Double}, {LanguageType::Double}, "arcsin(number) -> number" },
        { "arccos", {LanguageType::Double}, {LanguageType::Double}, "arccos(number) -> number" },
        { "cosh", {LanguageType::Double}, {LanguageType::Double}, "cosh(number) -> number" },
        { "sinh", {LanguageType::Double}, {LanguageType::Double}, "sinh(number) -> number" },
        { "tanh", {LanguageType::Double}, {LanguageType::Double}, "tanh(number) -> number" },
        { "exp", {LanguageType::Double}, {LanguageType::Double}, "exp(number) -> number" },
        { "ln", {LanguageType::Double}, {LanguageType::Double}, "ln(number) -> number" },
        { "log", {LanguageType::Double}, {LanguageType::Double}, "log(number) -> number" },
        { "log10", {LanguageType::Double},{LanguageType::Double}, "log10(number) -> number" },
        { "log2", {LanguageType::Double}, {LanguageType::Double}, "log2(number) -> number" },
        { "ceil", {LanguageType::Double}, {LanguageType::Double}, "ceil(number) -> number" },
        { "floor", {LanguageType::Double}, {LanguageType::Double}, "floor(number) -> number" }
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

std::map<string, std::vector<CustomFunction*>> Functions::GetTagedCustomFuncs()
{
    std::map<std::string, std::vector<CustomFunction*>> _tagedFuncs;
    for (CustomFunction& f: _customFunctions)
    {
        if (_tagedFuncs.find(f.Info().Tags()) == _tagedFuncs.end())
            _tagedFuncs[f.Info().Tags()] = {&f};
        else
            _tagedFuncs[f.Info().Tags()].push_back(&f);
    }
    return _tagedFuncs;
}
