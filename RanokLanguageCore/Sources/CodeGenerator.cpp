//
// Created by Balalarga on 09.03.2022.
//

#include "CodeGenerator.h"
#include "Functions.h"
#include "Operations.h"

#include <iostream>
#include <set>


std::map<std::string, std::string> CodeGenerator::defaultFunctionsMappings
{
    { "abs", "fabs" },
    { "sqrt", "sqrt" },
    { "sin", "sin" },
    { "cos", "cos" },
    { "tan", "tan" },
    { "arctan", "atan" },
    { "arcsin", "asin" },
    { "arccos", "acos" },
    { "cosh", "cosh" },
    { "sinh", "sinh" },
    { "tanh", "tanh" },
    { "exp", "exp" },
    { "ln", "log" },
    { "log", "log" },
    { "log10", "log10" },
    { "log2", "log2" },
    { "ceil", "ceil" },
    { "floor", "floor" }
};

std::map<std::string, std::string> CodeGenerator::defaultUnaryOperationsMappings
{
    {"-", "-{0}"},
};

std::map<std::string, std::string> CodeGenerator::defaultBinaryOperationsMappings
{
    {"+", "{0} + {1}"},
    {"-", "{0} - {1}"},
    {"/", "{0} / {1}"},
    {"*", "{0} * {1}"},
    {"^", "pow({0}, {1})"},
    {"|", "({0} + {1} + sqrt(pow({0}, 2) + pow({1}, 2)))"},
    {"&", "({0} + {1} - sqrt(pow({0}, 2) + pow({1}, 2)))"}
};

std::vector<std::set<std::string>> operationsPrioriry{
    {"+", "-", "&", "|"},
    {"*", "/", "^"},
};

int GetPriority(std::string operation)
{
    for (int i = 0; i < operationsPrioriry.size(); ++i)
    {
        if (operationsPrioriry[i].find(operation) != operationsPrioriry[i].end())
            return i;
    }
    return -1;
}

CodeGenerator::CodeGenerator()
{
    _languageDefinition.functionsMapping = std::move(defaultFunctionsMappings);
    _languageDefinition.unaryOperationsMapping = std::move(defaultUnaryOperationsMappings);
    _languageDefinition.binaryOperationsMapping = std::move(defaultBinaryOperationsMappings);
}

CodeGenerator::CodeGenerator(const LanguageDefinition& langDef):
    _languageDefinition(std::move(langDef))
{
    _languageDefinition.functionsMapping.merge(std::map<std::string, std::string>{
                                                   { "abs", "fabs" },
                                                   { "sqrt", "sqrt" },
                                                   { "sin", "sin" },
                                                   { "cos", "cos" },
                                                   { "tan", "tan" },
                                                   { "arctan", "atan" },
                                                   { "arcsin", "asin" },
                                                   { "arccos", "acos" },
                                                   { "cosh", "cosh" },
                                                   { "sinh", "sinh" },
                                                   { "tanh", "tanh" },
                                                   { "exp", "exp" },
                                                   { "ln", "log" },
                                                   { "log", "log" },
                                                   { "log10", "log10" },
                                                   { "log2", "log2" },
                                                   { "ceil", "ceil" },
                                                   { "floor", "floor" }
                                               });
    _languageDefinition.unaryOperationsMapping.merge(std::map<std::string, std::string>
                                                     {
                                                         {"-", "-{0}"},
                                                     });
    _languageDefinition.binaryOperationsMapping.merge(std::map<std::string, std::string>
                                                      {
                                                          {"+", "{0} + {1}"},
                                                          {"-", "{0} - {1}"},
                                                          {"/", "{0} / {1}"},
                                                          {"*", "{0} * {1}"},
                                                          {"^", "pow({0}, {1})"},
                                                          {"|", "({0} + {1} + sqrt(pow({0}, 2) + pow({1}, 2)))"},
                                                          {"&", "({0} + {1} - sqrt(pow({0}, 2) + pow({1}, 2)))"}
                                                      });
    CheckMappings();
}

void CodeGenerator::CheckMappings()
{
    for (auto& func: Functions::GetAll())
    {
        if (_languageDefinition.functionsMapping.find(func.Name()) == _languageDefinition.functionsMapping.end())
            throw std::runtime_error("Couldn't find code for " + func.Name() + " function");
    }

    for (auto& oper : Operations::GetUnaries())
    {
        if (_languageDefinition.unaryOperationsMapping.find(oper.Name()) == _languageDefinition.unaryOperationsMapping.end())
            throw std::runtime_error("Couldn't find code for " + oper.Name() + " unary operation");
    }

    for (auto& oper : Operations::GetBinaries())
    {
        if (_languageDefinition.binaryOperationsMapping.find(oper.Name()) == _languageDefinition.binaryOperationsMapping.end())
            throw std::runtime_error("Couldn't find code for " + oper.Name() + " binary function");
    }
}

