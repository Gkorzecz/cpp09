#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <cstddef>
# include <cstdlib>
# include <ctime>
# include <deque>
# include <iostream>
# include <stdexcept>
# include <utility>
# include <vector>

std::vector<std::size_t>	computeJacobsthal(std::size_t n);
std::vector<int>			makePending(const std::vector<std::pair<int, int> >& pairs);

class PmergeMe {
	private :
		PmergeMe();
		PmergeMe(const PmergeMe &);
		PmergeMe	&operator=(const PmergeMe &);
		~PmergeMe();
	public :

		template <typename Container>
		static void	sort(Container &);
		template <typename Iterator>
		static bool isSorted(Iterator begin, Iterator end);
		template <typename Sorter, typename Container>
		static unsigned long	benchmarkSorting(Sorter sorter, Container& container);
};

# include "PmergeMe.tpp"

#endif
