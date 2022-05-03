//
// Created by Balalarga on 07.03.2022.
//

#ifndef RANOKLANGUAGE_SYMBOLSTABLE_H
#define RANOKLANGUAGE_SYMBOLSTABLE_H

#include "Expression.h"

#include <vector>
#include <map>

struct Range
{
    double min = -1, max = 1;
};

class SymbolsTable
{
public:
    SymbolsTable();

    spVariableExpression CreateArgument(const std::string& name);
    spVariableExpression CreateArgument(const std::string& name, Range range);
    spVariableExpression CreateArgument(const std::string& name, std::vector<Range> ranges);
    spVariableExpression CreateVariable(const std::string& name, spExpression child);
    spNumberExpression   CreateConstant(double value);
    spArrayExpression    CreateConstant(std::vector<double> value);
    spArrayExpression    CreateConstant(std::vector<spExpression> value);

    spVariableExpression FindArgument(const std::string& name);
    spVariableExpression FindVariable(const std::string& name);
    spExpression FindConstant(const std::string& name);

    inline std::vector<spVariableExpression>& Arguments(){ return _arguments; }
    inline std::vector<spVariableExpression>& Variables(){ return _variables; }
    inline std::vector<spExpression>&         Constants(){ return _constants; }
    
    inline std::vector<std::vector<Range>>& Ranges(){ return _argumentRanges; }


    static std::map<std::string, double> GlobalConstants;


private:
    std::vector<spVariableExpression> _arguments;
    std::vector<std::vector<Range>> _argumentRanges;
    std::vector<spVariableExpression> _variables;
    std::vector<spExpression> _constants;
};

#endif //RANOKLANGUAGE_SYMBOLSTABLE_H
