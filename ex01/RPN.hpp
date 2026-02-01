#ifndef RPN_HPP
#define RPN_HPP

#include <list>
#include <string>

class RPN
{
public:
    RPN();
    RPN(const RPN &other);
    RPN operator=(const RPN &other);
    ~RPN();
    long evaluate(const std::string& expression);

private:
    std::list<long> _stack;
    bool isOperator(const std::string& token) const;
    long calculate(long a, long b, char op);
};

#endif