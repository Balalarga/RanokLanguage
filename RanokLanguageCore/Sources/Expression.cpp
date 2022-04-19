//
// Created by Balalarga on 05.03.2022.
//

#include "Expression.h"

#include <iostream>


Expression::Expression(const std::string& name):
    name(name)
{

}

void Expression::Visit(std::queue<std::pair<int, Expression*>>& container, int depth)
{
    container.push({depth, this});
}


NumberExpression::NumberExpression(double value):
    Expression(std::to_string(value)),
    Value(value)
{

}


ArrayExpression::ArrayExpression(const std::vector<spExpression>& values):
    Expression("["+std::to_string(values.size())+"]"),
    Values(values)
{

}


ArrayGetterExpression::ArrayGetterExpression(spVariableExpression root, unsigned id):
    Expression(root->name + "[" + std::to_string(id) + "]"),
    Root(root),
    Id(id)
{
    
}


VariableExpression::VariableExpression(const std::string& name, spExpression child):
    Expression(name),
    child(child)
{
    
}

void VariableExpression::VisitRecur(std::queue<std::pair<int, Expression *> > &container, int depth)
{
    Expression::Visit(container, depth);
    child->Visit(container, depth+1);
}

void VariableExpression::Visit(std::queue<std::pair<int, Expression*>>& container, int depth)
{
    Expression::Visit(container, depth);
}


UnaryOperation::UnaryOperation(const FunctionInfo& operation, spExpression child):
    Expression(operation.Name()),
    operation(operation),
    child(child)
{
    
}

void UnaryOperation::Visit(std::queue<std::pair<int, Expression*>>& container, int depth)
{
    Expression::Visit(container, depth);
    child->Visit(container, depth+1);
}


BinaryOperation::BinaryOperation(const FunctionInfo& operation,
        spExpression leftChild,
        spExpression rightChild):
    Expression(operation.Name()),
    operation(operation),
    leftChild(leftChild),
    rightChild(rightChild)
{
    
}

void BinaryOperation::Visit(std::queue<std::pair<int, Expression*>>& container, int depth)
{
    Expression::Visit(container, depth);
    leftChild->Visit(container, depth+1);
    rightChild->Visit(container, depth+1);
}


FunctionExpression::FunctionExpression(const FunctionInfo& function, const std::vector<spExpression>& args):
    Expression(function.Name()),
    function(function),
    params(args)
{
    
}

void FunctionExpression::Visit(std::queue<std::pair<int, Expression*>>& container, int depth)
{
    Expression::Visit(container, depth);
    for(auto& i: params)
        i->Visit(container, depth+1);
}


CustomFunctionExpression::CustomFunctionExpression(const FunctionInfo& function,
                                                   spExpression root,
                                                   const std::vector<spExpression>& args):
    FunctionExpression(function, args),
    root(std::move(root))
{

}

RangedVariableExpression::RangedVariableExpression(const std::string &name, spExpression child, Range range):
    VariableExpression(name, child),
    range(range)
{

}
