//
// Created by Balalarga on 10.03.2022.
//

#ifndef RANOKLANGUAGE_CUSTOMFUNCTION_H
#define RANOKLANGUAGE_CUSTOMFUNCTION_H

#include "Expression.h"
#include "FunctionInfo.h"
#include "Parser.h"


class CustomFunction
{
public:
    virtual ~CustomFunction() = default;

    void SetRoot(spExpression& root);

    inline const FunctionInfo& Info() const { return _info; }
    inline spExpression Root() { return _root; }
    inline const std::vector<spVariableExpression>& Args() const { return _args; }

    inline const std::string& Name() const { return _info.name; }
    inline const std::string& Code() const { return _code; }

    inline Program& GetProgram() { return _program; };


    static std::string ToString(const CustomFunction& func);
    static CustomFunction FromString(const std::string& str, size_t& endId);
    static CustomFunction FromString(const std::string& name, const std::string& code);


protected:
    CustomFunction(const std::string& name, const std::string &code);


private:
    FunctionInfo _info;
    spExpression _root;
    std::vector<spVariableExpression> _args;
    Program _program;
    const std::string _code;
};

#endif //RANOKLANGUAGE_CUSTOMFUNCTION_H
