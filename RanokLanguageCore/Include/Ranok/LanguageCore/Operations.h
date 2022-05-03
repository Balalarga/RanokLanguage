//
// Created by Balalarga on 07.03.2022.
//

#include "FunctionInfo.h"


class Operations
{
public:
    Operations() = delete;

    static FunctionInfo* UnaryFromString(const std::string& name);
    static const std::vector<FunctionInfo>& GetUnaries();
    
    static FunctionInfo* BinaryFromString(const std::string& name);
    static const std::vector<FunctionInfo>& GetBinaries();
    

private:
    static std::vector<FunctionInfo> _unaryOperations;
    static std::vector<FunctionInfo> _binaryOperations;
};
