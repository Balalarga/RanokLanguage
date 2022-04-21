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
            std::cout << "Return array expression with size "<< arr->Values.size() << std::endl;
            _info.returnType = {LanguageType::DoubleArray, static_cast<unsigned>(arr->Values.size())};
        }
        else if (auto var = dynamic_cast<VariableExpression*>(_program.Root().get()))
        {
            if (auto arr = dynamic_cast<ArrayExpression*>(var->child.get()))
            {
                std::cout << "Return var has array expression with size "<< arr->Values.size() << std::endl;
                _info.returnType = {LanguageType::DoubleArray, static_cast<unsigned>(arr->Values.size())};
            }
            else
            {
                std::cout << "Return var has number expressio" << std::endl;
                _info.returnType.Type = LanguageType::Double;
            }
        }
        else if (auto expr = dynamic_cast<FunctionExpression*>(_program.Root().get()))
        {
            _info.returnType = expr->function.ReturnType();
            std::cout << "Return function has type ";

            if (_info.returnType.Type == LanguageType::DoubleArray)
                std::cout << "Array [" << _info.returnType.Count << "]";
            else
                std::cout << "Number";

            std::cout << std::endl;
        }
        else
        {
            _info.returnType.Count = 1;
            _info.returnType.Type = LanguageType::Double;
            std::cout << "Function has type ";

            if (_info.returnType.Type == LanguageType::DoubleArray)
                std::cout << "Array [" << _info.returnType.Count << "]";
            else
                std::cout << "Number";

            std::cout << std::endl;
        }

        std::cout << "CustomFunction "<<name<<std::endl;

//        std::queue<std::pair<int, Expression *>> nodes;
//        _program.Root()->Visit(nodes);

//        for (auto& func: _program.Table().Variables())
//            func->VisitRecur(nodes);

//        while (!nodes.empty())
//        {
//            auto &top = nodes.front();

//            for (int i = 1; i < top.first; ++i)
//                std::cout << "  ";

//            if (auto func = dynamic_cast<FunctionExpression *>(top.second))
//            {
//                std::cout << func->function.Name() + "(";
//                for (auto &a: func->params)
//                    std::cout << a->name << ", ";
//                std::cout << ")\n";
//            }
//            else
//            {
//                std::cout << "Node: " << top.second->name << std::endl;
//            }
//            nodes.pop();
//        }
    }
    else
    {
        std::cout << "CustomFunction " << name << " error\n";
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
    stream << func.Name() << "\n{\n" << func.Code() << "\n}\n";
    return stream.str();
}

CustomFunction CustomFunction::FromString(const std::string &str, int& endId)
{
    std::string name;
    std::string code;
    int codeStart = str.find_first_of("{");
    int braceCounter = 0;
    int codeEnd = str.size();
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
