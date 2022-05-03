#pragma once


enum class LanguageType
{
    Double, DoubleArray
};

struct LanguageReturnType
{
    LanguageType Type;
    unsigned Count = 1;
};