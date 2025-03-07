#include "App/Master/Master.h"

Master::Master(const std::string& source) :
    mSource{ source }
{}

std::string
Master::GetSource() const
{
    return mSource;
}
