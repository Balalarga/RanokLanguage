//
// Created by Balalarga on 05.03.2022.
//

#ifndef RANOKLANGUAGE_FUNCTIONINFO_H
#define RANOKLANGUAGE_FUNCTIONINFO_H

#include <string>
#include <functional>


template<class T>
struct FunctionInfo
{
    FunctionInfo(const std::string& name, const std::string descr = ""):
        name(name),
        desc(descr)
    {

    }

    bool operator<(const FunctionInfo<T>& oth) const 
    {
        return name < oth.name;
    }


    inline const std::string& Name() const { return name; }
    inline const std::string& Desc() const { return desc; }


private:
    std::string name;
    std::string desc;

    friend class CustomFunction;
};


#endif //RANOKLANGUAGE_FUNCTIONINFO_H
