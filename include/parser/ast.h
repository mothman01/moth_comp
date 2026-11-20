#pragma once
#include<string>




struct Constant{
    int value;
};

struct Return{
   Constant exp; 
};

struct Function{
    std::string name;
    Return body;
};

struct Program{
    Function function;
};
