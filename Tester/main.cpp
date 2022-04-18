#include <iostream>
#include <fstream>
#include <sstream>

#include "Ranok/LanguageCore/Parser.h"
#include "Ranok/LanguageCore/Functions.h"
#include "Ranok/LanguageCore/CodeGenerator.h"

using namespace std;

int &NextErrorCode()
{
    static int ErrorCounter = 0;
    return --ErrorCounter;
}


int main(int argc, char **argv)
{
    string file = "../../CodeExamples/test1.txt";
    if (argc == 2)
        file = argv[1];

    fstream codeFile(file);
    if (!codeFile)
    {
        cout << "Couldn't open file " << file << endl;
        return NextErrorCode();
    }

    CodeGenerator::LanguageDefinition langDef;
    langDef.arrayInitialization = "{{{2}}}";
    CodeGenerator gener(langDef);

    stringstream stream;
    stream << codeFile.rdbuf();
    codeFile.close();

    string code = stream.str();

    Parser parser;
    Program program = parser.Parse(Lexer::CreateFrom(code));

    if (program.Root())
    {
        std::queue<std::pair<int, Expression *>> nodes;
        program.Root()->Visit(nodes);
        while (!nodes.empty())
        {
            auto &top = nodes.front();

            for (int i = 1; i < top.first; ++i)
                cout << "  ";

            if (auto func = dynamic_cast<FunctionExpression *>(top.second))
            {
                cout << func->function.Name() << "(";
                for (auto &a: func->params)
                    cout << a->name << ", ";
                cout << ")\n";
            }
            else
            {
                cout << "Node: " << top.second->name << endl;
            }
            nodes.pop();
        }
        cout << gener.Generate(program);
    }
    else
    {
        cout << "Root is empty";
    }

    return 0;
}
