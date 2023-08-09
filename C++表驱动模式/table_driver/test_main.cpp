#include <iostream>
#include "table_driver.h"

int main() {
    TableDriver::CalcOfInt calcOfInt;
    int a = 5, b = 3;
    std::string opt = "*";
    int ans = calcOfInt.CalcNums(opt, a, b);
    std::cout << a << " " << opt << " " << b << " = " << ans << std::endl;
    return 0;
}