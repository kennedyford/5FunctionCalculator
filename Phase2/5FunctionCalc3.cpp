// include some stuff we may need + using std namespace
#include <iostream>
#include <fstream>
#include <cmath>
#include <string.h>
using namespace std;

// Simple addition function
double add(double a, double b)
{
    return a + b;
}

// Simple subtraction function
double subtract(double a, double b)
{
    return a - b;
}

// Simple multiplication function
double multiply(double a, double b)
{
    return a * b;
}

// Simple division function
double divide(double a, double b)
{
    return a / b;
}

// Takes the percent of tax (b) and divides it by 100 using our divide function to get the value to a decimal.
// It then multiplies that value by the total (a) which is what we need to add to the total (a).
double addTax(double a, double b)
{
    return a + (a * divide(b, 100));
}

// Analyzes the operation that is inputted and performs the correct operations.
// (If order of opertions is requested, this will be changes into having an input of values in postfix format in an array [probably] -- TBD)
double executeOp(double a, double b, string op)
{
    if (op.compare("+") == 0)
    {
        return add(a, b);
    }
    else if (op.compare("-") == 0)
    {
        return subtract(a, b);
    }
    else if (op.compare("*") == 0)
    {
        return multiply(a, b);
    }
    else if (op.compare("/") == 0)
    {
        return divide(a, b);
    }
    else if (op.compare("%") == 0) 
    {
        return addTax(a, b);
    }
    else 
    {
        cout << "Error! Invalid operator!"; // This shouldn't ever be called, but its good to have just in case.
        return 0;
    }
}

int main()
{
    // *** VARIABLES ***
    double a, b, prevResult, ListBox;
    string op;
    string input;
    bool aDefined = false;
    bool DEBUG = false;

    // for fun variables
    string spacing = "";
    int spacingNum;

    // Creates/Overwrites CalcTrace.txt
    ofstream MyFile("CalcTrace.txt"); 

    // *** Instructions for user ***
    cout << "Options for calculations: +, -, *, /, %, and any integer or float." << endl;
    cout << "Input 'c' to revert to the previous result, and enter 'ce' to clear all results." << endl;
    cout << "To see your current result, input '='." << endl;
    cout << "Input 'exit' to end execution." << endl;
    cout << "-----------------------------------------------\n\n";

    // Infinite loop so that the program runs until the user wishes to break out of it
    while(1){
        // Get input from the user
        cout << "Input: ";
        cin >> input;

        // if the user inputs 'exit', the program will break out of the infinite loop
        if (input.compare("exit") == 0)
        {
            break;
        }
        // if the user inputs '=', then the program will return the current result that has been computed (stored in the variable 'a')
        else if (input.compare("=") == 0)
        {
            ListBox = a;
        }
        // if the user inputs 'c', the current result will be reset to the previously computed value
        else if (input.compare("c") == 0)
        {
            a = prevResult;
            ListBox = a;
        }
        // if the user inputs 'ce', all variables will be reset (I have just set them to 0 since there doesn't seem to be a more effective way to clear them.)
        else if (input.compare("ce") == 0)
        {
            a = 0;
            b = 0;
            aDefined = false;
            ListBox = a;
        }
        // if the user inputs one of the valid operations, the program will store that operation in the 'op' variable
        // this statement is written so that if the user choses one operation and then inputs another operation, then it will override to the most recently chosen operation
        else if ((input.compare("+") == 0) | (input.compare("-") == 0) | (input.compare("*") == 0) | (input.compare("/") == 0) | (input.compare("%") == 0))
        {
            op = input;
        }
        // if all other conditions have not been met, then we can assume the value is of numerical value (integer/float/double)
        // I have chosen not to error check for invalid inputs here because the user will not have the option to chose anything else once the GUI is implemented.
        else 
        {
            // if the 'a' value has already been defined (this isn't the first input of the program or first input after 'ce')
            // then assign the new input to 'b', save the previous result in 'prevResult', and compute the new current result and save it in 'a'
            if (aDefined)   
            {
                b = double(stoi(input));
                ListBox = b;
                prevResult = a;
                a = executeOp(a, b, op);
            }
            // otherwise, assign the new input to 'a' and change the aDefined boolean to true, since we have just defined 'a'
            else 
            {
                a = double(stoi(input));
                ListBox = a;
                aDefined = true;
            }
        }
        
        // Just a little bit of fun with the ListBox output until Tom answers my questions :)
        spacingNum = 8 - (to_string(ListBox).length() - 7);
        for (int i = 0; i < spacingNum; i++)
        {
            spacing = spacing + " ";
        }
        spacing = spacing + "|";

        cout << "______________________" << endl;
        cout << "|--------------------|" << endl;
        cout << "|   ListBox: " << ListBox << spacing << endl;
        cout << "|--------------------|" << endl;
        cout << "|___ ___ ___ ___ ____|" << endl;
        cout << "| 7 | 8 | 9 | C | CE |" << endl;
        cout << "|--------------------|" << endl;
        cout << "| 4 | 5 | 6 | + | -  |" << endl;
        cout << "|--------------------|" << endl;
        cout << "| 3 | 2 | 1 | * | /  |" << endl;
        cout << "|--------------------|" << endl;
        cout << "|   0   | . | = | %  |" << endl;
        cout << "|--------------------|" << endl;
        cout << "|____________________|\n" << endl;

        spacing = "";

        // Input current info to CalcTrace.txt
        MyFile << "Input: " << input << endl;
        MyFile << "a: " << a << endl;
        MyFile << "b: " << b << endl;
        MyFile << "op: " << op << endl;
        MyFile << "ListBox: " << ListBox << endl;
        MyFile << "\n-----------\n\n";

        // when the system is being debugged, the program will output the current values for the variables 'a', 'b', 'prevResult', and 'aDefined'
        if (DEBUG)
        {
            cout << "a: " << a << endl;
            cout << "b: " << b << endl;
            cout << "prevResult: " << prevResult << endl;
            cout << "aDefined: " << aDefined << endl;
        }
    }

    // Closes the data stream for writing to the CalcTrace.txt file
    MyFile << "EOF\n";
    MyFile.close();

    return 0;
}



/*
______________________
|--------------------|
|   ListBox: _       |
|--------------------|
|___ ___ ___ ___ ____|
| 7 | 8 | 9 | C | CE |
|--------------------|
| 4 | 5 | 6 | + | -  |
|--------------------|
| 3 | 2 | 1 | * | /  |
|--------------------|
|   0   | . | = | %  |
|--------------------|
|____________________|

*/