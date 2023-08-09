#include "table_driver.h"

namespace TableDriver {

int CalcOfInt::AddNums(int a, int b)
{
    return a + b;
}

int CalcOfInt::MinusNums(int a, int b)
{
    return a - b;
}

int CalcOfInt::MultNums(int a, int b)
{
    return a * b;
}

int CalcOfInt::DivideNums(int a, int b)
{
    if (b == 0) {
        return 0;
    }
    return a / b;
}

int CalcOfInt::CalcNums(const std::string& opt, int a, int b)
{
    CalcOfInt::FuncPtr funcPtr = this->calcFuncs_[opt];
    return (this->*funcPtr)(a, b);
}

} // namespace TableDriver