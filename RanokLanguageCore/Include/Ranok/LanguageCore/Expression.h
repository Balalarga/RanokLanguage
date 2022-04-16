//
// Created by Balalarga on 05.03.2022.
//

#ifndef RANOKLANGUAGE_EXPRESSION_H
#define RANOKLANGUAGE_EXPRESSION_H

#include <string>
#include <functional>
#include <queue>
#include <memory>

#include "FunctionInfo.h"
#include "Ranok/Utility/CheckedResult.h"


class Expression
{
public:
    Expression(const std::string& name = "");
    virtual ~Expression() = default;

    virtual void Visit(std::queue<std::pair<int, Expression*>>&, int depth = 0);

    const std::string name;
};
using spExpression = std::shared_ptr<Expression>;


class NumberExpression: public Expression
{
public:
    NumberExpression(double value);
    const double Value;
};
using spNumberExpression = std::shared_ptr<NumberExpression>;


class ArrayExpression: public Expression
{
public:
    ArrayExpression(size_t count);
    const size_t Size;
};
using spArrayExpression = std::shared_ptr<ArrayExpression>;


class ArgumentExpression: public Expression
{
public:
    struct Range
    {
        double min;
        double max;
    };

    ArgumentExpression(const std::string& name, const Range& range);

    const Range range;
};
using spArgumentExpression = std::shared_ptr<ArgumentExpression>;


class VariableExpression: public Expression
{
public:
    VariableExpression(const std::string& name, spExpression child);

    void VisitRecur(std::queue<std::pair<int, Expression*>>& container, int depth = 0);
    virtual void Visit(std::queue<std::pair<int, Expression*>>& container, int depth = 0) override;

    spExpression const child;
};
using spVariableExpression = std::shared_ptr<VariableExpression>;


class UnaryOperation: public Expression
{
public:
    UnaryOperation(const FunctionInfo& operation, spExpression child);

    virtual void Visit(std::queue<std::pair<int, Expression*>>& container, int depth = 0) override;

    const FunctionInfo operation;
    spExpression const child;
};
using spUnaryOperation = std::shared_ptr<UnaryOperation>;


class BinaryOperation: public Expression
{
public:
    BinaryOperation(const FunctionInfo& operation,
        spExpression leftChild,
        spExpression rightChild);

    virtual void Visit(std::queue<std::pair<int, Expression*>>& container, int depth = 0) override;

    const FunctionInfo operation;
    spExpression const leftChild;
    spExpression const rightChild;
};
using spBinaryOperation = std::shared_ptr<BinaryOperation>;


class FunctionExpression: public Expression
{
public:
    using FuncType = CheckedResult<double>(std::vector<spExpression>);
    FunctionExpression(const FunctionInfo& function, const std::vector<spExpression>& args);

    virtual void Visit(std::queue<std::pair<int, Expression*>>& container, int depth = 0) override;

    const FunctionInfo function;
    const std::vector<spExpression> params;
};
using spFunctionExpression = std::shared_ptr<FunctionExpression>;


class CustomFunctionExpression: public FunctionExpression
{
public:
    CustomFunctionExpression(const FunctionInfo& function, spExpression root, const std::vector<spExpression>& args);

    /// Used for code generation only
    const spExpression root;
};
using spCustomFunctionExpression = std::shared_ptr<CustomFunctionExpression>;

#endif //RANOKLANGUAGE_EXPRESSION_H
