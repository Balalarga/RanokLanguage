#ifndef STRINGUTILITY_H
#define STRINGUTILITY_H

#include <string>
#include <vector>

class StringUtility
{
public:

    static std::string Trim(const std::string& str,
                            const std::string& whitespace = " \t\n");


    static std::string Reduce(const std::string& str,
                              const std::string& fill = " ",
                              const std::string& whitespace = " \t\n");

    static std::vector<std::string> Split(std::string string, const std::string &delimiter);

    static std::string GetRandomString(int len);


private:
    StringUtility() = delete;
};

#endif // STRINGUTILITY_H
