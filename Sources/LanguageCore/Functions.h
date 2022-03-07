//
// Created by Balalarga on 07.03.2022.
//

#ifndef RANOKLANGUAGE_FUNCTIONS_H
#define RANOKLANGUAGE_FUNCTIONS_H

#include <map>
#include <string>
#include <functional>


class Functions
{
public:
    Functions() = delete;
    static std::map<std::string, double(double)> unaryFuncs;
    static std::map<std::string, double(double, double)> binaryFuncs;
};

#endif //RANOKLANGUAGE_FUNCTIONS_H
