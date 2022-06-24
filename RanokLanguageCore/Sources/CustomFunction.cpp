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
    auto nameNtag = StringUtility::Split(name, ":");
    _info.name = nameNtag[0];
    if (nameNtag.size() != 1)
        _info.tags = nameNtag[1];

    Parser parser;
    _program = parser.Parse(Lexer::CreateFrom(code));
    _args = _program.Table().Arguments();
    _root = _program.Root();

    std::stringstream customDesc;
    customDesc << _info.name << "(";
    for (size_t i = 0; i < Args().size(); ++i)
    {
        customDesc << Args()[i]->name;
        if (auto arr = dynamic_cast<ArrayExpression*>(Args()[i]->child.get()))
        {
            customDesc << "[" << arr->Values.size() << "]";
        }
        if (i != Args().size() - 1)
            customDesc << ", ";
    }
    customDesc << ") -> ";

    // Find out return value type
    if (_program.Root())
    {
        if (auto arr = dynamic_cast<ArrayExpression*>(_program.Root().get()))
        {
            _info.returnType = {LanguageType::DoubleArray, static_cast<unsigned>(arr->Values.size())};
        }
        else if (auto var = dynamic_cast<VariableExpression*>(_program.Root().get()))
        {
            if (auto arr = dynamic_cast<ArrayExpression*>(var->child.get()))
            {
                _info.returnType = {LanguageType::DoubleArray, static_cast<unsigned>(arr->Values.size())};
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
        else
        {
            _info.returnType.Count = 1;
            _info.returnType.Type = LanguageType::Double;
        }
    }

    if (_info.returnType.Count == 1)
        customDesc << "number";
    else
        customDesc << "[" << _info.returnType.Count << "]";
    _info.desc = customDesc.str();
}

std::string CustomFunction::ToString(const CustomFunction &func)
{
    std::stringstream stream;
    stream << func.Name();
    if (func.Info().Tags().size() > 0)
    {
        stream << ":";
        stream << func.Info().Tags();
    }
    stream << "\n{\n" << func.Code() << "\n}\n";
    return stream.str();
}

CustomFunction CustomFunction::FromString(const std::string &str, size_t& endId)
{
    std::string name;
    std::string code;
    size_t codeStart = str.find_first_of('{');
    int braceCounter = 0;
    size_t codeEnd = str.size();
    for (size_t i = codeStart; i < str.size(); ++i)
    {
        if (str[i] == '{')
            ++braceCounter;
        else if (str[i] == '}')
            --braceCounter;

        if (braceCounter == 0)
        {
            codeEnd = i;
            break;
        }
    }

    name = StringUtility::Reduce(str.substr(0, codeStart++), "");
    code = StringUtility::Trim(str.substr(codeStart, codeEnd - codeStart));
    endId += codeEnd + 1;
    return CustomFunction(name, code);
}

CustomFunction CustomFunction::FromString(const std::string &name, const std::string &code)
{
    return CustomFunction(name, code);
}
