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
    FunctionInfo() = default;

    FunctionInfo(const std::string& name, const std::vector<LanguageType>& params, LanguageReturnType returnType = LanguageReturnType{LanguageType::Double, 1}, const std::string descr = ""):
        name(name),
        desc(descr),
        params(params),
        returnType(returnType)
    {

    }

    bool operator<(const FunctionInfo& oth) const
    {
        return name < oth.name;
    }

    inline const std::string& Tags() const { return tags; }
    inline const std::string& Name() const { return name; }
    inline const std::string& Desc() const { return desc; }
    inline const std::vector<LanguageType>& Params() const { return params; }
    inline const LanguageReturnType ReturnType() const { return returnType; }


private:
    std::string tags;
    std::string name;
    std::string desc;
    std::vector<LanguageType> params;
    LanguageReturnType returnType;

    friend class CustomFunction;
};


#endif //RANOKLANGUAGE_FUNCTIONINFO_H
