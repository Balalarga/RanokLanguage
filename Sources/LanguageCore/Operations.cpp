//
// Created by Balalarga on 07.03.2022.
//

#include "Operations.h"
#include <cmath>


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


std::function<double(double, double)> Operations::FromString(const std::string& name)
{
    if (name == "+")
        return plus;

    if (name == "-")
        return minus;

    if (name == "/")
        return divides;

    if (name == "*")
        return multiplies;

    if (name == "^")
        return power;

    if (name == "|")
        return RvUnion;

    if (name == "&")
        return RvCross;

    return nullptr;
}