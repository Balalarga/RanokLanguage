//
// Created by Balalarga on 05.03.2022.
//

#ifndef RANOKLANGUAGE_FUNCTIONINFO_H
#define RANOKLANGUAGE_FUNCTIONINFO_H

#include <string>
#include <functional>
#include <type_traits>


template<class T>
struct FunctionInfo
{
    const std::string name;
    const std::function<T> function;
};

#endif //RANOKLANGUAGE_FUNCTIONINFO_H
