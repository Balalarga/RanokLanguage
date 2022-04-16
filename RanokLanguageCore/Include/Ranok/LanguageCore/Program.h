//
// Created by Balalarga on 06.03.2022.
//

#ifndef RANOKLANGUAGE_PROGRAM_H
#define RANOKLANGUAGE_PROGRAM_H

#include "Expression.h"
#include "SymbolsTable.h"
#include <iostream>


class Program
{
public:
    Program() = default;

    void Init(const spExpression& root);

    inline SymbolsTable& Table() { return _symbolsTable; }
    inline const spExpression& Root() { return _root; }

    template<class T>
    std::vector<T*> GetAllOf()
    {
        std::vector<T*> res;
        std::queue<std::pair<int, Expression *>> nodes;
        _root->Visit(nodes);
        while (!nodes.empty())
        {
            if (T* casted = dynamic_cast<T*>(nodes.front().second))
                res.push_back(casted);

            nodes.pop();
        }
        return res;
    }

    template<class T>
    std::vector<T*> GetSortedOf()
    {
        std::vector<T*> res;
        std::queue<std::pair<int, Expression *>> nodes;
        _root->Visit(nodes);
        while (!nodes.empty())
        {
            if (T* casted = dynamic_cast<T*>(nodes.front().second))
                res.push_back(casted);

            nodes.pop();
        }
        return res;
    }

    void Dump()
    {
        std::queue<std::pair<int, Expression *>> nodes;
        _root->Visit(nodes);
        std::cout << "----------------------------Program dump---------------------------------\n";
        while (!nodes.empty())
        {
            std::string frontSpacing;
            auto &top = nodes.front();
            for (int i = 1; i < top.first; ++i)
                frontSpacing += "    ";

            std::cout << frontSpacing << "Node: " << top.second->name << "\n";
            frontSpacing += "  ";

            nodes.pop();
        }
        std::cout << "-------------------------------------------------------------------------\n";
    }


private:
    spExpression _root;
    SymbolsTable _symbolsTable;

};


#endif //RANOKLANGUAGE_PROGRAM_H
