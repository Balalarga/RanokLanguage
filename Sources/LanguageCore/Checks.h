//
// Created by Balalarga on 06.03.2022.
//

#ifndef RANOKLANGUAGE_CHECKS_H
#define RANOKLANGUAGE_CHECKS_H

template<class Result, class Error = bool>
struct CheckedResult
{
    CheckedResult(Error error):
            error(error){}
    CheckedResult(Result result):
            result(result){}

    Result result;
    Error error;

    operator Result() const
    {
        return result;
    }
    operator Error() const
    {
        return error;
    }
};

#endif //RANOKLANGUAGE_CHECKS_H
