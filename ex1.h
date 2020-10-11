#ifndef EX1_H
#define EX1_H
#include "Expression.h"
#include <iostream>
#include <stack>
#include <queue>
#include <map>
using namespace std;


// Value class
class Value : public Expression {
private:
     double val;
public:
    Value(const double val);
    double calculate() override;
};

class Variable : public Expression {
private:
    string name;
    double value;
public:
    Variable(const string &name, const double &value);
    double calculate() override;
    Variable& operator++();
    Variable& operator--();
    Variable& operator+=(double val);
    Variable& operator-=(double val);
    Variable& operator++(int num);
    Variable& operator--(int num);
};

class UnaryOperator : public Expression {
protected:
    Expression* exp;
public:
    UnaryOperator(Expression* exp);
    virtual ~UnaryOperator();
};

// UPlus Class
class UPlus : public UnaryOperator {
public:
    UPlus(Expression* exp);
    double calculate() override;
};

// UMinus class
class UMinus : public UnaryOperator {
public:
    UMinus(Expression* exp);
    double calculate() override;
};

// BinaryOperator Class
class BinaryOperator : public Expression {
protected:
    Expression* left;
    Expression* right;
public:
    BinaryOperator(Expression* left, Expression* right);
    virtual ~BinaryOperator();
};

// Plus Class
class Plus : public BinaryOperator {
public:
    Plus(Expression *left, Expression *right);
    double calculate() override;
};

// Minus Class
class Minus : public BinaryOperator {
public:
    Minus(Expression* left, Expression* right);
    double calculate() override;
};

// Mul Class
class Mul : public BinaryOperator {
public:
    Mul(Expression* left, Expression* right);
    double calculate() override;
};

// Div Class
class Div : public BinaryOperator {
public:
    Div(Expression* left, Expression* right);
    double calculate() override;
};

// Interpreter Class

class Interpreter {
private:
    vector<string> listKeys;
    stack<string> stackOper;
    queue<string> queueNum;
    map<string, double> mapSet;
public:
    Expression* interpret(const string& str);
    void setVariables(const string& str);
    int precendenceCompare();
};

#endif //EX1_H



