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

CheckedResult<double> Sqrt(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::sqrt(params[0]->GetValue());
}
CheckedResult<double> Abs(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::fabs(params[0]->GetValue());
}
CheckedResult<double> Sin(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::sin(params[0]->GetValue());
}
CheckedResult<double> Cos(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::cos(params[0]->GetValue());
}
CheckedResult<double> Tan(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::tan(params[0]->GetValue());
}
CheckedResult<double> ArcTan(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::atan(params[0]->GetValue());
}
CheckedResult<double> ArcSin(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::asin(params[0]->GetValue());
}
CheckedResult<double> ArcCos(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::acos(params[0]->GetValue());
}
CheckedResult<double> Cosh(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::cosh(params[0]->GetValue());
}
CheckedResult<double> Sinh(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::sinh(params[0]->GetValue());
}
CheckedResult<double> Tanh(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::tanh(params[0]->GetValue());
}
CheckedResult<double> Exp(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::exp(params[0]->GetValue());
}
CheckedResult<double> Ln(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::log(params[0]->GetValue());
}
CheckedResult<double> Log(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::log(params[0]->GetValue());
}
CheckedResult<double> Log10(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::log10(params[0]->GetValue());
}
CheckedResult<double> Log2(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::log2(params[0]->GetValue());
}
CheckedResult<double> Ceil(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::ceil(params[0]->GetValue());
}
CheckedResult<double> Floor(const std::vector<spExpression>& params)
{
    CheckParams(params, 1)
    return std::floor(params[0]->GetValue());
}
CheckedResult<double> Functions::Cut(const std::vector<spExpression>& params)
{
    CheckParams(params, 4)
    auto& var = params[0];
    auto& axis = params[1];
    auto& start = params[2];
    auto& end = params[3];

    /*
        var s1 = -(z - 0.5); // Верх
        var s2 = z + 0.5; // Снизу
        RETURN W & s1 & s2;
    */
    auto plane1 = (-axis->GetValue() + end->GetValue());
    auto plane2 = (axis->GetValue() - start->GetValue());
    return Operations::RvCross(var->GetValue(), Operations::RvCross(plane1, plane2));
}


std::vector<FunctionInfo<FunctionExpression::FuncType>> Functions::_functions
{
        { "abs" , &Abs, "abs(number)" },
        { "sqrt" , &Sqrt, "sqrt(number)" },
        { "sin", &Sin, "sin(number)" },
        { "cos" , &Cos, "cos(number)" },
        { "tan" , &Tan, "tan(number)" },
        { "arctan" , &ArcTan, "arctan(number)" },
        { "arcsin" , &ArcSin, "arcsin(number)" },
        { "arccos" , &ArcCos, "arccos(number)" },
        { "cosh" , &Cosh, "cosh(number)" },
        { "sinh" , &Sinh, "sinh(number)" },
        { "tanh" , &Tanh, "tanh(number)" },
        { "exp" , &Exp, "exp(number)" },
        { "ln" , &Ln, "ln(number)" },
        { "log" , &Log, "log(number)" },
        { "log10" , &Log10, "log10(number)" },
        { "log2" , &Log2, "log2(number)" },
        { "ceil" , &Ceil, "ceil(number)" },
        { "floor" , &Floor, "floor(number)" }
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
        if (func.name == name)
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
        if (func.Info().name == name)
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
