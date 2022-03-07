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
    if (argc != 2)
    {
        //cout << "Argument error: must be like: app.exe code.txt\n";
        //return NextErrorCode();
    }
    else
    {
        file = argv[1];
    }

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

    Lexer lexer;
    lexer.Process(code);

    Parser parser;
    Program program = parser.Parse(lexer);
    queue<pair<int, spExpression>> nodes;
    program.Root()->Visit(nodes);
    while(!nodes.empty())
    {
        auto& top = nodes.front();
        cout<<top.second->name<<endl;
        nodes.pop();
    }

    return 0;
}
