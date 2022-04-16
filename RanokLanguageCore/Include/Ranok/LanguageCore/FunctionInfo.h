//
// Created by Balalarga on 05.03.2022.
//

#ifndef RANOKLANGUAGE_FUNCTIONINFO_H
#define RANOKLANGUAGE_FUNCTIONINFO_H

#include <string>
#include <vector>
#include "Types.h"


struct FunctionInfo
{
    FunctionInfo(const std::string& name, const std::vector<LanguageType>& params, const std::string descr = ""):
        name(name),
        desc(descr),
        params(params)
    {

    }

    bool operator<(const FunctionInfo& oth) const
    {
        return name < oth.name;
    }


    inline const std::string& Name() const { return name; }
    inline const std::string& Desc() const { return desc; }
    inline const std::vector<LanguageType>& Params() const { return params; }


private:
    std::string name;
    std::string desc;
    std::vector<LanguageType> params;

    friend class CustomFunction;
};


#endif //RANOKLANGUAGE_FUNCTIONINFO_H
