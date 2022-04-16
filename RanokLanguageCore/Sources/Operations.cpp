//
// Created by Balalarga on 07.03.2022.
//

#include "Operations.h"
#include <cmath>
#include <iostream>


std::vector<FunctionInfo> Operations::_unaryOperations
{
    {"-", {LanguageType::Double}},
};

std::vector<FunctionInfo> Operations::_binaryOperations
{
    {"+", {LanguageType::Double, LanguageType::Double}},
    {"-", {LanguageType::Double, LanguageType::Double}},
    {"/", {LanguageType::Double, LanguageType::Double}},
    {"*", {LanguageType::Double, LanguageType::Double}},
    {"^", {LanguageType::Double, LanguageType::Double}},
    {"|", {LanguageType::Double, LanguageType::Double}},
    {"&", {LanguageType::Double, LanguageType::Double}}
};

FunctionInfo* Operations::UnaryFromString(const std::string& name)
{
    for (auto& i: _unaryOperations)
        if (i.Name() == name)
            return &i;
    return nullptr;
}

FunctionInfo* Operations::BinaryFromString(const std::string& name)
{
    for (auto& i: _binaryOperations)
        if (i.Name() == name)
            return &i;
    return nullptr;
}


const std::vector<FunctionInfo>& Operations::GetBinaries()
{
    return _binaryOperations;
}

const std::vector<FunctionInfo>& Operations::GetUnaries()
{
    return _unaryOperations;
}
