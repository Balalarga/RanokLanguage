//
// Created by Balalarga on 06.03.2022.
//

#ifndef RANOKLANGUAGE_PROGRAM_H
#define RANOKLANGUAGE_PROGRAM_H

#include "Expression.h"
#include "SymbolsTable.h"

class Program
{
public:
    Program() = default;

    void Init(spExpression begin);

    double Process();
    inline SymbolsTable& Table() { return _symbolsTable; }
    inline const spExpression& Root() { return _root; }


private:
    spExpression _root;
    SymbolsTable _symbolsTable;

};


#endif //RANOKLANGUAGE_PROGRAM_H
