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

void CreateCustoms()
{
    std::string name = "SpaceMoveX";
    std::string code = R"(
        args S[3];

        v = {S[0] + 1, S[1], S[2]};

        return v;
    )";

    CustomFunction custom = CustomFunction::FromString(name, code);
    Functions::AddCustom(custom);
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

    CreateCustoms();

    CodeGenerator::LanguageDefinition langDef;
    langDef.arrayInitialization = "{{{2}}}";
    langDef.arrayReturnAsParam = true;
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
        cout << parser.Error() << endl;
    }

    return 0;
}
