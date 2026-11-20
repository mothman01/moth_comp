#pragma once
#include<string>
#include <memory>

enum class ExpKind{Constant, Unary};
enum class UnaryOp{Negate, Complement};

struct Exp {
    ExpKind kind = ExpKind::Constant;
    int value = 0;
    UnaryOp op = UnaryOp:: Negate;
    std:: unique_ptr<Exp> inner;
};

struct Return{
   Exp exp; 
};

struct Function{
    std::string name;
    Return body;
};

struct Program{
    Function function;
};
