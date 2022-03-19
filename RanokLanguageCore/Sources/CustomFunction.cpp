//
// Created by Balalarga on 10.03.2022.
//

#include "CustomFunction.h"

#include <sstream>


void CustomFunction::SetRoot(spExpression& root)
{
    _root = root;
}

CustomFunction::CustomFunction(const FunctionInfo<FunctionExpression::FuncType> &info, const std::string &code):
    _info(info),
    _code(code)
{
    Parser parser;
    _program = parser.Parse(Lexer::CreateFrom(code));
    _args = _program.Table().Arguments();
    _root = _program.Root();
}

CustomFunction::CustomFunction(const std::string &name, const std::string &code):
    CustomFunction({name, nullptr}, code)
{

}


static std::string Trim(const std::string& str,
                 const std::string& whitespace = " \t\n")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

static std::string Reduce(const std::string& str,
                   const std::string& fill = " ",
                   const std::string& whitespace = " \t\n")
{
    // trim first
    auto result = Trim(str, whitespace);

    // replace sub ranges
    auto beginSpace = result.find_first_of(whitespace);
    while (beginSpace != std::string::npos)
    {
        const auto endSpace = result.find_first_not_of(whitespace, beginSpace);
        const auto range = endSpace - beginSpace;

        result.replace(beginSpace, range, fill);

        const auto newStart = beginSpace + fill.length();
        beginSpace = result.find_first_of(whitespace, newStart);
    }

    return result;
}


std::string CustomFunction::ToString(const CustomFunction &func)
{
    std::stringstream stream;
    stream << func.Name() << "{" << func.Code();
    return stream.str();
}

CustomFunction CustomFunction::FromString(const std::string &str, int& endId)
{
    std::string name;
    std::string code;
    int codeStart = str.find_first_of("{");
    int codeEnd = str.find_first_of("}");
    name = Reduce(str.substr(0, codeStart++), "");
    code = Trim(str.substr(codeStart, codeEnd - codeStart));
    endId += codeEnd + 1;
    return {name, code};
}
