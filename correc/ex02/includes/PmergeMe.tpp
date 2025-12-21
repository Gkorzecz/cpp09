#include "PmergeMe.hpp"

template <typename Container>
void	fillContainer(Container& c, int ac, char** av)
{
    for (int i = 1; i < ac; ++i) {
        char* end = NULL;
        long value = std::strtol(av[i], &end, 10);

        if (*end != '\0' || value < 0)
            throw std::exception();
        c.push_back(static_cast<typename Container::value_type>(value));
    }
}

template <typename Container>
void	displayContainer(Container c)
{
	for (size_t i = 0; i < c.size(); i++)
		std::cout << c[i] << " ";
}

template <typename Iterator>
bool	PmergeMe::isSorted(Iterator begin, Iterator end)
{
    if (begin == end)
        return true;

    Iterator prev = begin;
    Iterator curr = begin;
    ++curr;

    while (curr != end) {
        if (*curr < *prev)
            return false;
        prev = curr;
        ++curr;
    }
    return true;
}

template <typename Sorter, typename Container>
unsigned long	PmergeMe::benchmarkSorting(Sorter sorter, Container& container)
{
    clock_t start = clock();
	clock_t	end;

    sorter(container);
    end = clock();
    return static_cast<unsigned long>((end - start) * 1000000 / CLOCKS_PER_SEC);
}

template <typename Container>
std::size_t	binarySearch(const Container& c, int value, std::size_t left, std::size_t right)
{
    while (left < right) {
        std::size_t mid = left + (right - left) / 2;
        if (c[mid] < value)
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}

template <typename Container>
bool	initStraggler(Container& c, int& straggler)
{
    if (c.size() % 2 == 1) {
        straggler = c.back();
        c.pop_back();
        return true;
    }
    return false;
}

template <typename Container>
std::vector<std::pair<int, int> >	makePairs(Container& c)
{
    std::vector<std::pair<int, int> > pairs;

    for (std::size_t i = 0; i < c.size(); i += 2) {
        int a = c[i];
        int b = c[i + 1];
        if (a < b)
            pairs.push_back(std::make_pair(a, b));
        else
            pairs.push_back(std::make_pair(b, a));
    }
    c.clear();
    return pairs;
}

template <typename Container>
void	insertPending(Container& mainChain, const std::vector<int>& pending)
{
    std::vector<std::size_t> jacob = computeJacobsthal(pending.size());
    std::size_t inserted = 0;

    for (std::size_t k = 2; k < jacob.size(); ++k) {
        std::size_t limit = jacob[k];
        
        if (limit > pending.size())
            limit = pending.size();
        for (std::size_t i = limit; i > inserted; --i) {
            int value = pending[i - 1];
            std::size_t pos = binarySearch(mainChain, value, 0, mainChain.size());
            mainChain.insert(mainChain.begin() + pos, value);
        }
        inserted = limit;
    }
}

template <typename Container>
void	PmergeMe::sort(Container& c)
{
    if (c.size() <= 1)
        return;

    int  straggler = 0;
    bool hasStraggler = initStraggler(c, straggler);
    std::vector<std::pair<int, int> > pairs = makePairs(c);

    for (std::size_t i = 0; i < pairs.size(); ++i)
        c.push_back(pairs[i].second);
    sort(c);
    std::vector<int> pending = makePending(pairs);
    insertPending(c, pending);
    if (hasStraggler) {
        std::size_t pos = binarySearch(c, straggler, 0, c.size());
        c.insert(c.begin() + pos, straggler);
    }
}