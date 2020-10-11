#include "ex1.h"
#include <iostream>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <sstream>
using namespace std;

// constructor of Value class
Value::Value(const double val) {
    this->val = val;
    // Value::num = num;
}

// Calculate function of Value class
double Value::calculate() {
    return this->val;
}

// constructor of Variable class
Variable::Variable(const string &name, const double &value) {
    this->name = name;
    this->value = value;
}

// Operator ++
Variable& Variable::operator++() {
    this->value++;
    return *this;
}

Variable& Variable::operator--() {
    this->value--;
    return *this;
}

Variable& Variable::operator-=(double val) {
    this->value -= val;
    return *this;
}

Variable& Variable::operator+=(double val) {
    this->value += val;
    return *this;
}

double Variable::calculate() {
    return this->value;
}

Variable& Variable::operator++(int num) {
    ++value;
    return *this;
}

Variable& Variable::operator--(int num) {
    --value;
    return *this;
}

// UnaryOperator class Constructor
UnaryOperator::UnaryOperator(Expression* exp) {
    this->exp = exp;
}

// UnaryOperator class DesConstructor
UnaryOperator::~UnaryOperator() {
    if (this->exp != NULL) {
        delete this->exp;
    }
}

//Constructor Uplus
UPlus::UPlus(Expression* exp):
        UnaryOperator(exp) {
}

double UPlus::calculate() {
    return this->exp->calculate();
}

//Constructor UMinus
UMinus::UMinus(Expression* exp) :
        UnaryOperator(exp) {
}

double UMinus::calculate() {
    return -(this->exp->calculate());
}

// BinaryOperator Class Constructor
BinaryOperator::BinaryOperator(Expression* left, Expression* right) {
    this->left = left;
    this->right = right;
}

// UnaryOperator class DesConstructor
BinaryOperator::~BinaryOperator() {
    if (this->left != NULL) {
        delete this->left;
    }
    if (this->right != NULL) {
        delete this->right;
    }
}

// Plus Class Constructor
Plus::Plus(Expression* left, Expression* right) : BinaryOperator(left, right) {
}

double Plus::calculate() {
    return this->left->calculate() + this->right->calculate();
}

// Minus Class Constructor
Minus::Minus(Expression* left, Expression* right) :
        BinaryOperator(left, right) {
}

double Minus::calculate() {
    return (this->left->calculate() - this->right->calculate());
}

// Mul Class Constructor
Mul::Mul(Expression* left, Expression* right) :
        BinaryOperator(left, right) {
}

double Mul::calculate() {
    return (this->left->calculate() * this->right->calculate());
}

// Div Class Constructor
Div::Div(Expression* left, Expression* right) :
        BinaryOperator(left, right) {
}

double Div::calculate() {
    if (this->right->calculate() == 0) {
        throw "Error! divide in zero!";
    }
    return (this->left->calculate() / this->right->calculate());
}

// Interpreter Class

bool isOperator(const char c) {
    if(c == '*' ||c == '/' || c == '+' || c == '-') {
        return true;
    }
    return false;
}

// Function that convert string to double and return the value.
double convertStringToDouble(const string& str) {
    char *p = new char[str.size() + 1];
    strcpy(p, str.c_str());
    return strtod(p,NULL);
}

string convertDoubleToString(const double num) {
    ostringstream s;
    s << num;
    return s.str();
};

Expression* makeExpression(queue<string> &queue) {
    stack<Expression*> expStack;
    Expression* leftExp;
    Expression* rightExp;
    // loop over the queue
    while (!queue.empty()) {
        Expression* exp;
        // check if it's digit so push to stack
        if (isdigit(queue.front()[0])) {
            exp = new Value(convertStringToDouble(queue.front()));
            expStack.push(exp);
        } // check if this is operator.
      //  else if (isOperator(queue.front()[0])) {
      else {
            rightExp = expStack.top();
            expStack.pop();
            // check if the stack is empty and there is '$' in the queue.
            if (queue.front()[0] == '$' && expStack.empty()) {
                return new UMinus(rightExp);
            }
            // check if '+'
            if (queue.front()[0] == '#' && expStack.empty()) {
                return new UPlus(rightExp);
            }
            leftExp = expStack.top();
            if (queue.front()[0] != '$') {
                expStack.pop();
            }
            Expression* val;
            switch (queue.front()[0]) {
                case '+':
                    val = new Plus(leftExp, rightExp);
                    expStack.push(val);
                    break;
                case '-':
                    val = new Minus(leftExp, rightExp);
                    expStack.push(val);
                    break;
                case '*':
                    val = new Mul(leftExp, rightExp);
                    expStack.push(val);
                    break;
                case '/':
                    val = new Div(leftExp, rightExp);
                    expStack.push(val);
                    break;
                case '$':
                    val = new UMinus(rightExp);
                    expStack.push(val);
                    break;
                case '#':
                    val = new UPlus(rightExp);
                    expStack.push(val);
                    break;
            }
        }
      queue.pop();
    }
    if (expStack.size() > 1) {
        cout << "more than 1 variable in stack";
    }
    return expStack.top();
}

