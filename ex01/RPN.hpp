#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>

class RPN
{
private:
    std::stack<double> _stack; // stack to store operands for rpn calculation
    
    // private helper methods
    bool isOperator(const std::string& token) const;
    bool isNumber(const std::string& token) const;
    double performOperation(double a, double b, const std::string& op) const;
    void processToken(const std::string& token);

public:
    // orthodox canonical form
    RPN();
    RPN(const RPN& other);
    RPN& operator=(const RPN& other);
    ~RPN();
    
    // main method to evaluate rpn expression
    double evaluate(const std::string& expression);
};

#endif
