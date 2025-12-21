#include "RPN.hpp"
#include <iostream>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "\e[31mError\e[0m" << std::endl;
		return 1;
	}
	RPN rpn;

	try {
		int result = rpn.evaluate(argv[1]);
		std::cout << result << std::endl;
	} catch (std::runtime_error &e) {
		std::cerr << "\e[31mError : \e[0m" << e.what() << std::endl;
		return 1;
	}
	return 0;
}
