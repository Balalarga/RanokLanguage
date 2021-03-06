//
// Created by Balalarga on 07.03.2022.
//

#ifndef RANOKLANGUAGE_FUNCTIONS_H
#define RANOKLANGUAGE_FUNCTIONS_H

#include <vector>
#include <map>

#include "CustomFunction.h"


class Functions
{
public:
    Functions() = delete;

    static void InitCustomsFrom(const std::string& filepath);
    static void DumpCustomsOnDemandTo(const std::string& filepath);

    static FunctionInfo* Find(const std::string& name);
    static const std::vector<FunctionInfo>& GetAll();

    static CustomFunction* FindCustom(const std::string& name);
    static const std::vector<CustomFunction>& GetAllCustoms();
    static void AddCustom(const CustomFunction& function);

    static CheckedResult<double> Cut(const std::vector<spExpression>& params);

    static std::map<std::string, std::vector<CustomFunction*>> GetTagedCustomFuncs();


private:
    static std::vector<FunctionInfo> _functions;
    static std::vector<CustomFunction> _customFunctions;
};

#endif //RANOKLANGUAGE_FUNCTIONS_H
