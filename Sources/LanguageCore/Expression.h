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
#include "Checks.h"


class Expression
{
public:
    Expression() = default;

    virtual void Visit(std::queue<std::shared_ptr<Expression>>){}
    virtual double GetValue();
    virtual void Reset();

    void SetValue(double val);

    inline bool Computed() const { return computed; }

protected:
    bool computed = false;
    double value = 0;
};
using spExpression = std::shared_ptr<Expression>;

class ArgumentExpression: public Expression
{
public:
    struct Range
    {
        double min;
        double max;
    };

    ArgumentExpression(const std::string& name, const Range& range):
        name(name),
        range(range){}

    const Range range;
    const std::string name;
};
using spArgumentExpression = std::shared_ptr<ArgumentExpression>;

class VariableExpression: public Expression
{
public:
    VariableExpression(const std::string& name, spExpression child):
        name(name),
        child(child){}

    virtual void Visit(std::queue<spExpression> container) override;
    virtual double GetValue() override;
    virtual void Reset() override;

    spExpression const child;
    const std::string name;
};
using spVariableExpression = std::shared_ptr<VariableExpression>;

class UnaryOperation: public Expression
{
public:
    UnaryOperation(const FunctionInfo<double(double)>& operation, spExpression child):
        operation(operation),
        child(child){}

    virtual void Visit(std::queue<spExpression> container) override;
    virtual double GetValue() override;
    virtual void Reset() override;

    const FunctionInfo<double(double)> operation;
    spExpression const child;
};
using spUnaryOperation = std::shared_ptr<UnaryOperation>;

class BinaryOperation: public Expression
{
public:
    struct Duo
    {
        Duo(const spExpression left, const spExpression right):
            left(left),
            right(right){}
        const spExpression left;
        const spExpression right;
    };
    BinaryOperation(const FunctionInfo<double(double, double)>& operation, Duo childs):
            operation(operation),
            childs(childs){}

    virtual void Visit(std::queue<spExpression> container) override;
    virtual double GetValue() override;
    virtual void Reset() override;

    const FunctionInfo<double(double, double)> operation;
    const Duo childs;
};
using spBinaryOperation = std::shared_ptr<BinaryOperation>;

class FunctionExpression: public Expression
{
public:
    using FuncType = CheckedResult<spExpression, std::string>(std::vector<spExpression>);
    FunctionExpression(const FunctionInfo<FuncType>& function, const std::vector<spExpression>& args):
        function(function){}

    virtual void Visit(std::queue<spExpression> container) override;
    virtual double GetValue() override;
    virtual void Reset() override;

    const FunctionInfo<FuncType> function;
    const std::vector<spExpression> params;
};
using spFunctionExpression = std::shared_ptr<FunctionExpression>;

#endif //RANOKLANGUAGE_EXPRESSION_H
