#include "App/Slave/Slave.h"

Slave::Slave(const std::string& source) :
    mSource{ source }
{}

std::string
Slave::GetSource() const
{
    return mSource;
}
