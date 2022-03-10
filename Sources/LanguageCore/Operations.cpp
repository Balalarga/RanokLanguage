//
// Created by Balalarga on 07.03.2022.
//

#include "Operations.h"
#include <cmath>



std::map<std::string, Operations::Unary> Operations::_unaryOperations
{
    {"-", Operations::unaryMinus},
};
std::map<std::string, Operations::Binary> Operations::_binaryOperations
{
    {"+", Operations::plus},
    {"-", Operations::minus},
    {"/", Operations::divides},
    {"*", Operations::multiplies},
    {"^", Operations::power},
    {"|", Operations::RvUnion},
    {"&", Operations::RvCross},
};

std::function<double(double)> Operations::unaryMinus = [](double v)
{
    return -v;
};

std::function<double(double, double)> Operations::plus = std::plus<double>();
std::function<double(double, double)> Operations::minus = std::minus<double>();
std::function<double(double, double)> Operations::divides = std::divides<double>();
std::function<double(double, double)> Operations::multiplies = std::multiplies<double>();
std::function<double(double, double)> Operations::power = [](double a, double b)
{
    return std::pow(a, b);
};
std::function<double(double, double)> Operations::RvCross = [](double a, double b)
{
    return a + b - std::sqrt(std::pow(a, 2) + std::pow(b, 2));
};
std::function<double(double, double)> Operations::RvUnion = [](double a, double b)
{
    return a + b + std::sqrt(std::pow(a, 2) + std::pow(b, 2));
};


Operations::Unary Operations::UnaryFromString(const std::string& name)
{
    auto it = _unaryOperations.find(name);
    if (it == _unaryOperations.end())
        return nullptr;
    return it->second;
}

Operations::Binary Operations::BinaryFromString(const std::string& name)
{
    auto it = _binaryOperations.find(name);
    if (it == _binaryOperations.end())
        return nullptr;
    return it->second;
}


const std::map<std::string, Operations::Binary>& Operations::GetBinaries()
{
    return _binaryOperations;
}

const std::map<std::string, Operations::Unary>& Operations::GetUnaries()
{
    return _unaryOperations;
}