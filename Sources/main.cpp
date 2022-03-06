#include <iostream>
#include <fstream>
#include <sstream>

#include "LanguageCore/Lexer.h"

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
    if (argc != 2)
    {
        cout << "Argument error: must be like: app.exe code.txt\n";
        PrintArgs(argc, argv);
        return NextErrorCode();
    }

    fstream codeFile(argv[1]);
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
    while (!lexer.Empty())
    {
        auto lexeme = lexer.Pop();
        cout << TokenToString(lexeme.token)<<": ";
        visit([](const auto& value){ cout << value;}, lexeme.value);
        cout<<"\n";
    }

    return 0;
}
