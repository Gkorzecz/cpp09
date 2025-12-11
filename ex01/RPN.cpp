#include "RPN.hpp"
#include <sstream>
#include <stdexcept>
#include <cctype>

RPN::RPN() {}

bool RPN::isOperator(const std::string& tok) const
{
    return tok.size() == 1 && (tok == "+" || tok == "-" || tok == "*" || tok == "/");
}

long long RPN::applyOp(long long a, long long b, char op)
{
    switch (op)
    {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) throw std::runtime_error("division by zero");
            return a / b;
    }
    throw std::runtime_error("invalid operator");
}

long long RPN::evaluate(const std::string& expression)
{
    _stack.clear();
    std::stringstream ss(expression);
    std::string tok;

    while (ss >> tok)
    {
        if (tok.size() == 1 && std::isdigit(tok[0]))          // single-digit operand
        {
            _stack.push_back(tok[0] - '0');
        }
        else if (isOperator(tok))
        {
            if (_stack.size() < 2) throw std::runtime_error("not enough operands");
            long long b = _stack.back(); _stack.pop_back();
            long long a = _stack.back(); _stack.pop_back();
            _stack.push_back(applyOp(a, b, tok[0]));
        }
        else
            throw std::runtime_error("invalid token");
    }
    if (_stack.size() != 1) throw std::runtime_error("malformed expression");
    return _stack.back();
}   