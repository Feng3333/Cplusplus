#include <iostream>
#include <map>

class FunMap {
public:
    
    int func1(int a, int b)
    {
        return a - b;
    }

    int func2(int a, int b) 
    {
        return a + b;
    }

    typedef int(FunMap::*FUN)(int a, int b);
    std::map<std::string, FUN> map = {
        {"+", &FunMap::func1},
        {"-", &FunMap::func2},
    };
};

int main() {
    FunMap *m = new FunMap();
    int a = 10, b = 5;
    std::string str("-");
    FunMap::FUN f = m->map[str];
    int ans = (m->*f)(a, b);
    std::cout << ans;
    return 0;
}
