#include "PmergeMe.hpp"

int	main(int ac, char **av) {
	std::vector<int>	vec;
	std::deque<int>		deq;
	unsigned long		vec_time;
	unsigned long		deq_time;

	try {
		fillContainer(vec, ac, av);
		fillContainer(deq, ac, av);
	} catch (std::exception &e) {
		std::cerr << "Error\n";
		return 1;
	}
	std::cout << "unsorted : ";
	displayContainer(vec);
	std::cout << "\n";
	vec_time = PmergeMe::benchmarkSorting(PmergeMe::sort<std::vector<int> >, vec);
	deq_time = PmergeMe::benchmarkSorting(PmergeMe::sort<std::deque<int> >, deq);
	if (!PmergeMe::isSorted(vec.begin(), vec.end()) || !PmergeMe::isSorted(deq.begin(), deq.end()))
		return std::cout << "\e[31m sorting failed !\e[0m\n", 1;
	std::cout << "sorted : ";
	displayContainer(vec);
	std::cout << "\n";
	std::cout << "vector was sorted in " << vec_time << "\n";
	std::cout << "deque was sorted in " << deq_time << "\n";
	return 0;
}
