#ifndef RPN_HPP
#define RPN_HPP

#include <list>
#include <string>

class RPN
{
public:
    RPN();
    long long evaluate(const std::string& expression);

private:
    std::list<long long> _stack;          // our only STL container
    bool        isOperator(const std::string& tok) const;
    long long   applyOp(long long a, long long b, char op);
};

#endif