#ifndef COMPARERESULT_H
#define COMPARERESULT_H

enum class CompareResult
{
    success,
    failure,
    masterFileNotAvailable,
    slaveFileNotAvailable,
    masterReadError,
    slaveReadError
};

#endif //COMPARERESULT_H
