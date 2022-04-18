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

CustomFunction::CustomFunction(const FunctionInfo &info, const std::string &code):
    _info(info),
    _code(code)
{
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
        if (dynamic_cast<ArrayExpression*>(_program.Root().get()))
            _info.returnType = LanguageType::DoubleArray;
        else if (auto expr = dynamic_cast<FunctionExpression*>(_program.Root().get()))
            _info.returnType = expr->function.ReturnType();
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
    int argsStart = str.find_first_of("args");
    std::vector<LanguageType> params;
    while (str[argsStart] != ';')
    {
        if (str[argsStart] == ',')
        {
            if (str[argsStart-1] == ']' && str[argsStart-2] == '[')
                params.push_back(LanguageType::DoubleArray);
            else
                params.push_back(LanguageType::Double);
        }
        argsStart++;
    }
    name = StringUtility::Reduce(str.substr(0, codeStart++), "");
    code = StringUtility::Trim(str.substr(codeStart, codeEnd - codeStart));
    endId += codeEnd + 1;
    return CustomFunction({name, params}, code);
}
