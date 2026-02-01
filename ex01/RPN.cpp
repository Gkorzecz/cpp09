#include "RPN.hpp"
#include <sstream>
#include <stdexcept>
#include <cctype>

RPN::RPN()
{}

RPN::RPN(const RPN &other)
{
    (void)other;
}

RPN RPN::operator=(const RPN &other)
{
    (void)other;
    return (*this);
}

RPN::~RPN()
{}

bool RPN::isOperator(const std::string& token) const
{
    if (token.size() != 1)
        return (false);
    if (token != "+" && token != "-" && token != "*" && token != "/")
        return (false);
    return (true);
}

long RPN::calculate(long a, long b, char op)
{
    switch (op)
    {
        case '+':
            return (a + b);
        case '-':
            return (a - b);
        case '*':
            return (a * b);
        case '/':
            if (b == 0)
                throw std::runtime_error("division by zero");
            return (a / b);
        default :
            throw std::runtime_error("invalid operator");
    }
}

long RPN::evaluate(const std::string& expression)
{
    _stack.clear();
    std::stringstream ss(expression);
    std::string token;

    while (ss >> token)
    {
        if (token.size() == 1 && std::isdigit(token[0]))
        {
            _stack.push_back(token[0] - '0');
        }
        else if (isOperator(token))
        {
            if (_stack.size() < 2)
                throw std::invalid_argument("not enough elements");
            long b = _stack.back();
            _stack.pop_back();
            long a = _stack.back();
            _stack.pop_back();
            _stack.push_back(calculate(a, b, token[0]));
        }
        else
            throw std::invalid_argument("invalid element");
    }
    if (_stack.size() != 1)
        throw std::invalid_argument("can't calculate expression");
    return (_stack.back());
}   