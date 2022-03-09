#include <iostream>
#include <fstream>
#include <sstream>

#include "LanguageCore/Lexer.h"
#include "LanguageCore/Parser.h"

using namespace std;


int& NextErrorCode()
{
    static int ErrorCounter = 0;
    return --ErrorCounter;
}

void PrintArgs(int argc, char** argv)
{
    for(int i = 0; i < argc; ++i)
        cout<<argv[i]<<endl;
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
        queue<pair<int, spExpression>> nodes;
        program.Root()->Visit(nodes);
        cout << "Visiting AST\n";
        while (!nodes.empty())
        {
            auto& top = nodes.front();
            for (int i = 1; i < top.first; ++i)
                cout << '\t';
            cout<<"Node: "<<top.second->name<<endl;
            nodes.pop();
        }
    }
    else
    {
        cout << "Root is empty";
    }
    cout << "\n\n";

    return 0;
}
