#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

double add(double a, double b)
{
    return a + b;
}

double subtract(double a, double b)
{
    return a - b;
}

double multiply(double a, double b)
{
    return a * b;
}

double divide(double a, double b)
{
    return a / b;
}

double addTax(double a, double b)
{
    return a + (a * divide(b, 100));
}

int main()
{
    double a, b, result;
    a = 100;
    b = 5;

    result = add(a, b);
    cout << result;
    cout << "\n";

    result = subtract(a, b);
    cout << result;
    cout << "\n";

    result = multiply(a, b);
    cout << result;
    cout << "\n";

    result = divide(a, b);
    cout << result;
    cout << "\n";

    result = addTax(a, b);
    cout << result;
    cout << "\n";

    return 0;
}