//
// Created by Balalarga on 10.03.2022.
//

#include "CustomFunction.h"
#include "../Utility/StringUtility.h"
#include <sstream>


void CustomFunction::SetRoot(spExpression& root)
{
    _root = root;
}

CustomFunction::CustomFunction(const std::string& name, const std::string &code):
    _code(code)
{
    _info.name = name;

    Parser parser;
    _program = parser.Parse(Lexer::CreateFrom(code));
    _args = _program.Table().Arguments();
    _root = _program.Root();

    std::stringstream customDesc;
    customDesc << _info.name << "(";
    for (size_t i = 0; i < Args().size(); ++i)
    {
        customDesc << Args()[i]->name;
        if (i != Args().size() - 1)
            customDesc << ", ";
    }
    customDesc << ")";
    _info.desc = customDesc.str();

    // Find out return value type
    if (_program.Root())
    {
        if (auto arr = dynamic_cast<ArrayExpression*>(_program.Root().get()))
        {
            _info.returnType = {LanguageType::DoubleArray, arr->Values.size()};
        }
        else if (auto var = dynamic_cast<VariableExpression*>(_program.Root().get()))
        {
            if (auto arr = dynamic_cast<ArrayExpression*>(var->child.get()))
            {
                _info.returnType = {LanguageType::DoubleArray, arr->Values.size()};
            }
            else
            {
                _info.returnType.Type = LanguageType::Double;
            }
        }
        else if (auto expr = dynamic_cast<FunctionExpression*>(_program.Root().get()))
        {
            _info.returnType = expr->function.ReturnType();
        }
    }
}

std::string CustomFunction::ToString(const CustomFunction &func)
{
    std::stringstream stream;
    stream << func.Name() << "\n{\n" << func.Code() << "\n}\n";
    return stream.str();
}

CustomFunction CustomFunction::FromString(const std::string &str, int& endId)
{
    std::string name;
    std::string code;
    int codeStart = str.find_first_of("{");
    int codeEnd = str.find_first_of("}");
    name = StringUtility::Reduce(str.substr(0, codeStart++), "");
    code = StringUtility::Trim(str.substr(codeStart, codeEnd - codeStart));
    endId += codeEnd + 1;
    return CustomFunction(name, code);
}

CustomFunction CustomFunction::FromString(const std::string &name, const std::string &code)
{
    return CustomFunction(name, code);
}
