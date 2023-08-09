#include <map>
#include <vector>

namespace TableDriver{

class CalcOfInt {
public:
    int AddNums(int a, int b);
    int MinusNums(int a, int b);
    int MultNums(int a, int b);
    int DivideNums(int a, int b);
    int CalcNums(const std::string& opt, int a, int b);

private:
    std::vector<std::string> opts_ = {"+", "-", "*", "/"};
    using FuncPtr = int(CalcOfInt::*)(int a, int b);
    std::map<std::string, FuncPtr> calcFuncs_ = {
        {opts_[0], &CalcOfInt::AddNums},
        {opts_[1], &CalcOfInt::MinusNums},
        {opts_[2], &CalcOfInt::MultNums},
        {opts_[3], &CalcOfInt::DivideNums},
    };
};

} // namespace TableDriver