std::string CodeGenerator::Generate(Program& program)
{
    std::stringstream code;
    std::queue<std::pair<int, Expression*>> nodes;
    program.Root()->Visit(nodes);
    
    std::vector<std::string> argsDef;
    const auto& args = program.Table().Arguments();
    for (auto& a: args)
    {
        if (ArrayExpression* child = dynamic_cast<ArrayExpression*>(a->child.get()))
            argsDef.push_back(fmt::format(_languageDefinition.arrayParamSignature, _languageDefinition.numberArrayType, a->name, child->Values.size()));
        else
            argsDef.push_back(fmt::format("{0} {1}", _languageDefinition.numberType, a->name));
    }

    code << DefineFunctions(program);
    code << EnterFunction(_languageDefinition.numberType, _languageDefinition.mainFuncName, fmt::format("{}", fmt::join(argsDef, ", ")));
    code << DefineVariables(program);
    code << fmt::format(_languageDefinition.returnDef, GetExpressionCode(program.Root().get())) << _languageDefinition.endLineDef;
    code << LeaveFunction();

    return code.str();
}

std::string CodeGenerator::DefineFunctions(Program& program)
{
    std::stringstream code;
    std::set<std::string> namesBuffer;
    std::vector<CustomFunction> usedCustomFuncs = Functions::GetAllCustoms();

    std::vector<std::string> argsDef;
    for (auto& func: usedCustomFuncs)
    {
        argsDef.clear();
        for (auto& a: func.Args())
        {
            if (ArrayExpression* child = dynamic_cast<ArrayExpression*>(a->child.get()))
                argsDef.push_back(fmt::format(_languageDefinition.arrayParamSignature, _languageDefinition.numberArrayType, a->name, child->Values.size()));
            else
                argsDef.push_back(fmt::format("{0} {1}", _languageDefinition.numberType, a->name));
        }
        if (_languageDefinition.arrayReturnAsParam && func.Info().ReturnType().Type == LanguageType::DoubleArray)
        {
            if (auto var = dynamic_cast<VariableExpression*>(func.Root().get()))
            {
                if (auto arr = dynamic_cast<ArrayExpression*>(var->child.get()))
                    argsDef.push_back(fmt::format(_languageDefinition.arrayResultParamSignature, _languageDefinition.numberArrayType, var->name, arr->Values.size()));
            }
        }

        if (_languageDefinition.arrayReturnAsParam && func.Info().ReturnType().Type == LanguageType::DoubleArray)
            code << EnterFunction("void", func.Info().Name(), fmt::format("{}", fmt::join(argsDef, ", ")));
        else
            code << EnterFunction(_languageDefinition.numberType, func.Info().Name(), fmt::format("{}", fmt::join(argsDef, ", ")));

        code << DefineVariables(func.GetProgram());

        if (!_languageDefinition.arrayReturnAsParam || func.Info().ReturnType().Type != LanguageType::DoubleArray)
            code << fmt::format(_languageDefinition.returnDef, GetExpressionCode(func.Root().get())) << _languageDefinition.endLineDef;
        
        code << LeaveFunction();
    }
    return code.str();
}

