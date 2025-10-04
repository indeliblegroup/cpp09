#include "RPN.hpp"

RPN::RPN()
{
    // constructor - stack is automatically initialized
}

RPN::RPN(const RPN& other)
{
    _stack = other._stack;
}

RPN& RPN::operator=(const RPN& other)
{
    if (this != &other)
    {
        _stack = other._stack;
    }
    return *this;
}

RPN::~RPN()
{
    // destructor - stack is automatically cleaned up
}

double RPN::evaluate(const std::string& expression)
{
    // clear the stack for new calculation
    while (!_stack.empty())
    {
        _stack.pop();
    }
    
    std::istringstream iss(expression);
    std::string token;
    
    while (iss >> token)
    {
        processToken(token);
    }
    
    // after processing all tokens, stack should contain exactly one element
    if (_stack.size() != 1)
    {
        throw std::runtime_error("Error");
    }
    
    return _stack.top();
}

void RPN::processToken(const std::string& token)
{
    if (isNumber(token))
    {
        // push number onto stack
        double num = std::strtod(token.c_str(), NULL);
        _stack.push(num);
    }
    else if (isOperator(token))
    {
        // need at least 2 operands for operation
        if (_stack.size() < 2)
        {
            throw std::runtime_error("Error");
        }
        
        // pop two operands
        double b = _stack.top();
        _stack.pop();
        double a = _stack.top();
        _stack.pop();
        
        // perform operation and push result
        double result = performOperation(a, b, token);
        _stack.push(result);
    }
    else
    {
        // invalid token
        throw std::runtime_error("Error");
    }
}

bool RPN::isOperator(const std::string& token) const
{
    return (token == "+" || token == "-" || token == "*" || token == "/");
}

bool RPN::isNumber(const std::string& token) const
{
    if (token.empty())
        return false;
    
    // check if all characters are digits (single digit numbers only)
    for (size_t i = 0; i < token.length(); ++i)
    {
        if (!std::isdigit(token[i]))
            return false;
    }
    
    // check if number is less than 10
    int num = std::atoi(token.c_str());
    return (num >= 0 && num < 10);
}

double RPN::performOperation(double a, double b, const std::string& op) const
{
    if (op == "+")
        return a + b;
    else if (op == "-")
        return a - b;
    else if (op == "*")
        return a * b;
    else if (op == "/")
    {
        if (b == 0)
            throw std::runtime_error("Error: division by zero");
        return a / b;
    }
    else
        throw std::runtime_error("Error: unknown operator");
}
