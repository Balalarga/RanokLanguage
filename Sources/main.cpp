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
    if (program.Root())
    {
        CodeGenerator gener;
        cout << gener.Generate(program);
        for (auto& a : program.Table().Arguments())
            a->SetValue(0);
        cout << "Result = " << program.Process() << "\n";
    }
    else
    {
        cout << "Root is empty";
    }
    

    return 0;
}
