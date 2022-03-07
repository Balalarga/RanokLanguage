//
// Created by Balalarga on 07.03.2022.
//

#include <map>
#include <functional>

class Operations
{
public:
    Operations() = delete;

    static std::function<double(double)> unaryMinus;
    
    static std::function<double(double, double)> plus;
    static std::function<double(double, double)> minus;
    static std::function<double(double, double)> divides;
    static std::function<double(double, double)> multiplies;
    static std::function<double(double, double)> power;
    static std::function<double(double, double)> RvCross;
    static std::function<double(double, double)> RvUnion;

    static std::function<double(double, double)> FromString(const std::string& name);
};