//
// Created by Balalarga on 09.03.2022.
//

#ifndef RANOKLANGUAGE_CODEGENERATOR_H
#define RANOKLANGUAGE_CODEGENERATOR_H

#include "Expression.h"

#include <map>
#include <sstream>


class CodeGenerator
{
public:
    CodeGenerator(std::map<std::string, std::string>&& functionsMappings,
                  std::map<std::string, std::string>&& operationsMappings = {});
    virtual ~CodeGenerator() = default;

    virtual std::string Generate(spExpression root);


protected:
    void GetFunctionCode(const std::string& name, std::stringstream& code);
    void GetOperationCode(const std::string& name, std::stringstream& code);
    void GetExpressionCode(Expression*& expression, std::stringstream &code);

private:
    void CheckMappings();

    std::map<std::string, std::string> _functionsMapping;

    std::map<std::string, std::string> _operationsMapping;
    static std::map<std::string, std::string> defaultOperationsMappings;
};


#endif //RANOKLANGUAGE_CODEGENERATOR_H
