//
// Created by Balalarga on 05.03.2022.
//

#ifndef RANOKLANGUAGE_FUNCTIONINFO_H
#define RANOKLANGUAGE_FUNCTIONINFO_H

#include <string>
#include <functional>
#include <type_traits>


template<class T>
struct FunctionInfo: public std::function<T>
{
    FunctionInfo(const std::string& name, std::function<T> function):
        std::function<T>(function),
        name(name)
    {

    }

    const std::string name;
};

template<class T>
struct CustomFunctionInfo: public FunctionInfo<T>
{
    CustomFunctionInfo(const std::string& name, std::function<T> function, spExpression root):
        FunctionInfo(name, function),
        root(root)
    {
        
    }
    
    spExpression root;
};

#endif //RANOKLANGUAGE_FUNCTIONINFO_H