std::string CodeGenerator::DefineVariables(Program& program)
{
    std::stringstream varCode;
    for (auto& var: program.Table().Variables())
    {
        if (auto expr = dynamic_cast<ArrayExpression*>(var->child.get()))
        {
            if (_languageDefinition.arrayReturnAsParam && var.get() == program.Root().get())
            {
                std::vector<std::string> args;
                for (auto& i: expr->Values)
                    args.push_back(GetExpressionCode(i.get()));

                varCode << _languageDefinition.fillResultArray(var->name, args);
            }
            else
            {
                varCode << fmt::format(_languageDefinition.varArrayDefinition, _languageDefinition.numberType, var->name, expr->Values.size());
                std::stringstream params;
                for (size_t i = 0; i < expr->Values.size(); ++i)
                {
                    params << GetExpressionCode(expr->Values[i].get());
                    if (i + 1 < expr->Values.size())
                        params << ", ";
                }
                varCode << " = " << fmt::format(_languageDefinition.arrayInitialization, _languageDefinition.numberType, expr->Values.size(), params.str());
            }
        }
        else if(auto expr = dynamic_cast<FunctionExpression*>(var->child.get()))
        {
            if (expr->function.ReturnType().Type == LanguageType::DoubleArray)
            {
                if (!_languageDefinition.arrayReturnAsParam)
                {
                    varCode << fmt::format(_languageDefinition.varArrayDefinition, _languageDefinition.numberType, var->name, expr->function.ReturnType().Count);
                    varCode << " = " << GetExpressionCode(var->child.get());
                }
                else
                {
                    varCode << fmt::format(_languageDefinition.varArrayDefinition, _languageDefinition.numberType, var->name, expr->function.ReturnType().Count);
                    varCode << _languageDefinition.endLineDef;
                    expr->params.push_back(var);
                    varCode << GetExpressionCode(var->child.get());
                    expr->params.pop_back();
                }
            }
        }
        else
        {
            varCode << fmt::format(_languageDefinition.varDefinition, _languageDefinition.numberType, var->name, GetExpressionCode(var->child.get()));
        }
        varCode << _languageDefinition.endLineDef;
    }
    return varCode.str();
}

const std::string& CodeGenerator::GetFunctionCode(const std::string& name) const
{
    return _languageDefinition.functionsMapping.at(name);
}

const std::string& CodeGenerator::GetUnaryOperationCode(const std::string& name) const
{
    return _languageDefinition.unaryOperationsMapping.at(name);
}

const std::string& CodeGenerator::GetBinaryOperationCode(const std::string& name) const
{
    return _languageDefinition.binaryOperationsMapping.at(name);
}

std::string CodeGenerator::GetExpressionCode(Expression* expression)
{
    if (auto* expr = dynamic_cast<NumberExpression*>(expression))
    {
        return expr->name;
    }
    else if (auto* expr = dynamic_cast<ArrayExpression*>(expression))
    {
        std::stringstream stream;
        stream << "{ ";
        for (size_t i = 0; i < expr->Values.size(); ++i)
        {
            stream << GetExpressionCode(expr->Values[i].get());
            if (i + 1 < expr->Values.size())
                stream << ", ";
        }
        stream << " }";
        return stream.str();
    }
    else if (auto* expr = dynamic_cast<ArrayGetterExpression*>(expression))
    {
        return fmt::format("{0}[{1}]", expr->Root->name, expr->Id);
    }
    else if (auto* expr = dynamic_cast<VariableExpression*>(expression))
    {
        return expr->name;
    }
    else if (auto* expr = dynamic_cast<UnaryOperation*>(expression))
    {
        return fmt::format(GetUnaryOperationCode(expr->operation.Name()), GetExpressionCode(expr->child.get()));
    }
    else if (auto* expr = dynamic_cast<BinaryOperation*>(expression))
    {
        int rootPriority = GetPriority(expr->name);
        auto left = GetExpressionCode(expr->leftChild.get());
        auto right = GetExpressionCode(expr->rightChild.get());
        if (auto op = dynamic_cast<BinaryOperation*>(expr->rightChild.get()))
        {
            if (rootPriority > GetPriority(op->name))
                right = "(" + right + ")";
        }
        if (auto op = dynamic_cast<BinaryOperation*>(expr->leftChild.get()))
        {
            if (rootPriority > GetPriority(op->name))
                left = "(" + left + ")";
        }

        return fmt::format(GetBinaryOperationCode(expr->operation.Name()),
                           left,
                           right);
    }
    else if (auto* expr = dynamic_cast<CustomFunctionExpression*>(expression))
    {
        std::vector<std::string> params;

        for (auto& i: expr->params)
            params.push_back(GetExpressionCode(i.get()));

        return fmt::format("{0}({1})", expr->name, fmt::join(params, ", "));
    }
    else if (auto* expr = dynamic_cast<FunctionExpression*>(expression))
    {
        std::vector<std::string> params;
        
        for (auto& i: expr->params)
            params.push_back(GetExpressionCode(i.get()));

        return fmt::format("{0}({1})", GetFunctionCode(expr->function.Name()), fmt::join(params, ", "));
    }
    return "";
}

std::string CodeGenerator::LeaveFunction()
{
    return _languageDefinition.codeBlock.second;
}

std::string CodeGenerator::EnterFunction(const std::string& returnType,
                                         const std::string& name,
                                         const std::string& params)
{
    std::stringstream enterCode;
    enterCode << fmt::format(_languageDefinition.funcSignature, returnType, name, params) +
                 _languageDefinition.codeBlock.first;
    return enterCode.str();
}

