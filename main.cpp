#include <iostream>

void test_buggy_code(int a, int b, int x)
{
    int diff = b - b;
    a = a;
    if (a & 1 == 0){}
    if (x == 1 && x == 2){}

    if (x == 1){
    }else if (x == 2){
    }else if (x == 1){
    }
    if (x)
    {
        x = 42;
    }
    else
    {
        x = 42;
    }
    if (a && true){}
    if (a = 3){}
}

int main()
{
    test_buggy_code(5, 5, 1);
    return 0;
}