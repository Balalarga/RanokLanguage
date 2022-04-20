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
    std::vector<std::pair<std::string, std::string>> funcs{
//         {"SymetryAxis", R"(
// args o;
// return abs(o);)"},
//         {"Prisma", R"(
// args x, y, z, lenX, lenY, lenZ;
// return (lenX - abs(x)) & (lenY - abs(y)) & (lenZ - abs(z));)"},
//         {"Cube", R"(
// args x, y, z, size;
// return Prisma(x, y, z, size, size, size);)"},
        {"CircleArrayEvenXY", R"(
args s[3], halfCount;
no = halfCount;
ro = sqrt(s[0]^2 + s[1]^2);
tet = arctan(s[1] / s[0]);

ff = tet * no;
mu = 4 / PI / no * (sin(ff) - sin(3*ff)/9 + sin(5*ff)/25 - sin(7*ff)/49); // Похоже на костыль

R = 4;
x11 = ro * cos(mu) - R; // радиус вычитается именно из косинуса
y11 = ro * sin(mu);
sn[3] = { x11, y11, s[2] };
return sn;)"}
    };
    
    for (auto& i: funcs)
        Functions::AddCustom(CustomFunction::FromString(i.first, i.second));
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
    auto glslGener = CodeGenerator::LanguageDefinition()
        .MainFuncName("__resultFunc")
        .Functions({{"abs", "abs"}})
        .NumberType("float")
        .NumberArrayType("float")
        .ArrayParamSignature("{0} {1}[{2}]")
        .ArrayReturnAsParam(true)
        .ArrayResultParamSignature("out {0} {1}[{2}]")
        .FillResultArray([](const std::string& varName, const std::vector<std::string>& params) -> std::string
        {
            std::stringstream stream;

            for (size_t i = 0; i < params.size(); ++i)
            {
                stream << varName << "[" << i << "]" << " = " << params[i];
                if (i + 1 < params.size())
                    stream << ";\n";
            }

            return stream.str();
        });

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
