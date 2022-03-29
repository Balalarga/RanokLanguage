//
// Created by Balalarga on 06.03.2022.
//

#ifndef RANOKLANGUAGE_CHECKED_RESULT_H
#define RANOKLANGUAGE_CHECKED_RESULT_H

template<class T>
class CheckedResult
{
public:
    CheckedResult(const T& result, bool check = true):
        _value(result),
        _check(check) {}
        
    CheckedResult():
        _check(false) {}
    
    inline bool Ok() const { return _check; }

    inline const T& Get() const { return _value; }


private:
    T _value;
    bool _check;
};

#endif //RANOKLANGUAGE_CHECKED_RESULT_H