Expression * Interpreter::interpret(const string &str) {
    int i = 0,j = 0;
    // check the string
    while (str[i] != '\0') {
        if(isOperator(str[0]) && str[0] != '-' && str[0] != '+') {
            throw "invalid exeption";
        }
        // check if the char is Operator
        if(isOperator(str[i])){
            // check the problem with '-' in the start, if it's unary expression.
            // and if it's '-' and before we have '('
            // push '$' to recognize.
            if ((str[i] == '-' && i == 0) || (str[i] == '-' && str[i-1] == '(')) {
                this->stackOper.push("$");
            }
            else if ((str[i] == '+' && i == 0) || (str[i] == '+' && str[i-1] == '(')) {
                this->stackOper.push("#");
            }
            else if (this->stackOper.empty()) {
                string currentI(1, str[i]);
                this->stackOper.push(currentI);
            }
            // the top have precedence
            else if (((this->stackOper.top() == "*") && (str[i] == '+' || str[i] == '-')) ||
                    ((this->stackOper.top() == "/") && (str[i] == '+' || str[i] == '-'))) {
                // while we have * and / we need to pop them from stack and move to queue
                while (this->stackOper.top() == "*" || this->stackOper.top() == "*") {
                    this->stackOper.top() = this->stackOper.top();
                    this->stackOper.pop();
                    this->queueNum.push(this->stackOper.top());
                }
                // now push the operator to stack.
                string currentI(1, str[i]);
                this->stackOper.push(currentI);
            }
            else {
                // convert to string from char
                string s( 1, str[i]);
                this->stackOper.push(s);
            }
        }
        // check Parenthesis
        else if (str[i] == '(') {
            string s( 1, str[i]);
            this->stackOper.push(s);
        }
        // Pop all the stack and push it to queue until the ')'
        else if (str[i] == ')') {
            while (this->stackOper.top() != "(") {
                string topOnStack = this->stackOper.top();
                this->stackOper.pop();
                this->queueNum.push(topOnStack);
                // if we don't have ')'
                if (this->stackOper.empty()) {
                    throw "No close ')' Parenthesis";
                }
            }
            // remove the ')' from stack
            this->stackOper.pop();
        }
        // check Number or Variable
        else {
            // if the char is digit
            if (isdigit(str[i])) {
                int ifValidDot = 0;
                string num ="";
                int g = i;
                // check until the number is finish, and check for dots.
                while (isdigit(str[g]) || str[g] == '.') {
                    if (str[g] == '.') {
                        ifValidDot++;
                        if (g < str.size() && !isdigit(str[g + 1])) {
                            throw "Erorr, not valid exception like 5.";
                        }
                    }
                    num += str[g];
                    g++;
                }// check if more than 2 dots not valid
                if (ifValidDot > 1) {
                    throw "Error, more than one '.'";
                }
                // push the number to the queue.
                this->queueNum.push(num);
                i = g - 1;
            }
            // check if it's variables.
            else if (isalpha(str[i])) {
                string var = "";
                int g = i;
                // while its a variable.
                while (str[g] != '(' && str[g] != ')' &&
                       !isOperator(str[g]) && str[g] != '\0') {
                    var += str[g];
                    g++;
                }
                // check if the var is in the map
                if (this->mapSet.find(var) != mapSet.end()) {
                    string temp = convertDoubleToString(this->mapSet.at(var));
                    this->queueNum.push(temp);
                } else {
                    throw "No Var in map";
                }
            }
        }
        i++;
    }
    // remove all the stack to queue
    while (!this->stackOper.empty()) {
        this->queueNum.push(this->stackOper.top());
        this->stackOper.pop();
    }
//    while (!this->queueNum.empty()) {
//        cout << this->queueNum.front();
//        this->queueNum.pop();
//    }
    return makeExpression(this->queueNum);
}

// check validity of number.
bool numberValid(const string& num){
    int start = 0, countPoints = 0;
    // check if the first digit is negative so start ++.
    if ((num[start] == '-') && (num.size() > 1) && (num[start + 1] >= '0') &&
        (num[start + 1] <= '9')) {
        start++;
    }
    // loop over the input number and check if valid.
    while ((start < num.size() && num[start] >= '0' && num[start] <= '9') ||
           num[start] == '.') {
        // check in case of '.'
        if (num[start] == '.') {
            countPoints++;
        }
        start++;
    }
    // check if there is more than 1 of '.' is not valid
    if (countPoints > 1) {
        return false;
    }
    // if the word is valid start is in the end.
    if (start == num.size()) {
        return true;
    }
    return false;
}

// check validity of variable.
bool variableValid(const string& var){
    int start = 0, countPoints = 0;
    // if not start with alphabat word return false
    if (!isalpha(var[0])) {
        return false;
    }
    start++;
    while (start < var.size() || isalpha(var[start])  || isdigit(var[start])
        || var[start] == '_') {
        start++;
    }
    if (start == var.size()) {
        return true;
    }
    return false;
}

bool checkValidity(string input, map<string, double>& mapT, vector<string> &listKeys) {
    int x = input.find('=');
    // if we don't have "=" so return false.
    if (x == string::npos) {
        return false;
    }
    string left = input.substr(0,x);
    string right = input.substr(x + 1, input.size() - x);
    //
    if (variableValid(left) && numberValid(right)) {
        double newRight = convertStringToDouble(right);
        // check if the key is allready in the map so replace the value of it.
        if (mapT.find(left) != mapT.end()) {
            mapT.at(left) = newRight;
        }
        else {
            listKeys.push_back(left);
            mapT.insert(pair<string, double> (left, newRight));
        }
        return true;
    }
    return false;
}

void Interpreter::setVariables(const string &str) {
    int y=0;
    string tempS = str;
    vector<string> varList;
    // push to the list all the sentences until ;
    while (tempS.find(';') != std::string::npos) {
        y = tempS.find(';');
        varList.push_back(tempS.substr(0,y));
        tempS = tempS.substr(y+1,tempS.size() - y);
    }
    // it is valid to sometimes include “;”, and sometimes
    //not.
    if (tempS.size() > 1) {
        varList.push_back(tempS);
    }
    for(int i = 0;i < varList.size();i++) {
        try {
            checkValidity(varList[i], this->mapSet, this->listKeys);
        }
        catch (const char *p) {
            throw p;
        }
    }
}










