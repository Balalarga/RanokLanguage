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
#include "Utils.h"


class Expression
{
public:
    Expression(const std::string& name = "");

    virtual void Visit(std::queue<std::pair<int, std::shared_ptr<Expression>>>&, int depth = 0);
    virtual double GetValue();
    virtual void Reset();

    void SetValue(double val);

    inline bool Computed() const { return computed; }

    const std::string name;


protected:
    bool computed = false;
    double value = 0;
};
using spExpression = std::shared_ptr<Expression>;


class NumberExpression: public Expression
{
public:
    NumberExpression(double value);
    void Reset() override;
};
using spNumberExpression = std::shared_ptr<NumberExpression>;


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

    virtual void Visit(std::queue<std::pair<int, std::shared_ptr<Expression>>>& container, int depth = 0) override;
    virtual double GetValue() override;
    virtual void Reset() override;

    spExpression const child;
};
using spVariableExpression = std::shared_ptr<VariableExpression>;


class UnaryOperation: public Expression
{
public:
    UnaryOperation(const FunctionInfo<double(double)>& operation, spExpression child);

    virtual void Visit(std::queue<std::pair<int, std::shared_ptr<Expression>>>& container, int depth = 0) override;
    virtual double GetValue() override;
    virtual void Reset() override;

    const FunctionInfo<double(double)> operation;
    spExpression const child;
};
using spUnaryOperation = std::shared_ptr<UnaryOperation>;


class BinaryOperation: public Expression
{
public:
    BinaryOperation(const FunctionInfo<double(double, double)>& operation,
        spExpression leftChild,
        spExpression rightChild);

    virtual void Visit(std::queue<std::pair<int, std::shared_ptr<Expression>>>& container, int depth = 0) override;
    virtual double GetValue() override;
    virtual void Reset() override;

    const FunctionInfo<double(double, double)> operation;
    spExpression const leftChild;
    spExpression const rightChild;
};
using spBinaryOperation = std::shared_ptr<BinaryOperation>;


class FunctionExpression: public Expression
{
public:
    using FuncType = Variant<spExpression, std::string>(std::vector<spExpression>);
    FunctionExpression(const FunctionInfo<FuncType>& function, const std::vector<spExpression>& args);

    virtual void Visit(std::queue<std::pair<int, std::shared_ptr<Expression>>>& container, int depth = 0) override;
    virtual double GetValue() override;
    virtual void Reset() override;

    const FunctionInfo<FuncType> function;
    const std::vector<spExpression> params;
};
using spFunctionExpression = std::shared_ptr<FunctionExpression>;


#endif //RANOKLANGUAGE_EXPRESSION_H
