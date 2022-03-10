#include <iostream>
#include <fstream>
#include <sstream>

#include "LanguageCore/Lexer.h"
#include "LanguageCore/Parser.h"
#include "LanguageCore/CodeGenerator.h"

using namespace std;

int& NextErrorCode()
{
    static int ErrorCounter = 0;
    return --ErrorCounter;
}


int main(int argc, char** argv)
{
    string file = "../CodeExamples/test1.txt";
    if (argc == 2)
        file = argv[1];

    fstream codeFile(file);
    if (!codeFile)
    {
        cout << "Couldn't open file " << argv[1] << endl;
        return NextErrorCode();
    }

    stringstream stream;
    stream << codeFile.rdbuf();
    codeFile.close();

    string code = stream.str();

    Parser parser;
    Program program = parser.Parse(Lexer::CreateFrom(code));
    cout << "\n\n";
    if (program.Root())
    {
        queue<pair<int, Expression*>> nodes;
        program.Root()->Visit(nodes);
        cout << "Visiting AST\n";
        while (!nodes.empty())
        {
            auto& top = nodes.front();
            constexpr const char* depthPrefix = "  ";
            for (int i = 1; i < top.first; ++i)
                cout << depthPrefix;
            cout << top.second->name << endl;
            nodes.pop();
        }
        
        CodeGenerator::LanguageDefinition langDef;
        CodeGenerator gener(langDef);
        cout << "Generate\n";
        cout << gener.Generate(program);
        cout << "Done\n";
    }
    else
    {
        cout << "Root is empty";
    }
    

    return 0;
}
