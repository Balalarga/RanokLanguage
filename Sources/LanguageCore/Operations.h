//
// Created by Balalarga on 07.03.2022.
//

#include <map>
#include <functional>


class Operations
{
public:
    Operations() = delete;

    using Unary = std::function<double(double)>;
    using Binary = std::function<double(double, double)>;

    static Unary UnaryFromString(const std::string& name);
    static const std::map<std::string, Unary>& GetUnaries();

    static Unary unaryMinus;

    
    static Binary BinaryFromString(const std::string& name);
    static const std::map<std::string, Binary>& GetBinaries();

    static Binary plus;
    static Binary minus;
    static Binary divides;
    static Binary multiplies;
    static Binary power;
    static Binary RvCross;
    static Binary RvUnion;


private:
    static std::map<std::string, Unary> _unaryOperations;
    static std::map<std::string, Binary> _binaryOperations;
};