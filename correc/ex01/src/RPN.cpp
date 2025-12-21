#include <sstream>
#include <stdexcept>

#include "RPN.hpp"

bool RPN::isOperator(char c) const {
	return (c == '+' || c == '-' || c == '*' || c == '/');
}

int RPN::applyOperator(int a, int b, char op) const {
	if (op == '+')
		return a + b;
	else if (op == '-')
		return a - b;
	else if (op == '*')
		return a * b;
	else if (op == '/') {
		if (b == 0)
			throw std::runtime_error("division by zero");
		return a / b;
	}
	throw std::runtime_error("invalid operator");
}

int RPN::evaluate(const std::string &expr) {
	std::istringstream iss(expr);
	std::string tok;

	while (iss >> tok) {
		if (tok.length() == 1 && isOperator(tok[0])) {
			if (_stack.size() < 2)
				throw std::runtime_error("not enough operands");
			int b = _stack.top();
			_stack.pop();
			int a = _stack.top();
			_stack.pop();
			int r = applyOperator(a, b, tok[0]);
			_stack.push(r);
		} else if (tok.length() == 1 && tok[0] >= '0' && tok[0] <= '9') {
			_stack.push(tok[0] - '0');
		} else {
			throw std::runtime_error("invalid token");
		}
	}
	if (_stack.size() != 1)
		throw std::runtime_error("invalid expression");
	return _stack.top();
}

RPN::RPN() {}
RPN::RPN(const RPN &other) {*this = other;}
RPN::~RPN() {}

RPN &RPN::operator=(const RPN &other) {
	if (this != &other) {
		this->_stack = other._stack;
	}
	return *this;
}
