//
// Created by Balalarga on 06.03.2022.
//

#ifndef RANOKLANGUAGE_CHECKS_H
#define RANOKLANGUAGE_CHECKS_H

#include <variant>

template<class T, class U>
class Variant
{
public:
    Variant(const T& value):
        _data(value)
    {}
    Variant(const U& value):
        _data(value)
    {}
    virtual ~Variant() = default;

    template<class To>
    bool Is() const { return std::holds_alternative<To>(_data); }

    template<class To>
    To Get() const { return std::get<To>(_data); }

    
private:
    std::variant<T, U> _data;
};


#endif //RANOKLANGUAGE_CHECKS_H
