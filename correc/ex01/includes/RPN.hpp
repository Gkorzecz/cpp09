#ifndef RPN_HPP
# define RPN_HPP

# include <stack>
# include <string>

class RPN {
	private:
		std::stack<int>	_stack;
		bool	isOperator(char) const;
		int		applyOperator(int, int, char) const;

	public:
		RPN();
		RPN(const RPN &);
		RPN &operator=(const RPN &);
		~RPN();

		int	evaluate(const std::string &);
};

#endif

