//
// Created by Balalarga on 05.03.2022.
//

#include "Expression.h"

#include <iostream>

void Expression::SetValue(double val)
{
    value = val;
    computed = true;
}

double Expression::GetValue()
{
    return value;
}

void Expression::Reset()
{
    computed = false;
}

double VariableExpression::GetValue()
{
    if (!Computed())
        SetValue(child->GetValue());

    return Expression::GetValue();
}

void VariableExpression::Reset()
{
    child->Reset();
    Expression::Reset();
}

void VariableExpression::Visit(std::queue<spExpression> container)
{
    container.push(child);
}

void UnaryOperation::Reset()
{
    child->Reset();
    Expression::Reset();
}

double UnaryOperation::GetValue()
{
    if (!Computed())
        operation.function(child->GetValue());

    return Expression::GetValue();
}

void UnaryOperation::Visit(std::queue<spExpression> container)
{
    container.push(child);
}

void BinaryOperation::Visit(std::queue<spExpression> container)
{
    container.push(childs.left);
    container.push(childs.right);
}

double BinaryOperation::GetValue()
{
    if (!Computed())
        SetValue(operation.function(childs.left->GetValue(), childs.right->GetValue()));
    return Expression::GetValue();
}

void BinaryOperation::Reset()
{
    childs.left->Reset();
    childs.right->Reset();
    Expression::Reset();
}

double FunctionExpression::GetValue()
{
    if (!Computed())
    {
        CheckedResult<spExpression, std::string> result = function.function(params);
        if (result.error.empty())
        {
            SetValue(result.result->GetValue());
        }
        else
        {
            Reset();
            std::cout << "Function " << function.name;
            std::cout << " execute error: ";
            std::cout << result.error << '\n';
        }
    }
    return Expression::GetValue();
}

void FunctionExpression::Reset()
{
    for(auto& i: params)
        i->Reset();
    Expression::Reset();
}

void FunctionExpression::Visit(std::queue<spExpression> container)
{
    for(auto& i: params)
        container.push(i);
    Expression::Visit(container);
}
