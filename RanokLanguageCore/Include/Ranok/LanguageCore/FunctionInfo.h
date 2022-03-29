//
// Created by Balalarga on 05.03.2022.
//

#ifndef RANOKLANGUAGE_FUNCTIONINFO_H
#define RANOKLANGUAGE_FUNCTIONINFO_H

#include <string>
#include <functional>


template<class T>
struct FunctionInfo: public std::function<T>
{
    FunctionInfo(const std::string& name, std::function<T> function, const std::string descr = ""):
        std::function<T>(function),
        name(name),
        desc(descr)
    {

    }

    bool operator<(const FunctionInfo<T>& oth) const 
    {
        return name < oth.name;
    }

    const std::string name;
    const std::string desc;
};


#endif //RANOKLANGUAGE_FUNCTIONINFO_H
